#!/bin/sh

SDL2=/opt/sdl2/win
zipname=7dfps-win32-$(date +"%m%d%y-%H%M").zip

make CC=i686-w64-mingw32-gcc SDL2_PATH=$SDL2 OUT=7dfps.exe OPT=0 DBG=3

zip -r /tmp/$zipname 7dfps.exe
cd $SDL2/bin
zip /tmp/$zipname SDL2.dll
cd /usr/mw32/i686-w64-mingw32/lib/
zip /tmp/$zipname libgcc_s_sjlj-1.dll
