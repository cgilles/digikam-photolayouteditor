# Photo Layout Editor

This tool allows you to create collage layout of images using effect.

# Dependencies

- Cmake 3.x.
- Qt5.
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

- Integrate auto-layout algorithm (https://github.com/cgilles/cute-collage).
- Export as template do not work as expected. Result do not refect of the canvas content.
- Fix crashes in qt properties pannel.
- Fix broken signal/slots.
- Fix captitalized class file names.
- Fix all C++11 warnings (nullptr, override, fallthrough, etc.).
- Fix coding style (constify, brackets, indent, etc.).
- Remove KDE dependencies (done).
- Port to QMainWindow (done).
- Rewrite configuration dialog (done).
- Port to DPlugin API (done).
- Port as stand alone plugin (done).
