setlocal enableDelayedExpansion
set "TEMPVAR=0.01"

mkdir dt_"!TEMPVAR!"
cd dt_"!TEMPVAR!"
start ../../build/Debug/Exercise1.exe -method analytic -step "!TEMPVAR!"
start ../../build/Debug/Exercise1.exe -method euler -step "!TEMPVAR!"
start ../../build/Debug/Exercise1.exe -method symplectic_euler -step "!TEMPVAR!"
start ../../build/Debug/Exercise1.exe -method midpoint -step "!TEMPVAR!"
start ../../build/Debug/Exercise1.exe -method backwards_euler -step "!TEMPVAR!"
cd ..