cd ../doom/doomgeneric/doomgeneric
make
mv doom ../../../realOS/doom
cd ../../../realOS
mcopy -i disc.img doom ::/TEST.ELF
