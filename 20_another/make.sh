#!/bin/sh

# создать диск для vbox
# VBoxManage convertfromraw --format VDI disk.img disk.vdi

# Для kernel.c
if (nasm -felf32 -o startup.o startup.asm)
then

# Компиляция ядра -msse -msse2
if (clang -Os -ffreestanding -m32 -march=i386 -mno-sse -c -o kernel.o kernel.c)
then

# Выгрузка бинарного файла :: код располагается в $100000, данные в $0x200000
if (ld -m elf_i386 -nostdlib -nodefaultlibs --oformat binary -Ttext=0x100000 -Tdata=0x200000 startup.o kernel.o -o kernel.c.bin)
then

# Собрать Loader -- главный загрузчик
if (fasm loader.asm >> /dev/null) 
then

# Выгрузка на диск
if (mv loader.bin disk/coreboot.bin)
then

    # Повтор для floppy-диска
    cp disk/coreboot.bin floppy/coreboot.bin

    rm *.o
    rm kernel.c.bin    
    bochs -f c.bxrc -q >> /dev/null 2>&1

fi
fi
fi
fi
fi

