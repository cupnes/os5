#!/bin/bash

# make control block
echo $# | awk '{printf "%c", $1}'
dd if=/dev/zero count=4095 bs=1

# make data block(s)
while [ -n "$1" ]; do
    # make header
    name=$(basename $1)
    echo -n $name
    dd if=/dev/zero count=$((32 - ${#name})) bs=1

    # make data
    dd if=$1
    dd if=/dev/zero count=$((4096 - 32 - $(stat -c '%s' $1))) bs=1

    shift
done
