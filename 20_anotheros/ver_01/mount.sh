#!/bin/sh

# qemu-system-i386 -drive format=raw,file=disk.img

# Пример для запуска. Отсюда не запускать.
sudo mount disk.img   -t vfat -o loop,rw,uid="`whoami`",sync,offset=$[1048576] disk
sudo mount floppy.img -t vfat -o loop,rw,uid="`whoami`",sync,offset=$[0] floppy 
