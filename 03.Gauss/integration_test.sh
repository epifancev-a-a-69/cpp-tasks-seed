#!/bin/bash

status=true
TEST_DIR="./test_systems"
mkdir -p "$TEST_DIR"

SEED=12345
SIZES="2 3 5 10 20"

for size in $SIZES; do
    ./generate_system "$size" "$SEED" "$TEST_DIR/sys_${size}.csv"
    ./gauss "$TEST_DIR/sys_${size}.csv" "$TEST_DIR/sol_${size}.csv"
    
    # Сравниваем с эталоном (.solution файл от generate_system)
    if cmp -s "$TEST_DIR/sys_${size}.csv.solution" "$TEST_DIR/sol_${size}.csv"; then
        echo "✓ Size $size: OK"
    else
        echo "✗ Size $size: FAILED"
        status=false
    fi
done

$status