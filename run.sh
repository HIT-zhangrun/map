cd build

win_run(){
./geo_encode.exe
}

linux_run(){
./geo_encode
}

uNames=`uname -s`
osName=${uNames: 0: 4}
if [ "$osName" == "Darw" ] # Darwin
then
    echo "Mac OS X"
elif [ "$osName" == "Linu" ] # Linux
then
    echo "GNU/Linux"
    linux_run
elif [ "$osName" == "MING" ] # MINGW, windows, git-bash
then
    echo "Windows, git-bash"
    win_run
else
    echo "unknown os"
fi