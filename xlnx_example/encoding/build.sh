#!/bin/bash

set -e

#dir_build=$(dirname -- "$(readlink -f -- "$0")")
dir_build=../video_lib
#source "$dir_build/settings.sh"
make clean
STATIC=1 CROSS_COMPILE=aarch64-linux-gnu- make

#cp lab2a_capture_encode_filedump /proj/ppm_global/users/Damoder/TSC_2018/

