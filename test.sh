#!/usr/bin/env bash

################################################################################
# Utility Functions
################################################################################
flash_device() {
    device=$1
    
    firmware_dir=./firmware
    if [ -d $firmware_dir ]; then
        cd $firmware_dir
    fi
    #  [ -d $firmware_dir ] || { echo "Can't find $firmware_dir"; exit 1; }
    
    particle flash $device ./
}

################################################################################
# Functions to test particular functionality
################################################################################
test_digitalport(){
        particle call $device "get" ""

        particle call $device "set" "15"

        particle call $device "get" ""

        particle call $device "set" "0"

        particle call $device "get" ""
}

test_cin_stdout(){
    particle call $device cin "\$(reg 0)"
    particle get $device stdout
    
    particle call $device cin "\$(DW D7=HIGH)"
    particle get $device stdout

    particle call $device cin "\$(DR D7)"
    particle get $device stdout

    particle call $device cin "\$(DR D4)"
    particle get $device stdout

    particle call $device cin "\$(set 15)"
    particle get $device stdout

    particle call $device cin "\$(get)"
    particle get $device stdout

    particle call $device cin "\$(get)"
    particle get $device stdout
    
    particle call $device cin "\$(reg 0)"
    particle get $device stdout
}

################################################################################
# Main
################################################################################

################################################################################
# Main.Setup
################################################################################
device=${1?"Specify the device as first arg."}

set -x
set -e

################################################################################
# Parse Arguments
################################################################################

flash_device $device
sleep 10s

# test_digitalport
test_cin_stdout
