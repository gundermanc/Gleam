# Untitled 'Christian-Gunderman' project
# (C) 2019 Christian Gunderman

# Important: must keep in sync with init.sh

# Install python, required for glad
brew install python

# Install GCC6, which is needed for vcpkg.
brew install gcc6

# # Initialize vcpkg.
git submodule init
git submodule update
pushd ./vcpkg
./bootstrap-vcpkg.sh

# # Install required vcpkgs.
# TODO: support linux.
./vcpkg install glad --triplet x64-osx
./vcpkg install glfw3 --triplet x64-osx
popd
