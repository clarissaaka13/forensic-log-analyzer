
#include "analyzer.h"
#include <algorithm>

void Analyzer::consume(const Event& ev) {

    if (ev.type == EventType::FailedPassword) {
        auto& lista = failedByIp_[ev.ip];
        lista.push_back(ev.timestamp);

        // limpia  intentos fuera de la ventana
        lista.erase(
            std::remove_if(lista.begin(), lista.end(), [&](time_t t) {
                return difftime(ev.timestamp, t) > ventana_;
            }),
            lista.end()
        );

        int attempts = static_cast<int>(lista.size());

        // alerta solo si supera umbral Y es mayor que la última alerta (así no se repite la misma IP infinitamente)
        if (attempts >= umbral_) {
            if (ultimaAlertaPor_.find(ev.ip) == ultimaAlertaPor_.end()) {
                ultimaAlertaPor_[ev.ip] = attempts;
                alertas_.push_back({ev.ip, attempts});
            }
        }
    }

    if (ev.type == EventType::InvalidUser) {
        invalidByIp_[ev.ip]++;
    }
}

const std::unordered_map<std::string, std::vector<time_t>>& Analyzer::failedByIp() const {
    return failedByIp_;
}

const std::unordered_map<std::string, int>& Analyzer::invalidByIp() const {
    return invalidByIp_;
}

const std::vector<std::pair<std::string, int>>& Analyzer::alertas() const {
    return alertas_;
}

