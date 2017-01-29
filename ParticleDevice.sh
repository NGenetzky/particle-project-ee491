#!/usr/bin/env bash

# Configuration

device=${1?"Specify the device as first arg."}

outfile="$device"".yaml";
variables=( stdin stdout data help );
registers=( 0 1 2 3 );

# Functions

write(){
    echo $1 >> $outfile
}

ini_section(){
    write "[$1]"
}

ini_value(){
    write "$1=$2"
}

# Main
echo "; Created by ParticleDevice.sh" > $outfile

ini_section "Variables"
for var in ${variables[@]}; do
    rv=$(particle get $device $var)
    if [ $? -eq 0 ]; then
        IFS='%'
        ini_value $var "$rv"
        unset IFS
    fi
done

ini_section "RegisterBank"
for i in ${registers[@]}; do
    rv=$(particle call $device reg $i)
    if [ $? -eq 0 ]; then
        ini_value $i "$rv"
    else
        break;
    fi
done


