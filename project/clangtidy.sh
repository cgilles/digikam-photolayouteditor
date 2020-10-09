#!/bin/bash

# Copyright (c) 2020 by Marcel Mathis, <maeseee at gmail dot com>
#
# Run Clang tidy on ple code.
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

. ./common.sh

checksCPUCores

ORIG_WD="`pwd`"
REPORT_DIR="${ORIG_WD}/report.tidy"

# Clean up and prepare to scan.
rm -fr $REPORT_DIR
mkdir -p $REPORT_DIR

# Run clang tidy
python3 ./tidy/run-clang-tidy.py -quiet -j$CPU_CORES -p  ../build/ -header-filter=../src/ -fix | tee $REPORT_DIR/clang-tidy.log
