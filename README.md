## Building

To build, first create a file called `android_cross.txt` and use this template:

```
[binaries]
c = '/path/to/aarch64-linux-android33-clang'
cpp = '/path/to/aarch64-linux-android33-clang++'
ar = '/path/to/llvm-ar'
strip = '/path/to/llvm-strip'

[host_machine]
system = 'android'
cpu_family = 'aarch64'
cpu = 'armv8-a'
endian = 'little'
```

The binaries are from the Android NDK.

To build the project, run the following commands,

```
meson setup builddir/
meson compile -C builddir/
```

To run the project, run the script `send_and_execute.sh` (make sure you're connected through adb).
