#!/bin/bash

gcc "weights_training.c" -o "build/main" -Isrc/header -lm -lpthread -fgnu89-inline
"./build/main"
exit 0

