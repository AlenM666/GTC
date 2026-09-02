cmake -S . -B build/linux-release \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_LTO=ON

cmake --build build/linux-release -j$(nproc)
