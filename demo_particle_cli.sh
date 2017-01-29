#!/usr/bin/env bash

set -x

device=Logan
particle list $device

particle call $device "get" "" # [sw3, sw2, sw1, led3, led2, led1, board_led]
particle call $device "digitalread" "D4" # sw1

particle call $device "set" "97" # [sw3, sw2, sw1, led3, led2, led1, board_led]
particle call $device "digitalwrite" "D0=LOW" # board_led

particle call $device "reg" "2" # POT1=A0
particle call $device "analogread" "A0" # POT1=A0



