local default_deps_base='libsystemd-dev qt5-default qtdeclarative5-dev qml-module-qtquick-controls qml-module-qtquick-controls2 qml-module-qtquick-dialogs qml-module-qt-labs-platform qml-module-qtcharts libqt5charts5-dev liblokimq-dev';
local default_deps_nocxx=default_deps_base;
local default_deps='g++ ' + default_deps_nocxx; // g++ sometimes needs replacement
local default_windows_deps='mingw-w64-binutils mingw-w64-gcc mingw-w64-crt mingw-w64-headers mingw-w64-winpthreads perl openssh zip bash binutils'; // deps for windows cross compile



local submodules = {
    name: 'submodules',
    image: 'drone/git',
    commands: ['git fetch --tags', 'git submodule update --init --recursive']
};


// Regular build on a debian-like system:
local debian_pipeline(name, image,
        arch='amd64',
        deps=default_deps,
        build_type='Release',
        werror=true,
        cmake_extra='',
        extra_cmds=[],
        imaginary_repo=true,
        allow_fail=false) = {
    kind: 'pipeline',
    type: 'docker',
    name: name,
    platform: { arch: arch },
    trigger: { branch: { exclude: ['debian/*', 'ubuntu/*'] } },
    steps: [
        submodules,
        {
            name: 'build',
            image: image,
            [if allow_fail then "failure"]: "ignore",
            environment: { SSH_KEY: { from_secret: "SSH_KEY" } },
            commands: [
                'echo "man-db man-db/auto-update boolean false" | debconf-set-selections',
                'apt-get update',
                'apt-get install -y eatmydata',
                'eatmydata apt-get dist-upgrade -y',
                ] + (if imaginary_repo then [
                    'eatmydata apt-get install -y gpg curl lsb-release',
                    'echo deb https://deb.imaginary.stream $$(lsb_release -sc) main >/etc/apt/sources.list.d/imaginary.stream.list',
                    'curl -s https://deb.imaginary.stream/public.gpg | apt-key add -',
                    'eatmydata apt-get update'
                    ] else []
                ) + [
                'eatmydata apt-get install -y gdb cmake git ninja-build pkg-config ccache ' + deps,
                'mkdir build',
                'cd build',
                'cmake .. -G Ninja -DCMAKE_CXX_FLAGS=-fdiagnostics-color=always -DSYSTEMD=ON -DCMAKE_BUILD_TYPE='+build_type+' ' +
                    (if werror then '-DWARNINGS_AS_ERRORS=ON ' else '') +
                cmake_extra,
                'ninja -v',
            ] + extra_cmds,
        }
    ],
};

// windows cross compile on alpine linux
local windows_cross_pipeline(name, image,
        arch='amd64',
        deps=default_windows_deps,
        build_type='Release',
        lto=false,
        werror=false,
        cmake_extra='',
        toolchain='32',
        extra_cmds=[],
        allow_fail=false) = {
    kind: 'pipeline',
    type: 'docker',
    name: name,
    platform: { arch: arch },
    trigger: { branch: { exclude: ['debian/*', 'ubuntu/*'] } },
    steps: [
        submodules,
        {
            name: 'build',
            image: image,
            [if allow_fail then "failure"]: "ignore",
            environment: { SSH_KEY: { from_secret: "SSH_KEY" }, WINDOWS_BUILD_NAME: toolchain+"bit" },
            commands: [
                'apk update && apk upgrade',
                'apk add cmake git ninja pkgconf ccache patch make g++ curl linux-headers python2 ' + deps,
                'git clone https://github.com/openssl/openssl.git -b OpenSSL_1_1_1g',
                'git clone https://code.qt.io/qt/qt5.git -b 5.12',
                'git clone https://github.com/jedisct1/libsodium.git -b 1.0.18-RELEASE',
                'git clone https://github.com/despair86/lokinet-gui-deployment-kit.git deploy',
                'cd openssl',
                'RC=i686-w64-mingw32-windres RANLIB=i686-w64-mingw32-ranlib CC=\'ccache i686-w64-mingw32-gcc\' LD=i686-w64-mingw32-ld ./Configure no-shared no-async mingw --prefix=/drone/src/ssleay-win32',
                'make -j16; make install',
                'cd /drone/src', // go back to build folder!
                'mkdir qt5-build',
                'cd qt5; perl init-repository --module-subset=default,-qtwebengine,-qtactiveqt',
                'cd /drone/src/qt5-build',
                '/drone/src/qt5/configure -prefix /drone/src/qt5-win32 -qt-zlib -qt-libjpeg -qt-libpng -qt-freetype -qt-pcre -qt-harfbuzz -opensource -confirm-license -platform linux-g++ -xplatform win32-g++ -release -device-option \'CROSS_COMPILE=ccache i686-w64-mingw32-\' -opengl desktop -no-angle -skip qtwebengine -optimize-size -strip -ssl -openssl-linked -qt-sqlite -sql-sqlite -qt-assimp -qt-tiff -qt-webp -recheck-all OPENSSL_PREFIX=/drone/src/ssleay-win32 \'OPENSSL_LIBS_DEBUG=-lssl -lcrypto\' \'OPENSSL_LIBS_RELEASE=-lssl -lcrypto\' \'OPENSSL_LIBS=-lws2_32 -lgdi32 -ladvapi32 -lcrypt32 -luser32\' OPENSSL_INCDIR=/drone/src/ssleay-win32/include OPENSSL_LIBDIR=/drone/src/ssleay-win32/lib -qt3d-simd sse2 -ccache -skip qtactiveqt -nomake examples -nomake tests',
                'make -j16; make install',
                'cd /drone/src/libsodium',
                'CC=\'ccache i686-w64-mingw32-gcc\' ./configure --host=i686-w64-mingw32 --target=i686-w64-mingw32 --disable-shared --prefix=/drone/src/sodium-win32',
                'make -j16; make install',
                'cd /drone/src',
                'mkdir build',
                'cd build',
                'PKG_CONFIG_PATH=/drone/src/sodium-win32/lib/pkgconfig cmake .. -G Ninja -DCMAKE_CROSSCOMPILE=ON -DCMAKE_EXE_LINKER_FLAGS=-fstack-protector -DCMAKE_TOOLCHAIN_FILE=$PWD/../mingw32.cmake -DCMAKE_BUILD_TYPE=Release -DQt5_DIR=/drone/src/qt5-win32/lib/cmake/Qt5 -DQt5Qml_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Qml -DQt5Network_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Network -DQt5Core_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Core -DQt5Quick_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Quick -DQt5Gui_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Gui -DQt5Widgets_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Widgets -DQt5Charts_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Charts -DBUILD_SHARED_LIBS=OFF'
                //'ninja -v',
            ]// + extra_cmds,
        }
    ],
};

