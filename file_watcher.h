#pragma once
#include <string>
#include <functional>
#include <atomic>
/*
  FileWatcher:
  Monitorea un archivo en tiempo real usando inotify (llamada al sistema Linux).
  Cuando detecta que el archivo fue modificado, lee las líneas nuevas
  y las pasa a un callback para procesarlas.
*/
class FileWatcher {
public:
    /*
      watch(path, callback):
      - path: ruta del archivo a monitorear
      - callback: función que se llama con cada línea nueva detectada
      Bloquea el programa en un loop hasta que llegue SIGINT (Ctrl+C)
    */
    void watch(const std::string& path, std::function<void(const std::string&)> callback);
};
