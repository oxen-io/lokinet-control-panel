#!/bin/sh
# this script is invoked by ci and people who want to build
# their own qt5 gui for windows

# get our current srcdir
SRCDIR=$PWD

git clone https://github.com/openssl/openssl.git -b OpenSSL_1_1_1g
git clone https://code.qt.io/qt/qt5.git -b 5.12
git clone https://github.com/jedisct1/libsodium.git -b 1.0.18-RELEASE
git clone https://github.com/despair86/lokinet-gui-deployment-kit.git deploy
cd openssl
RC=i686-w64-mingw32-windres RANLIB=i686-w64-mingw32-ranlib CC='ccache i686-w64-mingw32-gcc' LD=i686-w64-mingw32-ld ./Configure no-shared no-async mingw --prefix=$SRCDIR/ssleay-win32
make -j16; make install
cd $SRCDIR
mkdir qt5-build
cd qt5; perl init-repository --module-subset=default,-qtwebengine,-qtactiveqt
cd $SRCDIR/qt5-build
$SRCDIR/qt5/configure -prefix $SRCDIR/qt5-win32 -qt-zlib -qt-libjpeg -qt-libpng -qt-freetype -qt-pcre -qt-harfbuzz -opensource -confirm-license -platform linux-g++ -xplatform win32-g++ -release -device-option 'CROSS_COMPILE=ccache i686-w64-mingw32-' -opengl desktop -no-angle -skip qtwebengine -optimize-size -strip -ssl -openssl-linked -qt-sqlite -sql-sqlite -qt-assimp -qt-tiff -qt-webp -recheck-all OPENSSL_PREFIX=$SRCDIR/ssleay-win32 'OPENSSL_LIBS_DEBUG=-lssl -lcrypto' 'OPENSSL_LIBS_RELEASE=-lssl -lcrypto' 'OPENSSL_LIBS=-lws2_32 -lgdi32 -ladvapi32 -lcrypt32 -luser32' OPENSSL_INCDIR=$SRCDIR/ssleay-win32/include OPENSSL_LIBDIR=$SRCDIR/ssleay-win32/lib -qt3d-simd sse2 -ccache -skip qtactiveqt -nomake examples -nomake tests
make -j16; make install
cd $SRCDIR/libsodium
CC='ccache i686-w64-mingw32-gcc' ./configure --host=i686-w64-mingw32 --target=i686-w64-mingw32 --disable-shared --prefix=$SRCDIR/sodium-win32
make -j16; make install
