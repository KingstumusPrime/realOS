#!/bin/sh
set -e
. ./iso.sh
 
qemu-system-$(./target-triplet-to-arch.sh $HOST) -m 4G -cpu max -cdrom myos.iso -boot order=dc -drive file=disc.img,media=disk  -no-shutdown -no-reboot 