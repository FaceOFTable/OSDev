if (fasm core.asm)
then
    mv core.bin disk/
    bochs -f c.bxrc -q
fi
