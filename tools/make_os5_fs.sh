#!/bin/bash

block_size=4096

# make control block
echo $# | awk '{printf "%c", $1}'
dd if=/dev/zero count=$((block_size - 1)) bs=1

# make data block(s)
while [ -n "$1" ]; do
	# make header
	name=$(basename $1)
	echo -n $name
	dd if=/dev/zero count=$((32 - ${#name})) bs=1
	header_size=48
	data_size=$(stat -c '%s' $1)
	block_num=$((((header_size + data_size) / block_size) + 1))
	echo $block_num | awk '{printf "%c", $1}'
	dd if=/dev/zero count=15 bs=1

	# make data
	dd if=$1
	data_size_1st_block=$((block_size - header_size))
	if [ $data_size -le $data_size_1st_block ]; then
		dd if=/dev/zero count=$((data_size_1st_block - data_size)) bs=1
	else
		dd if=/dev/zero count=$((block_size - ((header_size + data_size) % block_size))) bs=1
	fi

	shift
done
