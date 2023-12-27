#!/bin/bash

win_build(){
    if [ ! -d build  ];then
    mkdir build
else
    echo build dir exist
fi
    cd build
    cmake ../geo -G "MinGW Makefiles"
    mingw32-make.exe
}

linux_build(){
    if [ ! -d build  ];then
    mkdir build
else
    echo build dir exist
fi
    cd build
    cmake ../geo
    make
}

uNames=`uname -s`
osName=${uNames: 0: 4}
if [ "$osName" == "Darw" ] # Darwin
then
    echo "Mac OS X"
elif [ "$osName" == "Linu" ] # Linux
then
    echo "GNU/Linux"
    linux_build
elif [ "$osName" == "MING" ] # MINGW, windows, git-bash
then
    echo "Windows, git-bash"
    win_build
else
    echo "unknown os"
fi





