#!/bin/bash
TYPE=release
CMAKE=cmake
if [ $# -gt 0 ]; then
	TYPE=$1
fi
if [[ $TYPE != "debug" && $TYPE != "release" ]]; then
	echo "error param! please input 'debug' or 'release'."
	exit -1
fi

PROJECT_PATH=$(cd `dirname $0`; pwd)
PROJECT_NAME="${PROJECT_PATH##*/}"
echo "project path:" $PROJECT_PATH
echo "project name:" $PROJECT_NAME

BUILD_RELEASE_DIR=bin/$TYPE
if [ ! -d $BUILD_RELEASE_DIR ]; then
	mkdir -p $BUILD_RELEASE_DIR
fi
cd $BUILD_RELEASE_DIR
$CMAKE $PROJECT_PATH -DCMAKE_BUILD_TYPE=$TYPE
make