#!/bin/bash

set -ex

rm -rf build
mkdir -p build

pushd build > /dev/null

../configure \
    CFLAGS="-Wall -g" \
    --disable-silent-rules

popd > /dev/null

make -C build "${TARGET}"
