# ---- Etapa 1: Compilación ----
FROM ubuntu:22.04 AS builder

# Instalamos las herramientas necesarias para compilar
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make \
    && rm -rf /var/lib/apt/lists/*

# Carpeta de trabajo dentro del contenedor
WORKDIR /app

# Copiamos todo el código fuente
COPY . .

# Compilamos igual que lo hicimos a mano
RUN mkdir build && cd build && cmake .. && make

FROM ubuntu:22.04

WORKDIR /app

# Solo copiamos el ejecutable ya compilado
COPY --from=builder /app/build/fla .

# Carpeta donde irán los logs
RUN mkdir data

CMD ["./fla", "data/sample_auth.log"]

