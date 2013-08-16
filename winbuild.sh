#!/bin/sh

SDL2=~/win32-sdl2/
zipname=augshot-win32-$(date +"%m%d%y-%H%M").zip

make CC=i686-w64-mingw32-gcc SDL2_PATH=$SDL2 OUT=augshot.exe OPT=0 DBG=3

zip -r /tmp/$zipname augshot.exe res/*
cd $SDL2/bin
zip /tmp/$zipname SDL2.dll
cd /usr/mw32/i686-w64-mingw32/lib/
zip /tmp/$zipname libgcc_s_sjlj-1.dll
