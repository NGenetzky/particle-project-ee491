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

yaml_multiline0(){
    write "$1: |"
    write "$2"
    write "" # Blank line after multiline.
}

yaml_singleline0(){
    write "$1: $2"
}
yaml_singleline1(){
    write "    $1: $2"
}

# Main
echo "# Created by ParticleDevice.sh" > $outfile

write "# Variables"
for var in ${variables[@]}; do
    rv="$(particle get $device $var)"
    if [ $? -eq 0 ]; then
        IFS='%' # Preserve whitespace. until "unset IFS"
        yaml_multiline0 $var "$rv"
        unset IFS
    fi
done

yaml_singleline0 "RegisterBank" ""
for i in ${registers[@]}; do
    rv="$(particle call $device reg $i)"
    if [ $? -eq 0 ]; then
        yaml_singleline1 $i "$rv"
    else
        break;
    fi
done


