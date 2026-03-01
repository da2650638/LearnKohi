#!/bin/bash
set echo on

mkdir -p ../bin

# Get a list of all the .c files.
cFilenames=$(find . -type f -name "*.c")
echo "Files:$cFilenames"

assembly="engine"
compilerFlags="-g -shared -fPIC -fdeclspec" #QUESTION: -fdeclspec???
# -fms-extensions 
# -Wall -Werror
includePath="-Isrc -I$VULKAN_SDK/include"
linkerFlags="-L$VULKAN_SDK/lib -lvulkan -lxcb -lX11 -lX11-xcb -lxkbcommon" #-L/usr/X11R6/lib
defines="-DKEXPORT -D_DEBUG"

echo "Build $assembly..."
echo "clang $compilerFlags $cFilenames -o ../bin/lib$assembly.so $defines $includePath $linkerFlags"
clang $compilerFlags $cFilenames -o ../bin/lib$assembly.so $defines $includePath $linkerFlags 
echo "Build $assembly successfully."