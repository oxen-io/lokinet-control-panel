# Lokinet Control Panel

![Screenshot](../media/images/lokinet_ui_screenshot.png?raw=true)

This repository contains a cross-platform GUI for controlling and observing stats from a locally-running `lokinet`. See also [loki-network](https://github.com/loki-project/loki-network).

There is also an option (`Bootstrap Client from Web`) in the system tray's context menu (except when compiled with `SYSTEMD=true`, as is the case in the Debian packages). This will download a bootstrap file which will allow the `lokinet` client to join the lokinet network.

### Building

To build, you'll need a handful of `Qt` packages available. On `Ubuntu` or similar, you can install these with:

```bash
sudo apt install qt5-default qtdeclarative5-dev qml-module-qtquick-controls qml-module-qtquick-controls2 qml-module-qtquick-dialogs qml-module-qt-labs-platform qml-module-qtcharts libqt5charts5-dev
```

Then build and run:

```bash
mkdir build
cd build
qmake "CONFIG+=debug" ..
make
./lokinet-gui
```

### Systemd

The app can be compiled to support `systemd` for starting and stopping `lokinet`. To enable this, invoke `qmake` with `SYSTEMD=true`.

Note that interactions with systemd may require privilege escalation, which is handled through `polkit`. A graphical polkit backend must be running for this to work.

### Usage

By default, the app starts running in the background with access available through the system tray. Click on the system tray icon to interact with the app.

Additionally, the following command-line arguments are supported:

```
--no-hide

Prevents the window from being hidden automatically when it loses focus. Also adds normal window chroming.

--no-tray

Implies --no-hide, and additionally removes the system tray. Closing the window exits the app.
```

