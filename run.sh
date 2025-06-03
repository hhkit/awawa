#!/bin/sh
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build . -G Ninja && cmake --build build && build/bin/awawa