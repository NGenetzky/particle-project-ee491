#!/usr/bin/env bash
set -x
flash_device.bash Logan

sleep 20s
particle call Logan cin "\$(set 15)"
# particle get Logan stdin
particle get Logan stdout

sleep 2s
particle call Logan cin "\$(get)"
# particle get Logan stdin
particle get Logan stdout

