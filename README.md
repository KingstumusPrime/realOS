# RealOS

A POSIX compatable x86 operating system writtn in C and assembly

## Description

RealOS was created as hobby proejct too learn more about how computers work from boot and beyond. Aside from learning about low level concepts I also learned a lot more about proper debugging and optimization of code.

### Features

* Runs DOOM!
* Support for the FAT filesystem
* DMA & PIO disc drivers
* 1024x768 VGA graphics
* Custom system calls
* loading BMP and ELF files
* A POSIX compatable Lib C being developed hand-in-hand with the OS 
* Keyboard support
* Implentation of STDERR, STDOUT, and SDIN
* Linux Style File Handling

## Getting Started

### Dependencies

* Qemu (only have tested this running the linux version)

### Executing program

* run the ./qemu.sh command in the root of the project to boot the operating systems iso automatically
* To manually run the Operating System try:
```
qemu-system-i386 -m 4G -cpu max -cdrom ./path/to/the/OS.iso -boot order=dc -drive file=disc.img,media=disk  -no-shutdown -no-reboot
```

## Building

In order to build this project you will first need the custom GCC toolchain found in this here [RealOS Custom toolchain](https://twitter.com/dompizzie). After that you need to run:
```
export PATH="$HOME/path/to/realOS/toolchain/bin:$PATH"
```
Once you have ran this command you should be able to simply us the build and clean scripts included in this project. If you wish to write code to run on the Operating System you also need to use the custom toolchain in place of standard GCC.
