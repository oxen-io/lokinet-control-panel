# Lokinet Control Panel

This repository contains a cross-platform GUI for controlling and observing stats from a locally-running `lokinet`. See also [loki-network](https://github.com/loki-project/loki-network).

### Building

To build, you'll need a handful of `Qt` packages available. On `Ubuntu` or similar, you can install these with:

```bash
sudo apt install qt5-default qtdeclarative5-dev qml-module-qtquick-controls qml-module-qtquick-dialogs qml-module-qt-labs-platform
```

Then build and run:

```bash
qmake .
make
./lokicp
```
