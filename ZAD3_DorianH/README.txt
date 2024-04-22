***UWAGA*** Projekt wymaga konfiguracji SFML (Dodanie ścieżek do bibliotek)
W sekcji build/Release znajduje się skompilowana wersja programu (Sterowanie W,S,A,D)
Testowane na VS 2022 (PowerShell) z CMake 3.29.1 (Win (cmake-gui))

cmake -S . -B build
cmake --build build
cd build
ctest -C Debug