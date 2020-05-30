# Photo Layout Editor

This tool allows you to create collage layout of images using effect.

# Dependencies

- Qt5.
- Cmake 3.x.
- Exiv2 0.27.x API.
- digiKam 7.x plugin interface. <https://www.digikam.org> (optional)

# Compiling and Installing

- mdkir build
- cd build
- cmake . -DCMAKE_INSTALL_PREFIX=/usr ..                            (stand alone version only)
- cmake . -DCMAKE_INSTALL_PREFIX=/usr -DENABLE_DPLUGIN ..           (stand alone version and digiKam plugin)
- make
- make install/fast
