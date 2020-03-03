# Building with mingw

Using QtCreator, import this project via its `.pro` file and enable a mingw build kit.

## SSL DLL files required at runtime

After building, it will be necessary to include some `.dll` files that will be needed during runtime. Copy these into the build directory alongside the compiled `.exe` file.

```
C:\Qt\Tools\mingw530_32\opt\bin\libeay32.dll
C:\Qt\Tools\mingw530_32\opt\bin\ssleay32.dll
```

## Distribution

TODO
