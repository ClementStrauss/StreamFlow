![](https://github.com/ClementStrauss/StreamFlow/workflows/C/C++%20CI/badge.svg)

This is a toy project. 

# StreamFlow


A C++ framework to build multithreaded dataflow application. 

The framework help you create reusable software modules that can be linked together at runtime. 

Modules share data trought queue of various type. The way modules are assembled make the application. 

Every modules and IO are self documented by design


# Pre-build

pip install conan


sudo apt install libgtk2.0-dev libva-dev libvdpau-dev libx11-dev


sudo apt install libx11-dev  libx11-xcb-dev libfontenc-dev libice-dev libsm-dev libxau-dev libxaw7-dev libxcomposite-dev libxcursor-dev libxdamage-dev libxdmcp-dev libxext-dev libxfixes-dev libxi-dev libxinerama-dev libxkbfile-dev libxmu-dev libxmuu-dev libxpm-dev libxrandr-dev libxrender-dev libxres-dev libxss-dev libxt-dev libxtst-dev libxv-dev libxvmc-dev libxxf86vm-devxtrans-dev libxcb-render0-dev libxcb-render-util0-dev libxcb-xkb-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-xinerama0-dev xkb-data libxcb-dri3-dev uuid-dev libxcb-util-dev


conan install . --install-folder cmake-build-release --build=missing

cmake . -DCMAKE_TOOLCHAIN_FILE=cmake-build-release/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=debug 

# Build

cmake --build . 