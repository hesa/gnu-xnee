#!/bin/sh

clean_up()
{
    make clean
}


make_makefile()
{
    export  CC=/home/hesa/opt/NokiaQtSDK/Maemo/4.6.2/targets/fremantle-qt-1014/bin/gcc

#/home/hesa/opt/NokiaQtSDK/Maemo/4.6.2/toolchains/arm-2007q3-51sb6-gdb701-arm-none-linux-gnueabi_linux/arm-none-linux-gnueabi/bin/gcc
    make -f Makefile.cvs && \
      ./configure --disable-gui --disable-doc --disable-gnome-applet --host=arm --disable-xinput2
}

#clean_up
make_makefile
