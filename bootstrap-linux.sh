#!/bin/bash

# Copyright (c) 2008-2020 by Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# Copy this script on root folder where are source code

#export VERBOSE=1

# We will work on command line using MinGW compiler
export MAKEFILES_TYPE='Unix Makefiles'

# Uncomment and adjust lines below to use Clang compiler instead GCC
# export CC=/usr/bin/clang
# export CXX=/usr/bin/clang++

if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# set build type (options are: None Debug Release RelWithDebInfo MinSizeRel tsan asan lsan msan ubsan) 
cmake -G "$MAKEFILES_TYPE" . \
      -DCMAKE_INSTALL_PREFIX=/usr \
      -DCMAKE_BUILD_TYPE=debug \
      -Wno-dev \
      -DENABLE_DPLUGIN=ON \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      ..

