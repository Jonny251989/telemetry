#!/bin/bash
set -e

if [ -f /.dockerenv ]; then
    # Внутри Docker
    PROJECT_ROOT="/workspace"
else
    # Вне Docker (на хосте)
    SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    PROJECT_ROOT="$SCRIPT_DIR"
fi

# Создаем директорию сборки
BUILD_DIR="$PROJECT_ROOT/build"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Конфигурируем и собираем проект
cmake -DCMAKE_BUILD_TYPE=Release "$PROJECT_ROOT"
make -j$(nproc)

# Создаем директорию для артефактов
BIN_DIR="$PROJECT_ROOT/bin"
mkdir -p "$BIN_DIR"

# Копирование артефактов
cp "$BUILD_DIR/server/telemetry_server" "$BIN_DIR/"
cp "$BUILD_DIR/client/telemetry_client" "$BIN_DIR/"
cp "$BUILD_DIR/tests/unit_tests/unit_tests" "$BIN_DIR/"
cp "$BUILD_DIR/tests/functional_tests/functional_tests" "$BIN_DIR/"

# Копирование тестовых данных
TEST_DATA_DIR="$BIN_DIR/test_data"
mkdir -p "$TEST_DATA_DIR"
cp -r "$PROJECT_ROOT/tests/functional_tests/test_data/"* "$TEST_DATA_DIR/"
cp -r "$PROJECT_ROOT/tests/functional_tests/test_scripts/"* "$TEST_DATA_DIR/"

# Устанавливаем права
chmod -R +x "$BIN_DIR"/*

echo "Build completed successfully!"