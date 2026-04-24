#pragma once
#include <string>
#include <ctime>

enum class EventType {
    FailedPassword,   
    InvalidUser,      
    Other             
};

struct Event {
    std::string raw;        // línea original completa
    EventType   type;       // tipo de evento detectado
    std::string user;       // usuario involucrado
    std::string ip;         // IP involucrada
    time_t      timestamp;  // timestamp real parseado del log
};
