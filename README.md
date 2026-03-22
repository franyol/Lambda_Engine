# Welcome to Lambda Engine

I am too stuborn to use regular game engines, so I built one myself using SDL2 as a base.

If it turns out that I actually made a functional game with this (lmao) and you are a corius being wanting to see how, you are welcome, I will try to document this as best as I can.

If I haven't done anything functional with this, use at your own risk, as I might probably still be making changes to the base functionalities that could break something since the project is still not sturdy enough.

## Linux Installation

```
cmake -S . -B build 
```

then install it

```
cd build
sudo make install
```

After this you can build your executables using the library by using the flag
```
-llambda_engine
```

## Windows Installation

We use MinGW for building the project, but you can change it depending on where you have SDL installed

Go to project root and run:

Change "C:/lambda_engine_dev_lib" with the installation directory you want it to be

```
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX="C:/lambda_engine_dev_lib"
```

Then go to the build directory to build and install it:

In my case I will use mingw32-make to install it

```
mingw32-make install
```

## Linux installation of SDL

### SDL2 base APT Debian

```
sudo apt-get install libsdl2-2.0-0 
sudo apt install libsdl2-dev
```


### Build dependencies:
```
sudo apt update
sudo apt install -y \
  build-essential cmake git \
  libasound2-dev libpulse-dev libaudio-dev libjack-dev \
  libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxss-dev \
  libgl1-mesa-dev libdbus-1-dev libudev-dev libwayland-dev libxkbcommon-dev \
  libdrm-dev libgbm-dev \
  libfreetype6-dev libharfbuzz-dev \
  libjpeg-dev libpng-dev libtiff-dev libwebp-dev \
  libvorbis-dev libogg-dev libflac-dev libmpg123-dev libopus-dev
```

### from scratch

you can run this script that builds everything from the github repositories:
```
./install_sdl_from_source.sh 
```

### Remove previous installations

SDL2 if you used apt:
```
sudo apt purge libsdl2-dev libsdl2-2.0-0
sudo apt autoremove
```

```
sudo rm -rf /usr/local/include/SDL2
sudo rm -f /usr/local/lib/libSDL2*
sudo rm -f /usr/local/lib/libSDL2_image*
sudo rm -f /usr/local/lib/libSDL2_ttf*
sudo rm -f /usr/local/lib/libSDL2_mixer*

sudo ldconfig
```

Install everything:
```
# ===== SDL2 =====
git clone https://github.com/libsdl-org/SDL.git
cd SDL
git checkout SDL2
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
cd ../..
rm -rf SDL

# ===== SDL2_image =====
git clone https://github.com/libsdl-org/SDL_image.git
cd SDL_image
git checkout SDL2
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
cd ../..
rm -rf SDL_image

# ===== SDL2_ttf =====
git clone https://github.com/libsdl-org/SDL_ttf.git
cd SDL_ttf
git checkout SDL2
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
cd ../..
rm -rf SDL_ttf

# ===== SDL2_mixer =====
git clone https://github.com/libsdl-org/SDL_mixer.git
cd SDL_mixer
git checkout SDL2
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
cd ../..
rm -rf SDL_mixer

# refresh linker cache
sudo ldconfig
```

Check they are installed
```
pkg-config --modversion sdl2
pkg-config --modversion SDL2_image
pkg-config --modversion SDL2_ttf
pkg-config --modversion SDL2_mixer
```


### SDL2


