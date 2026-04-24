#pragma once
#include <fstream>
#include <string>


class LogReader {
private:
    std::ifstream file_;

public:
    explicit LogReader(const std::string& path);

    // true si el archivo se abrió correctamente
    bool ok() const;

    // lee la siguiente línea 
    bool nextLine(std::string& line);
};
