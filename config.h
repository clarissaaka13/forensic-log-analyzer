
#pragma once


// número de intentos fallidos antes de disparar alerta
constexpr int UMBRAL_INTENTOS     = 3;

// ventana de tiempo en segundos para contar intentos
constexpr int VENTANA_SEGUNDOS    = 60;

// ruta por defecto del log 
constexpr const char* LOG_DEFAULT = "data/sample_auth.log";

