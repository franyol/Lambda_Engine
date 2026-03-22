# Welcome to Lambda Engine

## Table of Contents
- [Linux Installation](#linux-installation)
- [Windows Installation](#windows-installation)
- [Linux installation of SDL](#linux-installation-of-sdl)
  - [SDL2 base APT Debian](#sdl2-base-apt-debian)
  - [Build dependencies](#build-dependencies)
  - [Remove previous installations](#remove-previous-installations)

---

I was too stubborn to use existing game engines, so I decided to build one myself using SDL2 as a base.

If this project eventually turns into a functional game (lmao) and you’re curious about how it works, you’re welcome to explore it. I’ll do my best to document everything clearly.

If not, use it at your own risk. The project is still evolving, and core features may change or break as development continues.

---

## Linux Installation

Clone the repo and build from the root:

```bash
cmake -S . -B build 
```

Then install it:

```bash
cd build
sudo make install
```

After this, you can build your executables using the library with the following flags:

```bash
-llambda_engine -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
```

---

## Windows Installation

I use MinGW to build the project on windows, but you can adapt it depending on where SDL is installed on your system.

From the project root, run:

Replace "C:/lambda_engine_dev_lib" with your desired installation directory.

```
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX="C:/lambda_engine_dev_lib"
```

Then go to the build directory to build and install:

In my case, I use mingw32-make:

```
mingw32-make install
```

---

## Linux installation of SDL

You can follow the detailed steps below, or simply run the script to install/update SDL2 and its dependencies:

```bash
./install_sdl_from_source.sh
```

This installs everything directly from the latest official SDL GitHub repositories. To uninstall, follow the manual steps below, since this setup is not managed by your package manager.

---

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

### Remove previous installations

If SDL2 was installed via apt:
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

Check installation:
```
pkg-config --modversion sdl2
pkg-config --modversion SDL2_image
pkg-config --modversion SDL2_ttf
pkg-config --modversion SDL2_mixer
```
