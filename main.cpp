
// acepta argumentos opcionales:
   // --umbral N   : intentos antes de disparar alerta (default config.h)
    //--ventana N  : segundos de la ventana de tiempo  (default config.h)


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

static std::atomic<bool> g_interrupted{false};

void manejadorSenal(int /*signum*/) {
    g_interrupted = true;
}

// imprime cómo usar el programa
void printUso(const char* prog) {
    std::cout << Color::CYAN << "Uso: " << Color::RESET
              << prog << " <archivo.log> [opciones]\n\n"
              << "Opciones:\n"
              << "  --umbral N   Intentos para disparar alerta (default: "
              << UMBRAL_INTENTOS << ")\n"
              << "  --ventana N  Ventana de tiempo en segundos (default: "
              << VENTANA_SEGUNDOS << ")\n\n"
              << "Ejemplo:\n"
              << "  " << prog << " data/sample_auth.log --umbral 5 --ventana 120\n";
}

int main(int argc, char* argv[]) {
    std::signal(SIGINT, manejadorSenal);

    // valores por defecto desde config.h
    std::string rutaLog  = LOG_DEFAULT;
    int umbral           = UMBRAL_INTENTOS;
    int ventana          = VENTANA_SEGUNDOS;

    // parseo de argumentos
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
        else if (arg[0] != '-') {
            // si no empieza con '-', es la ruta del archivo
            rutaLog = arg;
        }
    }

    LogReader reader(rutaLog);
    if (!reader.ok()) {
        std::cerr << Color::RED << "Error: " << Color::RESET
                  << "no se pudo abrir el archivo: " << rutaLog << "\n";
        return 1;
    }

    std::cout << Color::BOLD << "\nLog Analyzer\n" << Color::RESET
              << "Archivo : " << Color::YELLOW << rutaLog  << Color::RESET << "\n"
              << "Umbral  : " << Color::YELLOW << umbral   << Color::RESET << " intentos\n"
              << "Ventana : " << Color::YELLOW << ventana  << Color::RESET << " segundos\n"
              << Color::CYAN << "(Ctrl+C para reporte parcial)\n" << Color::RESET << "\n";

    // pasamos umbral y ventana al Analyzer
    AuthParser parser;
    Analyzer   analyzer(umbral, ventana);
    std::string line;

    while (!g_interrupted && reader.nextLine(line)) {
        Event ev;
        if (parser.tryParseAuthLine(line, ev)) {
            analyzer.consume(ev);
        }
    }

    if (g_interrupted) {
        std::cout << Color::YELLOW
                  << "\n[Interrumpido — reporte parcial:]\n"
                  << Color::RESET;
    }

    Reporter reporter;
    reporter.printSummary(analyzer);

    return 0;
}



