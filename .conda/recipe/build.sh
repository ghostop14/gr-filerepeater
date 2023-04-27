#!/usr/bin/env bash

set -ex

# conda-forge uses newer libcpp than the available XCode would have you believe
# so pass _LIBCPP_DISABLE_AVAILABILITY to skip version checks and just try linking
if [[ $target_platform == osx* ]] ; then
    CXXFLAGS="${CXXFLAGS} -D_LIBCPP_DISABLE_AVAILABILITY"
fi

cmake -E make_directory buildconda
cd buildconda

cmake_config_args=(
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_INSTALL_PREFIX=$PREFIX
    -DLIB_SUFFIX=""
    -DENABLE_DOXYGEN=OFF
    -DENABLE_TESTING=ON
)

cmake ${CMAKE_ARGS} -G "Ninja" .. "${cmake_config_args[@]}"
cmake --build . --config Release -- -j${CPU_COUNT}
cmake --build . --config Release --target install

if [[ "${CONDA_BUILD_CROSS_COMPILATION:-}" != "1" || "${CROSSCOMPILING_EMULATOR}" != "" ]]; then
    ctest --build-config Release --output-on-failure --timeout 120 -j${CPU_COUNT}
fi
