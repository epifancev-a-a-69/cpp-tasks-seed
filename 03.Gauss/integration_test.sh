#!/bin/bash

status=true
TEST_DIR="./test_systems"
mkdir -p "$TEST_DIR"

SEED=12345
SIZES="2 3 5 10 20"

for size in $SIZES; do
    ./generate_system "$size" "$SEED" "$TEST_DIR/sys_${size}.csv"

    if ! ./gauss "$TEST_DIR/sys_${size}.csv" "$TEST_DIR/sol_${size}.csv"; then
        echo "Size $size: FAILED (solver error)"
        status=false
        continue
    fi

    if paste "$TEST_DIR/sys_${size}.csv.solution" \
             "$TEST_DIR/sol_${size}.csv" |
       awk '
       {
           d = $1 - $2
           if (d < 0) d = -d
           if (d > 1e-8) {
               exit 1
           }
       }
       END {
           exit 0
       }'
    then
        echo "Size $size: OK"
    else
        echo "Size $size: FAILED"

        echo "Expected:"
        cat "$TEST_DIR/sys_${size}.csv.solution"

        echo "Actual:"
        cat "$TEST_DIR/sol_${size}.csv"

        status=false
    fi
done

if $status; then
    exit 0
else
    exit 1
fi