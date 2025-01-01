# linux and windows dependencies
sudo apt-get install g++ libc6-dev libx11-dev build-essential mingw-w64 git clang o64-clang++ -y
# libs we use 
sudo apt install libcurl4-openssl-dev libfltk1.3-dev -y
# android dependencies
wget https://dl.google.com/android/repository/android-ndk-r27c-linux.zip
unzip android-ndk-r27c-linux.zip
export PATH=$PATH:$(pwd)/android-ndk-r27c/toolchains/llvm/prebuilt/linux-x86_64/bin
sudo apt-get install openjdk-21-jdk kotlin -y
# macos depencies
git clone https://github.com/tpoechtrager/osxcross.git
mkdir -p tarballs
cp $(pwd)/libs/MacOSX10.11.sdk.tar.xz $(pwd)/osxcross/tarballs/ 
sudo bash osxcross/tools/get_dependencies.sh
SDK_VERSION=10.11 ./osxcross/build.sh
export PATH=$PATH::$(pwd)/osxcross/target/binexport PATH=$PATH:$(pwd)/android-ndk-r27c/toolchains/llvm/prebuilt/linux-x86_64/bin