#pragma once
#include "analyzer.h"

// imprime resultados 
class Reporter {
public:
    // imprime resumen
    void printSummary(const Analyzer& analyzer) const;
};

