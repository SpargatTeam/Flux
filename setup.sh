#!/bin/bash

# Instalare dependențe pentru Linux și Windows
sudo apt-get update
sudo apt-get install -y g++ libc6-dev libx11-dev build-essential mingw-w64 git clang o64-clang++

# Instalare librării necesare
sudo apt-get install -y libcurl4-openssl-dev libfltk1.3-dev

# Dependențe pentru Android
wget https://dl.google.com/android/repository/android-ndk-r27c-linux.zip
unzip android-ndk-r27c-linux.zip
export PATH=$PATH:$(pwd)/android-ndk-r27c/toolchains/llvm/prebuilt/linux-x86_64/bin
sudo apt-get install -y openjdk-21-jdk kotlin

# Dependențe pentru macOS
git clone https://github.com/tpoechtrager/osxcross.git
mkdir -p osxcross/tarballs
cp $(pwd)/libs/MacOSX10.11.sdk.tar.xz osxcross/tarballs/
sudo bash osxcross/tools/get_dependencies.sh
cd osxcross
SDK_VERSION=10.11 ./build.sh
cd ..
export PATH=$PATH:$(pwd)/osxcross/target/bin

# Dependențe pentru Android (adăugare repetată pentru siguranță)
export PATH=$PATH:$(pwd)/android-ndk-r27c/toolchains/llvm/prebuilt/linux-x86_64/bin

echo "Toate dependențele au fost instalate și configurate cu succes!"
