#!/bin/bash

# Copyright (c) 2013-2020 by Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Run Clazy analyzer on whole digiKam source code.
# https://github.com/KDE/clazy
# Dependencies : Python BeautifulSoup and SoupSieve at run-time.
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

. ./common.sh

checksCPUCores

ORIG_WD="`pwd`"
REPORT_DIR="${ORIG_WD}/report.clazy"
WEBSITE_DIR="${ORIG_WD}/site"

# Get active git branches to create report description string
TITLE="digiKam-$(parseGitBranch)$(parseGitHash)"
echo "Clazy Static Analyzer task name: $TITLE"

# Do not parse unwanted directories accordingly with Krazy configuration.
krazySkipConfig

IGNORE_DIRS=".*include.*|"

for DROP_ITEM in $KRAZY_FILTERS ; do
    IGNORE_DIRS+=".*$DROP_ITEM.*|"
done

# Remove last character
IGNORE_DIRS=${IGNORE_DIRS::-1}

export CLAZY_IGNORE_DIRS=$IGNORE_DIRS
export CLAZY_CHECKS="\
level2,\
no-fully-qualified-moc-types,\
no-qproperty-without-notify,\
no-old-style-connect,\
no-rule-of-three,\
no-inefficient-qlist-soft,\
no-qstring-allocations,\
no-qstring-arg,\
no-qstring-insensitive-allocation,\
no-qstring-ref\
"

echo "IGNORE DIRECTORIES:     $CLAZY_IGNORE_DIRS"
echo "CHECKERS CONFIGURATION: $CLAZY_CHECKS"

# Clean up and prepare to scan.

rm -fr $REPORT_DIR
rm -fr $WEBSITE_DIR
mkdir -p $REPORT_DIR

cd ..

rm -fr build.clazy
mkdir -p build.clazy
cd build.clazy

cmake -G "Unix Makefiles" . \
      -DCMAKE_INSTALL_PREFIX=/usr \
      -DCMAKE_BUILD_TYPE=debug \
      -DCMAKE_CXX_COMPILER=clazy \
      -DENABLE_DPLUGIN=ON \
      -Wno-dev \
      ..

make -j$CPU_CORES 2> ${REPORT_DIR}/trace.log

cd $ORIG_WD

python3 ./clazy_visualizer.py $REPORT_DIR/trace.log

rm -f $REPORT_DIR/trace.log
mv clazy.html $REPORT_DIR/index.html

cd $ORIG_DIR
