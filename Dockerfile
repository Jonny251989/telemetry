FROM ubuntu:22.04

# Установка зависимостей
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    cmake \
    libboost-system-dev \
    libgtest-dev \
    libgmock-dev \
    git \
    netcat \
    iputils-ping \
    xvfb \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

# Установка conan для управления зависимостями (опционально)
RUN pip3 install conan

# Создание рабочей директории
RUN mkdir -p /workspace
WORKDIR /workspace

# Копируем скрипт сборки
COPY build.sh /usr/local/bin/build.sh
RUN chmod +x /usr/local/bin/build.sh

# Установка gtest
RUN cd /usr/src/googletest && \
    cmake CMakeLists.txt && \
    make && \
    find . -name '*.a' -exec cp {} /usr/lib \;