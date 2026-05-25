cmake_minimum_required(VERSION 3.16)
project(LogAnalyzer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(fla
    main.cpp
    log_reader.cpp
    auth_parser.cpp
    analyzer.cpp
    reporter.cpp
    file_watcher.cpp
    csv_exporter.cpp
)
