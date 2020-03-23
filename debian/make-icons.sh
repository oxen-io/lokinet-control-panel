#!/bin/bash

set -e

for x in 128 96 64 48 32 24 16; do
    mkdir -p build-debian/icons/${x}x${x}
    convert -verbose -filter Catrom -resize ${x}x${x} res/images/icon.png build-debian/icons/${x}x${x}/lokinet-gui.png
done
