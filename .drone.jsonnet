local default_deps_base='libsystemd-dev qtbase5-dev qtdeclarative5-dev qml-module-qtquick-controls qml-module-qtquick-controls2 qml-module-qtquick-dialogs qml-module-qt-labs-platform qml-module-qtcharts libqt5charts5-dev liblokimq-dev';
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
                './contrib/build-win32-deps.sh',
                'cd /drone/src',
                'mkdir build',
                'cd build',
                'cmake .. -G Ninja -DCMAKE_CROSSCOMPILING=ON -DCMAKE_EXE_LINKER_FLAGS=-fstack-protector -DCMAKE_TOOLCHAIN_FILE=$PWD/../mingw32.cmake -DCMAKE_BUILD_TYPE=Release -DQt5_DIR=/drone/src/qt5-win32/lib/cmake/Qt5 -DQt5Qml_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Qml -DQt5Network_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Network -DQt5Core_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Core -DQt5Quick_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Quick -DQt5Gui_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Gui -DQt5Widgets_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Widgets -DQt5Charts_DIR=/drone/src/qt5-win32/lib/cmake/Qt5Charts -DQt5QmlModels_DIR=/drone/src/qt5-win32/lib/cmake/Qt5QmlModels -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_DEPS=ON',
                'ninja -v'
            ] + extra_cmds,
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
                # Ignore merge conflicts in .drone.jsonnet:
                'git config merge.ours.driver true',
                'echo .drone.jsonnet merge=ours >>.gitattributes',

                'git merge ${DRONE_COMMIT}',
                'export DEBEMAIL="${DRONE_COMMIT_AUTHOR_EMAIL}" DEBFULLNAME="${DRONE_COMMIT_AUTHOR_NAME}"',
                'gbp dch -S -s "HEAD^" --spawn-editor=never -U low',
                'eatmydata mk-build-deps --install --remove --tool "apt-get -o Debug::pkgProblemResolver=yes --no-install-recommends -y"',
                'export DEB_BUILD_OPTIONS="parallel=$$(nproc)"',
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
    
     windows_cross_pipeline("win32 on alpine (i386)", "i386/alpine:3.12",
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
