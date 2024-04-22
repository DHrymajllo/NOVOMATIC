Testowane na VS 2022 (PowerShell) z CMake 3.29.1 (Win (cmake-gui))

cmake -S . -B build
cmake --build build
cd build
ctest -C Debug