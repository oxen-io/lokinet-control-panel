# Building for distribution on MacOS

1) Build as normal:
```bash
mkdir build
cd build
qmake ..
make
```

2) Use Qt deploy tool:
```bash
macdeployqt lokicp.app -always-overwrite -qmldir=../res
```

3) Create DMG image
```bash
hdiutil create LokiCP.dmg -ov -volname "LokinetControlPanel" -fs HFS+ -srcfolder lokicp.app
hdiutil convert LokiCP.dmg -format UDZO -o LokinetControlPanel.dmg
```
