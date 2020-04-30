#!/bin/bash

echo "==== Build started using half available threads (obtained by nproc) ==== "
echo "./premake5 gmake2 && make -j $(var1=$(nproc);var2=2;echo $(($var1/$var2)) ) &&./premake5 export-compile-commands"

/usr/bin/time sh -c "./premake5 gmake2 && make -j $(var1=$(nproc);var2=2;echo $(($var1/$var2)) ) &&./premake5 export-compile-commands && echo '==== That was tough! You better pay me next time! .-. ===='"
