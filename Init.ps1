# Untitled 'Christian-Gunderman' project
# (C) 2019 Christian Gunderman

# Initialize vcpkg.
git submodule init
git submodule update
Push-Location .\vcpkg
.\bootstrap-vcpkg.bat

# Install required vcpkgs.
# TODO: install for the specified build configuration.
.\vcpkg.exe install freeglut --triplet x86-windows
.\vcpkg.exe install freeglut --triplet x64-windows
Pop-Location