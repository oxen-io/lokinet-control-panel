# Lokinet Control Panel

![Screenshot](../media/images/lokinet_ui_screenshot.png?raw=true)

This repository contains a cross-platform GUI for controlling and observing stats from a locally-running `lokinet`. See also [loki-network](https://github.com/oxen-io/loki-network).

There is also an option (`Bootstrap Client from Web`) in the system tray's context menu (except when compiled with `SYSTEMD=true`, as is the case in the Debian packages). This will download a bootstrap file which will allow the `lokinet` client to join the lokinet network.

### Building

To build, you'll need `cmake` and a handful of `Qt` packages available. On `Ubuntu` or similar, you can install these with:

```bash
sudo apt install qt5-default qtdeclarative5-dev qml-module-qtquick-controls qml-module-qtquick-controls2 qml-module-qtquick-dialogs qml-module-qt-labs-platform qml-module-qtcharts libqt5charts5-dev libqt5svg5-dev
```

Then clone, build and run:

```bash
git clone --recursive https://github.com/loki-project/loki-network-control-panel
cd loki-network-control-panel
mkdir build
cd build
cmake ..
make -j4
./lokinet-gui
```

### Systemd

The app can be compiled to support `systemd` for starting and stopping `lokinet`. To enable this, invoke `cmake` with `-DSYSTEMD=ON`.

Note that interactions with systemd may require privilege escalation, which is handled through `polkit`. A graphical polkit backend must be running for this to work.

    cmake -DSYSTEMD=ON ..

### Windows

Nightly Windows builds are available via [Continous Integration](https://builds.lokinet.dev/oxen-io/loki-network-control-panel/master/).

### MacOS

The above commands work to build a working executable, however if you also want to bundle it into a
`.app` bundle invoke cmake as:

    cmake -DMACOS_APP=ON -DBUILD_STATIC_DEPS=ON ..

which will produce a lokinet-gui.app executable app bundle directory containing the binary,
resources, and dependencies.

Note that building this way additionally requires imagemagick installed and built with SVG support.

### Usage

The following command-line arguments are supported:

```
--tray

Runs the gui in tray mode.

--rpc 

Provide a custom rpc url for talking to lokinet, defaults to tcp://127.0.0.1:1190

```

