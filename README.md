![](https://github.com/ClementStrauss/StreamFlow/workflows/C/C++%20CI/badge.svg)

This is a toy project. 

# StreamFlow


A C++ framework to build multithreaded dataflow application. 

The framework help you create reusable software modules that can be bring together at runtime. 

Modules share data trought queue of various type. The way modules are assembled make the application. 

Every modules and IO are self documented by design


The application is "build" at runtime. Ultimately this could be done by reading a config file.

The application generate a visual representation of the graph on startup, showing the node and connexions.


# Pre-build

pip install conan

sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev
sudo apt-get install libgtk-3-dev

sudo apt-get install libpng-dev
sudo apt-get install libjpeg-dev
sudo apt-get install libopenexr-dev
sudo apt-get install libtiff-dev
sudo apt-get install libwebp-dev

https://github.com/conan-io/conan-center-index/blob/master/recipes/opencv/4.x/conanfile.py
https://gitlab.com/CLIUtils/modern-cmake/tree/master/examples/extended-project

# Build


mkdir build && cd build
conan profile update settings.compiler.libcxx=libstdc++11 default
conan install .. --build missing
cmake .. -DCMAKE_BUILD_TYPE=release
cmake --build .. -v

