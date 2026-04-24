#pragma once

/*
  colors.h
  Códigos ANSI para colorear la consola.
  Úsalos así: cout << RED << "texto" << RESET;
*/

namespace Color {
    constexpr const char* RESET   = "\033[0m";
    constexpr const char* RED     = "\033[31m";
    constexpr const char* YELLOW  = "\033[33m";
    constexpr const char* GREEN   = "\033[32m";
    constexpr const char* CYAN    = "\033[36m";
    constexpr const char* BOLD    = "\033[1m";
}


