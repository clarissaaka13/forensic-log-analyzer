#include "auth_parser.h"
#include <ctime>
#include <cstring>

// safe_substr: extrae substring de forma segura sin lanzar excepciones

std::string AuthParser::safe_substr(const std::string& s, size_t start, size_t len) const {
    if (start >= s.size()) return "";
    if (start + len > s.size()) len = s.size() - start;
    return s.substr(start, len);
}

// extrae "Feb 18 10:01:01" del inicio de la línea del log
// como el log no incluye año, usamos el año actual

time_t AuthParser::parseTimestamp(const std::string& line) const {
    struct tm tm = {};

    // toma el año actual 
    time_t ahora = time(nullptr);
    struct tm* ahora_tm = localtime(&ahora);
    tm.tm_year = ahora_tm->tm_year;

    // formato
    if (strptime(line.c_str(), "%b %d %H:%M:%S", &tm) == nullptr) {
        return 0;  // si falla el parseo, regresa 0
    }

    tm.tm_isdst = -1;  // horario de verano
    return mktime(&tm);
}

// detecta si la línea es un evento relevante y extrae sus campos

bool AuthParser::tryParseAuthLine(const std::string& line, Event& out) const {
    out = Event{};
    out.raw       = line;
    out.type      = EventType::Other;
    out.timestamp = parseTimestamp(line); // timestamp real del log

    // caso 1: "Failed password" 
    if (line.find("Failed password") != std::string::npos) {
        out.type = EventType::FailedPassword;

        size_t posFor  = line.find("for ");
        size_t posFrom = line.find(" from ");

        if (posFor  != std::string::npos &&
            posFrom != std::string::npos &&
            posFrom > posFor + 4)
        {
            out.user = safe_substr(line, posFor + 4, posFrom - (posFor + 4));
        }

        if (posFrom != std::string::npos) {
            size_t ipStart = posFrom + 6;
            size_t ipEnd   = line.find(" ", ipStart);
            if (ipEnd != std::string::npos && ipEnd > ipStart) {
                out.ip = safe_substr(line, ipStart, ipEnd - ipStart);
            }
        }

        return true;
    }

    // caso 2: "Invalid user" 
    if (line.find("Invalid user") != std::string::npos) {
        out.type = EventType::InvalidUser;

        size_t posInv  = line.find("Invalid user ");
        size_t posFrom = line.find(" from ");

        if (posInv  != std::string::npos &&
            posFrom != std::string::npos &&
            posFrom > posInv + 13)
        {
            out.user = safe_substr(line, posInv + 13, posFrom - (posInv + 13));
        }

        if (posFrom != std::string::npos) {
            size_t ipStart = posFrom + 6;
            size_t ipEnd   = line.find(" ", ipStart);
            if (ipEnd != std::string::npos && ipEnd > ipStart) {
                out.ip = safe_substr(line, ipStart, ipEnd - ipStart);
            }
        }

        return true;
    }

    return false;
}
