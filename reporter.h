#pragma once
#include "analyzer.h"

class Reporter {
public:
    void printSummary(const Analyzer& analyzer) const;
};
