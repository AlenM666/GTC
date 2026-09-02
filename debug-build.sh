cmake -S . -B build/linux-debug \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_BUILD_TYPE=Debug \
    -DENABLE_ASAN=ON \
    -DENABLE_UBSAN=ON

cmake --build build/linux-debug -j$(nproc)

./build/linux-debug/bin/Debug/GTC
