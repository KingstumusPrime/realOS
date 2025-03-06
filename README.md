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

## Gallery
![doom1](https://github.com/user-attachments/assets/0b1c7f20-7999-4016-a980-743259910a05)
![doom](https://github.com/user-attachments/assets/4d164416-132b-4514-ac82-0c2b57e4f6f6)
![desktop](https://github.com/user-attachments/assets/475df027-c189-4e5f-b282-e83d955f6111)

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

In order to build this project you will first need the custom GCC toolchain found in the releases of this repo here [RealOS Custom toolchain](https://github.com/KingstumusPrime/RealOs-Custom-Toolchain/releases/tag/Release). After that you need to run:
```
export PATH="$HOME/path/to/realOS/toolchain/bin:$PATH"
```
Once you have ran this command you should be able to simply us the build and clean scripts included in this project. If you wish to write code to run on the Operating System you also need to use the custom toolchain in place of standard GCC.
