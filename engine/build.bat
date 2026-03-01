@ECHO OFF
SetLocal EnableDelayedExpansion
SET cFilenames=
FOR /R %%f in (*.c) do (
    SET cFilenames=!cFilenames! %%f
)
SET assembly=engine
SET compilerFlags=-g -shared -Wvarargs -Wall -Werror
SET includeFlags=-Isrc -I%VULKAN_SDK%\\Include
SET linkerFlags=-luser32 -lvulkan-1 -L%VULKAN_SDK%\\Lib
SET defines=-DKEXPORT -D_DEBUG -D_CRT_SECURE_NO_WARNINGS

ECHO "Building %assembly%.dll..."

ECHO "clang %cFilenames% %compilerFlags% -o ..\\bin\\%assembly%.dll %defines% %includeFlags% %linkerFlags%"
clang %cFilenames% %compilerFlags% -o ..\\bin\\%assembly%.dll %defines% %includeFlags% %linkerFlags%

ECHO "Build complete."