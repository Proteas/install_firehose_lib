#!/bin/sh
set -e

pushd `dirname $0` > /dev/null
CURRENT_DIR=`pwd`
popd > /dev/null
# ================================================================ #

# SDK_ROOT=`xcodebuild -version -sdk macosx Path`
SDK_ROOT=`xcodebuild -version -sdk iphoneos Path`
echo "Target SDK Path: ${SDK_ROOT}"
# ================================================================ #

# LIBDISPATCH_DIR_NAME="libdispatch-703.1.4"
# LIBDISPATCH_DIR_NAME="libdispatch-703.20.1"
# LIBDISPATCH_DIR_NAME="libdispatch-703.30.5"
# LIBDISPATCH_DIR_NAME="libdispatch-703.50.37"
# LIBDISPATCH_DIR_NAME="libdispatch-913.1.6"
# LIBDISPATCH_DIR_NAME="libdispatch-913.20.5"
# LIBDISPATCH_DIR_NAME="libdispatch-913.30.4"
# LIBDISPATCH_DIR_NAME="libdispatch-913.50.12"
LIBDISPATCH_DIR_NAME="libdispatch-913.60.2"

echo "Install: ${LIBDISPATCH_DIR_NAME}"
# ================================================================ #

TARGET_HEADER_DIR="${SDK_ROOT}/usr/local/include/firehose"
TARGET_LIB_DIR="${SDK_ROOT}/usr/local/lib/kernel"

echo "Dest Header Dir: ${TARGET_HEADER_DIR}"
echo "Dest Library Dir: ${TARGET_LIB_DIR}"
# ================================================================ #

mkdir -p "${TARGET_HEADER_DIR}"
mkdir -p "${TARGET_LIB_DIR}"
# ================================================================ #

ditto "${CURRENT_DIR}/${LIBDISPATCH_DIR_NAME}/firehose" "${TARGET_HEADER_DIR}"
cp -rf "${CURRENT_DIR}/${LIBDISPATCH_DIR_NAME}"/libfirehose_kernel*.a "${TARGET_LIB_DIR}/"
# ================================================================ #

echo "======== Done ========"
# ================================================================ #
