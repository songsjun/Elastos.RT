export XDK_TARGET_PLATFORM=android
export XDK_COMPILER=clang

export XDK_TARGET_CPU=arm
if [[ ! "$_ELASTOS64" == "" ]]; then
    export XDK_TARGET_CPU_ARCH=64
    export CMAKE_ARGS="-DANDROID_ABI=arm64-v8a"
else
    export XDK_TARGET_CPU_ARCH=32
    export CMAKE_ARGS="-DANDROID_ABI=armeabi-v7a"
fi

export CMAKE_ARGS="$CMAKE_ARGS -DANDROID_TOOLCHAIN=$XDK_COMPILER"
export CMAKE_ARGS="$CMAKE_ARGS -DANDROID_PLATFORM=android-21"

export XDK_BUILD_TOOL=cmake
