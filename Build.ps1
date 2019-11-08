# Untitled 'Christian-Gunderman' project
# (C) 2019 Christian Gunderman

# Build renderer.
Remove-Item -Recurse .\out -ErrorAction SilentlyContinue

cmake -DCMAKE_TOOLCHAIN_FILE=".\vcpkg\scripts\buildsystems\vcpkg.cmake" -G "Visual Studio 16 2019" -A Win32 -S . -B out\x86
cmake --build out\x86

cmake -DCMAKE_TOOLCHAIN_FILE=".\vcpkg\scripts\buildsystems\vcpkg.cmake" -G "Visual Studio 16 2019" -A x64 -S . -B out\x64
cmake --build out\x64
