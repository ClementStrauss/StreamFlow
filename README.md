![](https://github.com/ClementStrauss/StreamFlow/workflows/C/C++%20CI/badge.svg)

This is a toy project. 

# StreamFlow


A C++ framework to build multithreaded dataflow application. 

The framework help you create reusable software modules and bring them together at runtime. 

Modules share data trought queue of various type. The way modules are assembled together makes the application. 

Every modules and IO are self documented by design. Documentation can be recalled programmatically


The application is "assembled" at runtime. Ultimately this could be done by reading a config file.

The application generate a visual representation of itself on startup, showing the nodes graph:

![](graph.png)


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
conan profile update settings.compiler.libcxx=libstdc++ default  
conan install .. --build missing  
cmake .. -DCMAKE_BUILD_TYPE=release  
cmake --build . -v  


# Build, Take2

conan is a mess with opencv, cannot make it to work  
1 . install opencv from source (mkdir build, cd build, cmake .., make -j8)  
treat this repository as cmake project

mkdir build  
cd build  
cmake-gui ..   
make  


