#!/bin/bash

# Here we pick what gets to be compiled. The role of this script is to
# call cmake with the appropriate options. After this, the build
# directory should be ready to run 'make'.

## Significant environnement variables:
# - CI_JOB                    (e.g. ubuntu_gcc-4.8_options)
# - CI_CMAKE_OPTIONS          (additional arguments to pass to cmake)
# - CI_ARCH = x86 | amd64     (for Windows builds)
# - CI_BUILD_TYPE             Debug|Release
# - CC and CXX
# About available libraries:
# - CI_HAVE_BOOST
# - CI_BOOST_PATH             (empty string if installed in standard location)
# - CI_QT_PATH
# - CI_BULLET_DIR             (Path to the directory containing BulletConfig.cmake)
# - CI_HAVE_ASSIMP
# - CI_HAVE_OPENCASCADE
# - CI_HAVE_CUDA
# - CI_HAVE_OPENCL
# - CI_HAVE_CSPARSE
# - CI_HAVE_METIS


# Exit on error
set -o errexit


## Checks

usage() {
    echo "Usage: configure.sh <build-dir> <src-dir>"
}

if [[ "$#" = 2 ]]; then
    build_dir="$1"
    if [[ $(uname) = Darwin || $(uname) = Linux ]]; then
        src_dir="$(cd "$2" && pwd)"
    else
        # pwd with a Windows format (c:/ instead of /c/)
        src_dir="$(cd "$2" && pwd -W)"
    fi
else
    usage; exit 1
fi

if [[ ! -d "$src_dir/applications/plugins" ]]; then
    echo "Error: '$src_dir' does not look like a Sofa source tree."
    usage; exit 1
fi

if [[ ! -d "$build_dir" ]]; then
    mkdir -p "$build_dir"
fi
build_dir="$(cd "$build_dir" && pwd)"


## Defaults

if [ -z "$CI_ARCH" ]; then CI_ARCH="x86"; fi
if [ -z "$CI_JOB" ]; then CI_JOB="$JOB_NAME"; fi
if [ -z "$CI_JOB" ]; then CI_JOB="default"; fi
if [ -z "$CI_BUILD_TYPE" ]; then CI_BUILD_TYPE="Release"; fi


## Utils

generator() {
    if [[ $(uname) = Darwin || $(uname) = Linux ]]; then
        echo "Unix Makefiles"
    else
        echo "\"NMake Makefiles\""
    fi
}

call-cmake() {
    if [ $(uname) != Darwin -a $(uname) != Linux ]; then
        # Run cmake after calling vcvarsall.bat to setup compiler stuff
        local vcvarsall="call \"%VS110COMNTOOLS%..\\..\\VC\vcvarsall.bat\" $CI_ARCH"
        echo "Calling $COMSPEC /c \"$vcvarsall & cmake $*\""
        $COMSPEC /c "$vcvarsall & cmake $*"
    else
        cmake "$@"
    fi
}


## CMake options

cmake_options="-DCMAKE_COLOR_MAKEFILE=OFF -DCMAKE_BUILD_TYPE=$CI_BUILD_TYPE"

append() {
    cmake_options="$cmake_options $*"
}

# Options common to all configurations
append "-DSOFA_BUILD_TESTS=ON"
append "-DPLUGIN_SOFAPYTHON=ON"
if [[ -n "$CI_HAVE_BOOST" ]]; then
    append "-DBOOST_ROOT=$CI_BOOST_PATH"
fi

case $CI_JOB in
    # Build with as many options enabled as possible
    *options*)
        append "-DSOFA_BUILD_METIS=ON"
        append "-DSOFA_BUILD_ARTRACK=ON"

        if [[ -n "$CI_QT_PATH" ]]; then
            append "-DQT_ROOT=$CI_QT_PATH"
        fi

        if [[ -n "$CI_BULLET_DIR" ]]; then
            append "-DBullet_DIR=$CI_BULLET_DIR"
        fi

        ### Plugins
        append "-DPLUGIN_ARTRACK=ON"
        if [[ -n "$CI_BULLET_DIR" ]]; then
            append "-DPLUGIN_BULLETCOLLISIONDETECTION=ON"
        else
            append "-DPLUGIN_BULLETCOLLISIONDETECTION=OFF"
        fi
        # Missing CGAL library
        append "-DPLUGIN_CGALPLUGIN=OFF"
        # For Windows, there is the dll of the assimp library *inside* the repository
        if [[ ( $(uname) = Darwin || $(uname) = Linux ) && -z "$CI_HAVE_ASSIMP" ]]; then
            append "-DPLUGIN_COLLADASCENELOADER=OFF"
        else
            append "-DPLUGIN_COLLADASCENELOADER=ON"
        fi
        append "-DPLUGIN_COMPLIANT=ON"
        append "-DPLUGIN_EXTERNALBEHAVIORMODEL=ON"
        append "-DPLUGIN_FLEXIBLE=ON"
        # Requires specific libraries.
        append "-DPLUGIN_HAPTION=OFF"
        append "-DPLUGIN_IMAGE=ON"
        append "-DPLUGIN_INVERTIBLEFVM=ON"
        append "-DPLUGIN_MANIFOLDTOPOLOGIES=ON"
        append "-DPLUGIN_MANUALMAPPING=ON"
        if [ -n "$CI_HAVE_OPENCASCADE" ]; then
            append "-DPLUGIN_MESHSTEPLOADER=ON"
        else
            append "-DPLUGIN_MESHSTEPLOADER=OFF"
        fi
        append "-DPLUGIN_MULTITHREADING=ON"
        append "-DPLUGIN_OPTITRACKNATNET=ON"
        # Does not compile, but it just needs to be updated.
        append "-DPLUGIN_PERSISTENTCONTACT=OFF"
        append "-DPLUGIN_PLUGINEXAMPLE=ON"
        append "-DPLUGIN_REGISTRATION=ON"
        # Requires OpenHaptics libraries.
        append "-DPLUGIN_SENSABLE=OFF"
        if [[ -n "$CI_HAVE_BOOST" ]]; then
            append "-DPLUGIN_SENSABLEEMULATION=ON"
        else
            append "-DPLUGIN_SENSABLEEMULATION=OFF"
        fi
        # Requires Sixense libraries.
        append "-DPLUGIN_SIXENSEHYDRA=OFF"
        append "-DPLUGIN_SOFACARVING=ON"
        if [[ -n "$CI_HAVE_CUDA" ]]; then
            append "-DPLUGIN_SOFACUDA=ON"
        else
            append "-DPLUGIN_SOFACUDA=OFF"
        fi
        # Requires HAPI libraries.
        append "-DPLUGIN_SOFAHAPI=OFF"
        # Not sure if worth maintaining
        append "-DPLUGIN_SOFASIMPLEGUI=ON"
        append "-DPLUGIN_THMPGSPATIALHASHING=ON"
        # Requires XiRobot library.
        append "-DPLUGIN_XITACT=OFF"
        ;;
esac

# Options passed via the environnement
if [ ! -z "$CI_CMAKE_OPTIONS" ]; then
    cmake_options="$cmake_options $CI_CMAKE_OPTIONS"
fi

cd "$build_dir"

## Configure

echo "Calling cmake with the following options:"
echo "$cmake_options" | tr -s ' ' '\n'
if [ -e "full-build" ]; then
    call-cmake -G"$(generator)" $cmake_options "$src_dir"
else
    call-cmake $cmake_options .
fi
