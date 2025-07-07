#!/bin/bash
set -e

mkdir -p /workspace/build
cd /workspace/build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Копирование артефактов с абсолютными путями
cp /workspace/build/server/telemetry_server /workspace/bin/
cp /workspace/build/client/telemetry_client /workspace/bin/
cp /workspace/build/tests/unit_tests/unit_tests /workspace/bin/
cp /workspace/build/tests/functional_tests/functional_tests /workspace/bin/

# Копирование тестовых данных
mkdir -p /workspace/bin/test_data
cp -r /workspace/tests/functional_tests/test_data/* /workspace/bin/test_data/
cp -r /workspace/tests/functional_tests/test_scripts/* /workspace/bin/test_data/

# Устанавливаем права на ВСЕ файлы
chmod -R +x /workspace/bin/*

echo "Build completed successfully!"