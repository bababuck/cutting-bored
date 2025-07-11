# cutting-bored
Create complex cutting board patterns via simple steps.

# Build

```bash
mkdir build && cd build
cmake -DLLVM_DIR=$PWD/../../llvm-project/build/lib/cmake/llvm -DMLIR_DIR=$PWD/../../llvm-project/build/lib/cmake/mlir ..
cmake --build . --target cutting-bored-opt
```