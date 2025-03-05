cd ./programs/desktop
make
cp top.elf ../../
cd ../../
mcopy -o -i disc.img top.elf ::/TOP.ELF
