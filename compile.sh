cd ./programs
i686-myos-gcc -c ktest.c -o ktest.o
i686-myos-ld ../sysroot/usr/lib ../sysroot/usr/include ktest.o -o ktest
cd ../
mcopy -i disc.img ./programs/ktest ::/ -o
