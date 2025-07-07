#!/bin/bash
set -e

cd /build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Копирование артефактов с абсолютными путями
cp /build/server/telemetry_server /bin/
cp /build/client/telemetry_client /bin/
cp /build/tests/unit_tests/unit_tests /bin/
cp /build/tests/functional_tests/functional_tests /bin/

# Копирование тестовых данных
mkdir -p /bin/test_data
cp -r /tests/functional_tests/test_data/* /bin/test_data/
cp -r /tests/functional_tests/test_scripts/* /bin/test_data/

# Устанавливаем права на ВСЕ файлы
chmod -R +x /bin/*

echo "Build completed successfully!"