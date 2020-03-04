# Building with mingw

Using QtCreator, import this project via its `.pro` file and enable a mingw build kit.

## SSL DLL files required at runtime

After building, it will be necessary to include some `.dll` files that will be needed during runtime. Copy these into the build directory alongside the compiled `.exe` file.

```
C:\Qt\Tools\mingw530_32\opt\bin\libeay32.dll
C:\Qt\Tools\mingw530_32\opt\bin\ssleay32.dll
```

## Distribution

Creating a distribution archive from scratch involves invoking `windeployqt.exe`.

First, find this file. On my system, it's located at `C:\Qt\5.9.9\mingw53_32\bin\windeployqt.exe`. The preceding file path should match the Qt version and mingw version used to compile (e.g. the 'kit' mentioned above).

Invoke this with the arg `--qmldir` pointed to the `res/qml` directory of our source tree. The final argument should be a compiled `.exe` file or a directory containing such a file. It's best that this be an empty directory containing only this file, so that our eventual archive (e.g. `.zip` file) wraps a single directory.

```
C:\Qt\5.9.9\mingw53_32\bin\windeployqt.exe --qmldir ..\..\loki-network-control-panel\res\qml .
```

`windeployqt` should fill out the directory with a bunch of dependencies, but it will likely be missing some. Based on a diff, these are:

```
libgcc_s_dw2-1.dll
libstdc++-6.dll
libwinpthread-1.dll
```

These should be located alongside the `windeployqt.exe` from above.

Copy these files into the distribution directory, as well as the SSL DLL Files mentioned above. At this point, all required files should be present. Create an archive of the directory and we're done.
