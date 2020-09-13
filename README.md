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

| Topic                                                                                     | Status |
|-------------------------------------------------------------------------------------------|--------|
| Integrate auto-layout algorithm (https://github.com/cgilles/cute-collage)                 |        |
| Create new templates from PNG files hosted in data/TODO/                                  |        |
| Export as template do not work as expected. Result do not reflect of the canvas content   |        |
| Show Grid lines do nothing                                                                |        |
| Colorize effect rendering is not processed all time while editing properties              |        |
| Polaroid border: how to change text of legend?                                            |        |
| Text tool is not rendered on the canvas                                                   |        |
| Effects in Qt Property Editor cannot be cumulated (Borders work as espected)              |        |
| Add i18n support with QtLinguist.                                                         |  done  |
| Enforce more modern Qt API.                                                               |  done  |
| Fix coding style (constify, brackets, indent, spaces, new lines, const ref, etc.).        |  done  |
| Leaving PLE with canvas data not previously saved do not ask to save data on new file.    |  done  |
| Fix captitalized class file names.                                                        |  done  |
| Fix crashes in qt properties pannel.                                                      |  done  |
| Fix broken signal/slots in Qt Property Editor.                                            |  done  |
| Fix all static analyzers reports.                                                         |  done  |
| Fix all C++11 warnings (nullptr, override, fallthrough, overloaded virtual, etc.).        |  done  |
| Remove KDE dependencies                                                                   |  done  |
| Port to QMainWindow                                                                       |  done  |
| Rewrite configuration dialog                                                              |  done  |
| Port to DPlugin API                                                                       |  done  |
| Port as stand alone plugin                                                                |  done  |
