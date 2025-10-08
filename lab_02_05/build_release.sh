#!/bin/bash

gcc_build_args=("-fdiagnostics-color=always" "-std=c99" "-Wall" "-Werror" "-Wpedantic" "-Wextra" "-Wfloat-equal" "-Wfloat-conversion" "-Wvla" "-lm")
gcc "${gcc_build_args[@]}" -O2 -o app.exe ./*.c -lm

exit 0
