#!/bin/bash

if [ $# == 0 ]; then
    if [ -f "./build/main" ]; then
        ./build/main
        exit 0
    else
        echo "Error: build/ does not contain a file named: 'main'"
        exit 1
    fi
fi

if ! [ -f "./build/$1" ]; then
    echo "Error: build/ does not contain a file named: '$1'"
    exit 1
fi

"./build/$1"
exit 0