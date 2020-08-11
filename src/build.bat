@echo off
IF NOT EXIST ..\run mkdir ..\run
pushd ..\run

set compiler= -Od -nologo -Zi -DBUILD_DEBUG
set linker= /link user32.lib gdi32.lib winmm.lib opengl32.lib
cl %compiler% ..\src\main.c %linker%

popd
