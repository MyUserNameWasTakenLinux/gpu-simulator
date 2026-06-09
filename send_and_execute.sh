#!/bin/bash

adb push builddir/gpu-simulator /data/local/tmp
echo "--------------------------------------------"
adb shell "chmod +x /data/local/tmp/gpu-simulator && /data/local/tmp/gpu-simulator"
echo "--------------------------------------------"
