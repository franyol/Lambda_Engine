#!/usr/bin/env bash
set -e  # stop on error

echo "== Installing dependencies =="

sudo apt update
sudo apt install -y \
  build-essential cmake git pkg-config \
  libasound2-dev libpulse-dev libaudio-dev libjack-dev \
  libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxss-dev \
  libgl1-mesa-dev libdbus-1-dev libudev-dev libwayland-dev libxkbcommon-dev \
  libdrm-dev libgbm-dev \
  libfreetype6-dev libharfbuzz-dev \
  libpng-dev libjpeg-dev libtiff-dev libwebp-dev \
  libvorbis-dev libogg-dev libflac-dev libmpg123-dev libopus-dev

echo "== Cleaning previous SDL2 installations =="

sudo rm -rf /usr/local/include/SDL2
sudo rm -rf /usr/local/lib/cmake/SDL2
sudo rm -f /usr/local/lib/libSDL2*
sudo rm -f /usr/local/lib/libSDL2_image*
sudo rm -f /usr/local/lib/libSDL2_ttf*
sudo rm -f /usr/local/lib/libSDL2_mixer*
sudo rm -f /usr/local/lib/pkgconfig/sdl2.pc
sudo rm -f /usr/local/lib/pkgconfig/SDL2_image.pc
sudo rm -f /usr/local/lib/pkgconfig/SDL2_ttf.pc
sudo rm -f /usr/local/lib/pkgconfig/SDL2_mixer.pc

sudo ldconfig

echo "== Installing SDL2 stack =="

build_and_install () {
    REPO_URL=$1
    DIR_NAME=$2

    git clone "$REPO_URL"
    cd "$DIR_NAME"

    git checkout SDL2

    mkdir build
    cd build

    cmake .. -DCMAKE_BUILD_TYPE=Release
    make -j$(nproc)
    sudo make install

    cd ../..
    rm -rf "$DIR_NAME"
}

# ===== SDL2 =====
build_and_install https://github.com/libsdl-org/SDL.git SDL

# ===== SDL2_image =====
build_and_install https://github.com/libsdl-org/SDL_image.git SDL_image

# ===== SDL2_ttf =====
build_and_install https://github.com/libsdl-org/SDL_ttf.git SDL_ttf

# ===== SDL2_mixer =====
build_and_install https://github.com/libsdl-org/SDL_mixer.git SDL_mixer

sudo ldconfig

echo "== Verifying installation =="

pkg-config --modversion sdl2
pkg-config --modversion SDL2_image
pkg-config --modversion SDL2_ttf
pkg-config --modversion SDL2_mixer

echo "== Done =="
