#!/usr/bin/env bash

apt-get update
sudo apt-get -y install build-essential checkinstall clang-3.5 libc++1 libc++-dev gdb

sudo ln -s /usr/bin/clang-3.5 /usr/bin/clang
sudo ln -s /usr/bin/clang++-3.5 /usr/bin/clang++

cd ~
mkdir downloads
cd downloads
wget http://cmake.org/files/v3.3/cmake-3.3.2.tar.gz
tar xf cmake-3.3.2.tar.gz
cd cmake-3.3.2
./bootstrap
make
sudo checkinstall -y

cd /vagrant/thirdparty/boost
./bootstrap.sh --prefix=build --with-toolset=clang --with-libraries=system,program_options,filesystem
./b2 headers
./b2 --prefix=build -j4 --layout=tagged toolset=clang cxxflags="-fPIC -std=c++11 -stdlib=libc++" linkflags="-fPIC -stdlib=libc++" threading=multi,single link=shared,static install

cd /vagrant/thirdparty/googletest/googlemock
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_CXX_FLAGS="-std=c++11 -stdlib=libc++" ..
make -j 4

cd /vagrant/build
cmake ..
make -j 4
./realtalktests
sudo make install
sudo ldconfig /usr/local/lib
sudo make post_install
