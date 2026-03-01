#!/bin/bash
set echo on

mkdir -p ../bin

# Get a list of all the .c files.
cFilenames=$(find . -type f -name "*.c")
echo "Files:$cFilenames"

pushd ../
ORIGIN=$(pwd)/bin
popd
echo "rpath:$ORIGIN"
assembly="testbed"
compilerFlags="-g -fdeclspec -fPIC"
# -fms-extensions 
# -Wall -Werror
includePath="-Isrc -I../engine/src"
linkerFlags="-L../bin -lengine -Wl,-rpath,$ORIGIN" #QUESTION: -Wl,-rpath,. 是什么意思？
defines="-DKIMPORT -D_DEBUG"

echo "Build $assembly..."
clang $compilerFlags $cFilenames -o ../bin/$assembly $defines $includePath $linkerFlags 
echo "Build $assembly successfully."