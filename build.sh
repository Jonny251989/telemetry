#!/bin/bash
set -e

if [ -f /.dockerenv ]; then
    PROJECT_ROOT="/workspace"
else
    SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    PROJECT_ROOT="$SCRIPT_DIR"
fi


BUILD_DIR="$PROJECT_ROOT/build"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"


cmake -DCMAKE_BUILD_TYPE=Release "$PROJECT_ROOT"
make -j$(nproc)


BIN_DIR="$PROJECT_ROOT/bin"
mkdir -p "$BIN_DIR"


cp "$BUILD_DIR/server/telemetry_server" "$BIN_DIR/"
cp "$BUILD_DIR/client/telemetry_client" "$BIN_DIR/"
cp "$BUILD_DIR/tests/unit_tests/unit_tests" "$BIN_DIR/"
cp "$BUILD_DIR/tests/functional_tests/functional_tests" "$BIN_DIR/"

TEST_DATA_DIR="$BIN_DIR/test_data"
mkdir -p "$TEST_DATA_DIR"
cp -r "$PROJECT_ROOT/tests/functional_tests/test_data/"* "$TEST_DATA_DIR/"
cp -r "$PROJECT_ROOT/tests/functional_tests/test_scripts/"* "$TEST_DATA_DIR/"

chmod -R +x "$BIN_DIR"/*

echo "Build completed successfully!"