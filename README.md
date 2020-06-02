# Photo Layout Editor

This tool allows you to create collage layout of images using effect.

# Dependencies

- Cmake 3.x.
- Qt5.
- KDE Framework (KF5).
- digiKam 7.x plugin interface. <https://www.digikam.org> (optional)

# Compiling and Installing

- mdkir build
- cd build
- cmake . -DCMAKE_INSTALL_PREFIX=/usr ..                            (stand alone version only)
- cmake . -DCMAKE_INSTALL_PREFIX=/usr -DENABLE_DPLUGIN ..           (stand alone version and digiKam plugin)
- make
- make install/fast

![](https://i.imgur.com/79xs2Ef.png)

# TODO

- export as template do not work as expected. Result do not refect of the canvas content.
- Fix crashes in qt properties pannel.
- Remove KDE dependencies.
- Fix broken signal/slots.
- Port to QMainWindow.
- Rewrite configuration dialog (done).
- Port to C++11.
- Port to DPlugin API (done).
- Port as stand alone plugin (done).