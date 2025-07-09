
Cборка проекта:

chmod +x build.sh
./build.sh

Запуск клиент-серверной системы:

./bin/telemetry_server <PORT> <CONFIG_PATH>
Пример: ./bin/telemetry_server 12345 ./server/limits.json

./bin/telemetry_client <HOST> <PORT>
Пример: ./bin/telemetry_client localhost 12345


Для проверки корректности работы модели можно выполнить функциональные тесты, используя для этого Docker Compose

для этого необходимо:

создать конфигурационный файл: 
echo "SOURCE_DIR=$(pwd)" > .env

собрать образ
docker compose build builder

собрать проект в контейнере              
docker compose run --rm builder

запустить тестовую инфраструктуры
docker compose --profile test up -d

остановить систему
docker compose --profile test down


для визуального подтверждения корректности работы системы была подключена CI/CD
результаты выподнения можно посомтреть в разделе action

