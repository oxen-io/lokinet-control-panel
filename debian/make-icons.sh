#!/bin/bash

set -e
set -x

for x in 256 192 128 96 64 48 32 24 16; do
    mkdir -p build-debian/icons/${x}x${x}
    convert -verbose -filter Catrom -resize ${x}x${x} res/images/icon.svg build-debian/icons/${x}x${x}/lokinet-gui.png
done

mkdir build-debian/icons/scalable
cp res/images/icon.svg build-debian/icons/scalable/lokinet-gui.svg
