#include "csv_exporter.h"
#include "colors.h"
#include <fstream>
#include <iostream>
#include <ctime>

bool CsvExporter::exportar(const Analyzer& analyzer, const std::string& path) const {

    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << Color::RED << "Error: no se pudo crear: "
                  << path << Color::RESET << "\n";
        return false;
    }

    time_t ahora = time(nullptr);
    char fechaStr[64];
    strftime(fechaStr, sizeof(fechaStr), "%Y-%m-%d %H:%M:%S", localtime(&ahora));

    file << "# Reporte generado: " << fechaStr << "\n\n";

    file << "ALERTAS DE FUERZA BRUTA\n";
    file << "IP,Intentos en ventana,Tipo\n";
    const auto& alertas = analyzer.alertas();
    if (alertas.empty()) {
        file << "sin alertas,,\n";
    } else {
        for (const auto& [ip, intentos] : alertas) {
            file << ip << "," << intentos << ",Fuerza bruta\n";
        }
    }

    file << "\nFAILED PASSWORD POR IP\n";
    file << "IP,Total intentos\n";
    if (analyzer.failedByIp().empty()) {
        file << "ninguno,\n";
    } else {
        for (const auto& [ip, timestamps] : analyzer.failedByIp()) {
            file << ip << "," << timestamps.size() << "\n";
        }
    }

    file << "\nINVALID USER POR IP\n";
    file << "IP,Total intentos\n";
    if (analyzer.invalidByIp().empty()) {
        file << "ninguno,\n";
    } else {
        for (const auto& [ip, count] : analyzer.invalidByIp()) {
            file << ip << "," << count << "\n";
        }
    }

    file.close();
    return true;
}
