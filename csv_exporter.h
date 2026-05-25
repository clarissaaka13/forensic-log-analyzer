#pragma once
#include <string>
#include "analyzer.h"

class CsvExporter {
public:
    bool exportar(const Analyzer& analyzer, const std::string& path) const;
};
