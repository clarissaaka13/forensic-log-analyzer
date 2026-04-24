#include "log_reader.h"

LogReader::LogReader(const std::string& path) {
    file_.open(path);
}

bool LogReader::ok() const {
    return file_.is_open();
}

bool LogReader::nextLine(std::string& line) {
    return static_cast<bool>(std::getline(file_, line));
}



