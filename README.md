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

![](https://i.imgur.com/dFDdo2l.png)
