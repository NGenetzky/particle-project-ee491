#!/usr/bin/env bash

flash_device() {
    flash_device.bash Logan
    sleep 10s
}

test_digitalport(){
        particle call Logan "get" ""

        particle call Logan "set" "15"

        particle call Logan "get" ""

        particle call Logan "set" "0"

        particle call Logan "get" ""
}

test_cin_stdout(){
    particle call Logan cin "\$(DW D7=HIGH)"
    particle get Logan stdout

    particle call Logan cin "\$(DR D7)"
    particle get Logan stdout

    particle call Logan cin "\$(DR D4)"
    particle get Logan stdout
}

# Main
set -x

flash_device
test_digitalport
test_cin_stdout
