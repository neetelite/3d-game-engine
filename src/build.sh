#compiler="-Wall"
compiler="-g -DBUILD_DEBUG"
linker="-lm -lX11 -lGL"

gcc $compiler main.c -o ../run/gtd_linux_x64 $linker
