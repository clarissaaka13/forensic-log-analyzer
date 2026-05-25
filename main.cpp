#include <iostream>
#include <string>
#include <csignal>
#include <atomic>

#include "config.h"
#include "colors.h"
#include "log_reader.h"
#include "auth_parser.h"
#include "analyzer.h"
#include "reporter.h"
#include "file_watcher.h"
#include "csv_exporter.h"

std::atomic<bool> g_interrupted{false};

void manejadorSenal(int) {
    g_interrupted = true;
}

void printUso(const char* prog) {
    std::cout << Color::CYAN << "Uso: " << Color::RESET
              << prog << " <archivo.log> [opciones]\n\n"
              << "Opciones:\n"
              << "  --umbral N      Intentos para disparar alerta (default: "
              << UMBRAL_INTENTOS << ")\n"
              << "  --ventana N     Ventana de tiempo en segundos (default: "
              << VENTANA_SEGUNDOS << ")\n"
              << "  --watch         Monitorea el archivo en tiempo real\n"
              << "  --csv <archivo> Exporta reporte a .csv\n\n"
              << "Ejemplos:\n"
              << "  " << prog << " data/sample_auth.log\n"
              << "  " << prog << " data/sample_auth.log --watch\n"
              << "  " << prog << " data/sample_auth.log --csv reporte.csv\n";
}

int main(int argc, char* argv[]) {
    std::signal(SIGINT, manejadorSenal);

    std::string rutaLog = LOG_DEFAULT;
    std::string rutaCsv = "";
    int  umbral         = UMBRAL_INTENTOS;
    int  ventana        = VENTANA_SEGUNDOS;
    bool modoWatch      = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            printUso(argv[0]);
            return 0;
        }
        else if (arg == "--umbral" && i + 1 < argc) {
            umbral = std::stoi(argv[++i]);
        }
        else if (arg == "--ventana" && i + 1 < argc) {
            ventana = std::stoi(argv[++i]);
        }
        else if (arg == "--watch") {
            modoWatch = true;
        }
        else if (arg == "--csv" && i + 1 < argc) {
            rutaCsv = argv[++i];
        }
        else if (arg[0] != '-' && rutaLog == LOG_DEFAULT) {
            rutaLog = arg;
        }
    }

    std::cout << Color::BOLD << "\nLog Analyzer\n" << Color::RESET
              << "Archivo : " << Color::YELLOW << rutaLog << Color::RESET << "\n"
              << "Umbral  : " << Color::YELLOW << umbral  << Color::RESET << " intentos\n"
              << "Ventana : " << Color::YELLOW << ventana << Color::RESET << " segundos\n"
              << "Modo    : " << Color::YELLOW
              << (modoWatch ? "watch (tiempo real)" : "normal")
              << Color::RESET << "\n";

    if (!rutaCsv.empty()) {
        std::cout << "CSV     : " << Color::YELLOW << rutaCsv << Color::RESET << "\n";
    }
    std::cout << "\n";

    AuthParser parser;
    Analyzer   analyzer(umbral, ventana);
    Reporter   reporter;

    size_t alertasImpresas = 0;

    auto procesarLinea = [&](const std::string& line) {
        Event ev;
        if (parser.tryParseAuthLine(line, ev)) {
            analyzer.consume(ev);
            if (modoWatch) {
                const auto& alertas = analyzer.alertas();
                while (alertasImpresas < alertas.size()) {
                    const auto& a = alertas[alertasImpresas];
                    std::cout << Color::RED << "[!] ALERTA: "
                              << Color::YELLOW << a.first << Color::RESET
                              << " -> " << a.second
                              << " intentos en ventana de tiempo\n";
                    alertasImpresas++;
                }
            }
        }
    };

    if (modoWatch) {
        FileWatcher watcher;
        watcher.watch(rutaLog, procesarLinea);
        std::cout << Color::YELLOW
                  << "\n[watch detenido - reporte final:]\n"
                  << Color::RESET;
    } else {
        LogReader reader(rutaLog);
        if (!reader.ok()) {
            std::cerr << Color::RED << "Error: no se pudo abrir: "
                      << rutaLog << Color::RESET << "\n";
            return 1;
        }
        std::cout << Color::CYAN << "(Ctrl+C para reporte parcial)\n\n"
                  << Color::RESET;

        std::string line;
        while (!g_interrupted && reader.nextLine(line)) {
            procesarLinea(line);
        }

        if (g_interrupted) {
            std::cout << Color::YELLOW
                      << "\n[Interrumpido - reporte parcial:]\n"
                      << Color::RESET;
        }
    }

    reporter.printSummary(analyzer);

    if (!rutaCsv.empty()) {
        CsvExporter exporter;
        if (exporter.exportar(analyzer, rutaCsv)) {
            std::cout << Color::GREEN << "Reporte CSV guardado en: "
                      << Color::YELLOW << rutaCsv
                      << Color::RESET << "\n\n";
        }
    }

    return 0;
}

