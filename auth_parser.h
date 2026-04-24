#pragma once
#include <string>
#include "models.h"

// parsea una linea de auth.log y la convierte en un Event
class AuthParser {
public:

// devuelve true si detectó un evento útil (FailedPassword o InvalidUser)
// devuelve false si la línea no nos interesa
// llena out.timestamp con el tiempo parseado del log
    
    bool tryParseAuthLine(const std::string& line, Event& out) const;

private:
    // extrae timestamp del inicio de la línea ("Feb 18 10:01:01")
    time_t parseTimestamp(const std::string& line) const;

    // extrae un substring de forma segura
    std::string safe_substr(const std::string& s, size_t start, size_t len) const;
};
