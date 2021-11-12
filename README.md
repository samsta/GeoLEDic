# GeoLEDic

[![macOS build + test](https://github.com/samsta/GeoLEDic/actions/workflows/macos.yml/badge.svg)](https://github.com/samsta/GeoLEDic/actions/workflows/macos.yml)
[![PlatformIO build + (native) test](https://github.com/samsta/GeoLEDic/actions/workflows/platformio.yml/badge.svg)](https://github.com/samsta/GeoLEDic/actions/workflows/platformio.yml)
[![Ubuntu build + test](https://github.com/samsta/GeoLEDic/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/samsta/GeoLEDic/actions/workflows/ubuntu.yml)


## Teensy 4.1

Assuming [PlatformIO](https://platformio.org/) is installed and you have some experience using it, and `pio` is on your path:

~~~
$ git clone --recurse-submodules https://github.com/samsta/GeoLEDic.git
$ cd GeoLEDic
$ pio run -e teensy41
~~~

Alternatively, if you use PlatformIO from within an IDE like Visual Studio Code, open the project by opening the GeoLEDic directory cloned above and then build as usual.

## Linux or macOS

![GeoLEDic Screenshot](/img/GeoLEDic.png)

~~~
$ sudo apt-get install cmake libglfw3-dev libglew-dev libglm-dev libasound2-dev
~~~

or on Mac ([homebrew](https://brew.sh) required)

~~~
$ brew install cmake glfw glm
~~~


And then:

~~~
$ git clone --recurse-submodules https://github.com/samsta/GeoLEDic.git
$ mkdir build
$ cd build
$ cmake ../GeoLEDic
$ make
~~~

or if you prefer to work in XCode on macOS,

~~~
$ git clone --recurse-submodules https://github.com/samsta/GeoLEDic.git
$ mkdir build
$ cd build
$ cmake ../GeoLEDic -G Xcode
$ open GeoLEDic.xcodeproj
~~~
