#include "reporter.h"
#include "colors.h"
#include <iostream>
#include <iomanip>

void Reporter::printSummary(const Analyzer& analyzer) const {

    // alertas de fuerza bruta 
    const auto& alertas = analyzer.alertas();
    if (!alertas.empty()) {
        std::cout << Color::BOLD << Color::RED
                  << "\n=== ALERTAS: Posible fuerza bruta ==="
                  << Color::RESET << "\n";

        for (const auto& [ip, intentos] : alertas) {
            std::cout << Color::RED << "  [!] IP: "
                      << Color::YELLOW << std::setw(16) << std::left << ip
                      << Color::RESET
                      << " -> " << intentos << " intentos en ventana de tiempo\n";
        }
    } else {
        std::cout << Color::GREEN
                  << "\n=== Sin alertas de fuerza bruta ==="
                  << Color::RESET << "\n";
    }

    // failed passwords por IP
    std::cout << Color::BOLD << Color::CYAN
              << "\n=== Failed password por IP ==="
              << Color::RESET << "\n";

    if (analyzer.failedByIp().empty()) {
        std::cout << "  (ninguno)\n";
    }
    for (const auto& [ip, timestamps] : analyzer.failedByIp()) {
        std::cout << "  " << Color::YELLOW << std::setw(16) << std::left << ip
                  << Color::RESET
                  << " -> " << timestamps.size() << " intento(s)\n";
    }

    // invalid users por IP 
    std::cout << Color::BOLD << Color::CYAN
              << "\n=== Invalid user por IP ==="
              << Color::RESET << "\n";

    if (analyzer.invalidByIp().empty()) {
        std::cout << "  (ninguno)\n";
    }
    for (const auto& [ip, count] : analyzer.invalidByIp()) {
        std::cout << "  " << Color::YELLOW << std::setw(16) << std::left << ip
                  << Color::RESET
                  << " -> " << count << " intento(s)\n";
    }

    std::cout << "\n";
}
