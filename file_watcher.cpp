#include "file_watcher.h"
#include "colors.h"

#include <atomic>
#include <sys/inotify.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

extern std::atomic<bool> g_interrupted;

void FileWatcher::watch(const std::string& path,
                        std::function<void(const std::string&)> callback) {

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << Color::RED << "Error: no se pudo abrir: "
                  << path << Color::RESET << "\n";
        return;
    }

    // Solo leer lineas NUEVAS, nos vamos al final
    file.seekg(0, std::ios::end);

    int fd = inotify_init1(IN_NONBLOCK);
    if (fd < 0) {
        std::cerr << Color::RED << "Error: no se pudo inicializar inotify"
                  << Color::RESET << "\n";
        return;
    }

    int wd = inotify_add_watch(fd, path.c_str(), IN_MODIFY);
    if (wd < 0) {
        std::cerr << Color::RED << "Error: inotify_add_watch fallo"
                  << Color::RESET << "\n";
        close(fd);
        return;
    }

    std::cout << Color::GREEN << "[watch] Monitoreando: "
              << path << Color::RESET << "\n";
    std::cout << Color::CYAN
              << "[watch] Esperando nuevas lineas... (Ctrl+C para salir)\n"
              << Color::RESET;

    char buffer[4096];

    while (!g_interrupted) {
        int length = read(fd, buffer, sizeof(buffer));

        if (length > 0) {
            int i = 0;
            while (i < length) {
                struct inotify_event* event = (struct inotify_event*)&buffer[i];
                if (event->mask & IN_MODIFY) {
                    std::string line;
                    while (std::getline(file, line)) {
                        if (!line.empty()) {
                            callback(line);
                        }
                    }
                }
                i += sizeof(struct inotify_event) + event->len;
            }
        }

        usleep(100000);
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}
