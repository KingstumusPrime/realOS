cd ./programs
i686-myos-gcc -c -I../sysroot/include -L../sysroot/lib ktest.c -o ktest.o
i686-myos-ld -o ktest ktest.o ../sysroot/usr/lib/crt0.o  ../sysroot/usr/lib/crti.o  ../sysroot/usr/lib/crtn.o ../sysroot/usr/lib/libc.a
cd ../
mcopy -i disc.img ./programs/ktest ::/ -o
