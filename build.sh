# Untitled 'Christian-Gunderman' project
# (C) 2019 Christian Gunderman

# Build renderer.
rm -rf out

cmake -DCMAKE_TOOLCHAIN_FILE="./vcpkg/scripts/buildsystems/vcpkg.cmake" -B out
cmake --build out
