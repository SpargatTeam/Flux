#!/bin/bash

TARGET="flux"
APPSRCDIR="src/app"
ANDROIDSRCDIR="src/android"
SOURCES_APP=$(find $APPSRCDIR -type f \( -name "*.cpp" -o -name "*.h" \) 2>/dev/null)
SOURCE_ANDROID=$(find $ANDROIDSRCDIR)
SOURCES_LIB=$(find $LIBSRCDIR -name '*.cpp' 2>/dev/null)

build_linux() {
    if [ -z "$SOURCES_APP" ]; then
        echo "No source files found in $APPSRCDIR"
        exit 1
    fi
    echo "Building for Linux..."
    g++ -Wall -std=c++11 $SOURCES_APP -o release/${TARGET}-linux \
        -lcurl -lfltk -lm
    echo "Build complete: release/${TARGET}-linux"
}

build_android() { # for later
    if [ -z "$SOURCE_ANDROID" ]; then
        echo "No source files found in $SOURCE_ANDROID"
        exit 1
    fi
    JAVA_SOURCE="$SOURCE_ANDROID/java"
    LIB_SOURCE="$SOURCE_ANDROID/libs"
    SOLIB_SOURCE="$SOURCE_ANDROID/cpp"
    BUILD_OUTPUT="$SOURCE_ANDROID/build"
    SMALI_OUTPUT="$BUILD_OUTPUT/smali"
    echo "Building for Android..."
    if [[ ! -d "$JAVA_SOURCE" ]]; then
        echo "Source directory $JAVA_SOURCE does not exist."
        exit 1
    fi
    if [[ ! -d "$LIB_SOURCE" ]]; then
        echo "Library directory $LIB_SOURCE does not exist."
        exit 1
    fi
    mkdir -p "$BUILD_OUTPUT/classes"
    mkdir -p "$SMALI_OUTPUT"
    echo "Compiling .kt and .java files..."
    for file in "$JAVA_SOURCE"/*.{kt,java}; do
        if [[ -f "$file" ]]; then
            extension="${file##*.}"
            if [[ $extension == "kt" ]]; then
                echo "Compiling Kotlin: $file"
                kotlinc "$file" -classpath "$LIB_SOURCE/*" -d "$BUILD_OUTPUT/classes/"
            elif [[ $extension == "java" ]]; then
                echo "Compiling Java: $file"
                javac -cp "$LIB_SOURCE/*" -d "$BUILD_OUTPUT/classes/" "$file"
            fi
        fi
    done
    if [[ ! -d "$BUILD_OUTPUT/classes" || -z "$(ls -A "$BUILD_OUTPUT/classes")" ]]; then
        echo "No .class files were generated. Check the source code."
        exit 1
    fi
    echo "Creating .jar file..."
    jar cvf "$BUILD_OUTPUT/output.jar" -C "$BUILD_OUTPUT/classes/" .
    echo "Converting .jar to .dex..."
    d8 "$BUILD_OUTPUT/output.jar" "$LIB_SOURCE"/*.jar --output "$BUILD_OUTPUT/dex/"
    echo "Disassembling .dex into .smali files..."
    java -jar baksmali.jar disassemble "$BUILD_OUTPUT/dex/classes.dex" -o "$SMALI_OUTPUT/"
    echo "Build complete. Smali files are in $SMALI_OUTPUT/"
    echo "Dex file is in $BUILD_OUTPUT/dex/"
    armv7a-linux-androideabi21-clang -Wall -shared -fPIC -std=c++11 -w $SOLIB_SOURCE -I./libs -march=armv7-a -mfloat-abi=softfp -mfpu=neon -ferror-limit=99999 -o release/lib${TARGET}.so || true
}

build_macos() {
    if [ -z "$SOURCES_APP" ]; then
        echo "No source files found in $APPSRCDIR"
        exit 1
    fi
    echo "Building for macOS..."
    o64-clang++ -Wall -std=c++11 $SOURCES_APP -o release/${TARGET}-macos \
        -framework Cocoa -lcurl -lfltk
    echo "Build complete: release/${TARGET}-macos"
}

build_windows() {
    if [ -z "$SOURCES_APP" ]; then
        echo "No source files found in $APPSRCDIR"
        exit 1
    fi
    echo "Building for Windows..."
    x86_64-w64-mingw32-g++ -Wall -std=c++11 $SOURCES_APP -o release/${TARGET}.exe \
        -lws2_32 -lcurl -lfltk
    echo "Build complete: release/${TARGET}.exe"
}

clean() {
    echo "Cleaning up..."
    rm -f release/${TARGET}-linux release/${TARGET}-macos release/${TARGET}.exe release/${TARGET}.so
    echo "Clean complete."
}

case "$1" in
    linux)
        build_linux
        ;;
    android)
        build_android
        ;;
    macos)
        build_macos
        ;;
    windows)
        build_windows
        ;;
    clean)
        clean
        ;;
    *)
        echo "Usage: $0 {linux|macos|windows|android|clean}"
        exit 1
        ;;
esac