// Builds a snapshot .deb on a debian-like system by merging into the debian/* or ubuntu/* branch
local deb_builder(image, distro, distro_branch, arch='amd64', imaginary_repo=true) = {
    kind: 'pipeline',
    type: 'docker',
    name: 'DEB (' + distro + (if arch == 'amd64' then '' else '/' + arch) + ')',
    platform: { arch: arch },
    environment: { distro_branch: distro_branch, distro: distro },
    steps: [
        submodules,
        {
            name: 'build',
            image: image,
            failure: 'ignore',
            environment: { SSH_KEY: { from_secret: "SSH_KEY" } },
            commands: [
                'echo "man-db man-db/auto-update boolean false" | debconf-set-selections',
                'apt-get update',
                'apt-get install -y eatmydata',
                'eatmydata apt-get install -y git devscripts equivs ccache git-buildpackage python3-dev' + (if imaginary_repo then ' gpg' else'')
                ] + (if imaginary_repo then [ // Some distros need the imaginary.stream repo for backported sodium, etc.
                    'echo deb https://deb.imaginary.stream $${distro} main >/etc/apt/sources.list.d/imaginary.stream.list',
                    'curl -s https://deb.imaginary.stream/public.gpg | apt-key add -',
                    'eatmydata apt-get update'
                ] else []) + [
                |||
                    # Look for the debian branch in this repo first, try upstream if that fails.
                    if ! git checkout $${distro_branch}; then
                        git remote add --fetch upstream https://github.com/loki-project/loki-network-control-panel.git &&
                        git checkout $${distro_branch}
                    fi
                |||,
                'git merge ${DRONE_COMMIT}',
                'export DEBEMAIL="${DRONE_COMMIT_AUTHOR_EMAIL}" DEBFULLNAME="${DRONE_COMMIT_AUTHOR_NAME}"',
                'gbp dch -S -s "HEAD^" --spawn-editor=never -U low',
                'eatmydata mk-build-deps --install --remove --tool "apt-get -o Debug::pkgProblemResolver=yes --no-install-recommends -y"',
                'export DEB_BUILD_OPTIONS="parallel=$$(nproc)"',
                'grep -q lib debian/lokinet-qt.install || echo "/usr/lib/lib*.so*" >>debian/lokinet-qt.install',
                'debuild -e CCACHE_DIR -b',
                'pwd',
                'find ./contrib/ci',
                './contrib/ci/drone-debs-upload.sh ' + distro,
            ]
        }
    ]
};

[
    // Various debian builds
    debian_pipeline("Debian sid (amd64)", "debian:sid"),
    debian_pipeline("Debian sid/Debug (amd64)", "debian:sid", build_type='Debug'),
    debian_pipeline("Debian sid/clang-10 (amd64)", "debian:sid", deps='clang-10 '+default_deps_nocxx,
                    cmake_extra='-DCMAKE_C_COMPILER=clang-10 -DCMAKE_CXX_COMPILER=clang++-10'),
    debian_pipeline("Debian sid/gcc-10 (amd64)", "debian:sid", deps='g++-10 '+default_deps_nocxx,
                    cmake_extra='-DCMAKE_C_COMPILER=gcc-10 -DCMAKE_CXX_COMPILER=g++-10'),
    debian_pipeline("Debian buster (i386)", "i386/debian:buster"),
    debian_pipeline("Ubuntu focal (amd64)", "ubuntu:focal"),

    // ARM builds (ARM64 and armhf)
    debian_pipeline("Ubuntu bionic (ARM64)", "ubuntu:bionic", arch="arm64", deps='g++-8 ' + default_deps_nocxx,
                    cmake_extra='-DCMAKE_C_COMPILER=gcc-8 -DCMAKE_CXX_COMPILER=g++-8'),
    debian_pipeline("Debian buster (armhf)", "arm32v7/debian:buster", arch="arm64"),
    
     windows_cross_pipeline("win32 on alpine (i386)", "i386/alpine:edge",
        toolchain='32', extra_cmds=[
          '../contrib/ci/drone-static-upload.sh'
    ]),

    // Deb builds:
    deb_builder("debian:sid", "sid", "debian/sid"),
    deb_builder("debian:buster", "buster", "debian/buster"),
    deb_builder("ubuntu:focal", "focal", "ubuntu/focal"),
    deb_builder("debian:sid", "sid", "debian/sid", arch='arm64'),

    // Macos builds:
    //mac_builder('macOS (Release)'),
    //mac_builder('macOS (Debug)', build_type='Debug'),
    //mac_builder('macOS (Static)',
//                extra_cmds=[
//                    '../contrib/ci/drone-check-static-libs.sh',
//                    '../contrib/ci/drone-static-upload.sh'
//                ]),
]
