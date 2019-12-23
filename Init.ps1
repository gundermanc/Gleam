# Untitled 'Christian-Gunderman' project
# (C) 2019 Christian Gunderman

# Important: must keep in sync with init.sh

# Initialize vcpkg.
git submodule init
git submodule update
Push-Location .\vcpkg
.\bootstrap-vcpkg.bat

# Install required vcpkgs.
.\vcpkg.exe install glad --triplet x86-windows
.\vcpkg.exe install glad --triplet x64-windows
.\vcpkg.exe install glfw3 --triplet x86-windows
.\vcpkg.exe install glfw3 --triplet x64-windows
.\vcpkg.exe install freetype-gl --triplet x86-windows
.\vcpkg.exe install freetype-gl --triplet x64-windows
.\vcpkg.exe install gtest --triplet x86-windows
.\vcpkg.exe install gtest --triplet x64-windows
Pop-Location
