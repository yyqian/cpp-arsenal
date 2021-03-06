#!/usr/bin/env bash
BUILD_DIR=build

clear_build() {
    if [ -d "${BUILD_DIR}" ]; then
        rm -rf ${BUILD_DIR}
    fi
}

build() {
    cd ${BUILD_DIR}
    cmake -DCMAKE_BUILD_TYPE=Debug .. && make
    cd ..
}

rebuild() {
    clear_build
    mkdir ${BUILD_DIR}
    build
}

unit_test() {
    clear_build
    build
    cd ${BUILD_DIR}/bin
    ./arsenal_test
    cd ../../
}

case $1 in
   clean)
      shift
      clear_build
      ;;
   build)
      shift
      build
      ;;
   rebuild)
      shift
      rebuild
      ;;
   test)
      shift
      unit_test
      ;;
   exist)
      shift
      verify_exist
      ;;
esac
