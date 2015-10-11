#!/usr/bin/env bash

projectDir="$1"
environment="$2"
coresCount="$3"
buildType="Debug"

if [ "$environment" = "prod" ]; then
    buildType="Release"
fi

apt-get update \
&& sudo apt-get -y install build-essential clang-3.5 libc++1 libc++-dev gdb

sudo ln -sf /usr/bin/clang-3.5 /usr/bin/clang \
&& sudo ln -sf /usr/bin/clang++-3.5 /usr/bin/clang++

cd /tmp \
&& rm -rf cmake-3.3.2.tar.gz cmake-3.3.2 \
&& wget http://cmake.org/files/v3.3/cmake-3.3.2.tar.gz \
&& tar xf cmake-3.3.2.tar.gz \
&& cd cmake-3.3.2 \
&& ./bootstrap \
&& make "-j${coresCount}" \
&& make install

cd "${projectDir}/thirdparty/boost" \
&& rm -rf build bin.v2 \
&& ./bootstrap.sh --prefix=build --with-toolset=clang --with-libraries=system,program_options,filesystem \
&& ./b2 headers \
&& ./b2 --prefix=build "-j${coresCount}" --layout=tagged toolset=clang cxxflags="-fPIC -std=c++11 -stdlib=libc++" linkflags="-fPIC -stdlib=libc++" threading=multi,single link=shared,static install

cd "${projectDir}/thirdparty/googletest/googlemock" \
&& rm -rf build \
&& mkdir build \
&& cd build \
&& cmake -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_CXX_FLAGS="-std=c++11 -stdlib=libc++" .. \
&& make "-j${coresCount}"

cd "${projectDir}/build" \
&& rm -rf * \
&& cmake "-DCMAKE_BUILD_TYPE=${buildType}" .. \
&& make "-j${coresCount}" \
&& ./realtalktests \
&& sudo make install \
&& sudo ldconfig /usr/local/lib \
&& sudo make post_install
