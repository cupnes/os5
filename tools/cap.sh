#!/bin/sh

work_dir=cap_$(date '+%Y%m%d%H%M%S')

mkdir ${work_dir}

qemu-system-i386 -fda fd.img &

sleep 1
echo "convert -loop 0 -delay 15 ${work_dir}/cap_*.gif anime.gif"

window_id=$(xwininfo -name QEMU | grep 'Window id' | cut -d' ' -f4)

i=0
while :; do
    import -window ${window_id} "${work_dir}/$(printf 'cap_%04d.gif' $i)"
    i=$((i + 1))
    sleep 0.1
done
