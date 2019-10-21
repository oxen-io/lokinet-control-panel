# Lokinet Control Panel

![Screenshot](../media/images/lokinet_ui_screenshot.png?raw=true)

This repository contains a cross-platform GUI for controlling and observing stats from a locally-running `lokinet`. See also [loki-network](https://github.com/loki-project/loki-network).

### Building

To build, you'll need a handful of `Qt` packages available. On `Ubuntu` or similar, you can install these with:

```bash
sudo apt install qt5-default qtdeclarative5-dev qml-module-qtquick-controls qml-module-qtquick-controls2 qml-module-qtquick-dialogs qml-module-qt-labs-platform
```

Then build and run:

```bash
mkdir build
cd build
qmake ../lokicp.pro "CONFIG+=debug"
make
./lokicp
```

### Usage

The app starts running in the background with access available through the system tray. Click on the system tray icon to interact with the app.

### Status

This app is under heavy development. While the basic layout of the UI is established, the data is currently hard-coded.
