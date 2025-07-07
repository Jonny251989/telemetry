#!/bin/bash

TEST_NAME=$1
export LC_ALL=C
INPUT_FILE="/test_data/${TEST_NAME}_input.txt"
OUTPUT_FILE="/test_data/${TEST_NAME}_output.txt"

/app/telemetry_client telemetry_server 12345 < "$INPUT_FILE" > "$OUTPUT_FILE" 2>&1
EXIT_CODE=$?

exit $EXIT_CODE