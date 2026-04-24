
#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>
#include "models.h"
#include "config.h"

// acumula estadísticas de eventos
class Analyzer {
private:
    int umbral_;
    int ventana_;
 // IP -> lista de timestamps de intentos fallidos
    std::unordered_map<std::string, std::vector<time_t>> failedByIp_;
//  IP -> contador de usuarios inválidos
    std::unordered_map<std::string, int>                 invalidByIp_;
//  alertas generadas
    std::vector<std::pair<std::string, int>>             alertas_;

    // para no repetir la misma alerta por IP
    std::unordered_map<std::string, int> ultimaAlertaPor_;

public:
    // constructor con valores por defecto de config.h
    explicit Analyzer(int umbral = UMBRAL_INTENTOS, int ventana = VENTANA_SEGUNDOS)
        : umbral_(umbral), ventana_(ventana) {}

    void consume(const Event& ev);

    const std::unordered_map<std::string, std::vector<time_t>>& failedByIp() const;
    const std::unordered_map<std::string, int>&                 invalidByIp() const;
    const std::vector<std::pair<std::string, int>>&             alertas()     const;
};

