The perf tool is a part of the Linux kernel code base. The standard Linux distribution package for the perf tool includes only basic support, lacking the full array of options. This necessitates building the perf tool from scratch with all options enabled. Here are the steps to build perf from scratch:

Step 1: Download the Kernel source code

```
curl -o linux-source.{VERSION}.tar.gz https://mirrors.edge.kernel.org/pub/linux/kernel/v6.x/linux-{VERSION}.tar.gz
```

Note: Replace the kernel version. I am using Version 6.2

Step 2: Extract the Kernel source package

`tar -xvf linux-source.{VERSION}.tar.gz`

Step 3: Install build essential packages.

`sudo apt install make gcc flex bison`

Step 4: Install the required libraries to enable all the feature in perf
```
libzstd1
libdwarf-dev
libdw-dev
binutils-dev
libcap-dev
libelf-dev
libnuma-dev
python3
python3-dev
python-setuptools
libssl-dev
libunwind-dev
libdwarf-dev
zlib1g-dev
liblzma-dev
libaio-dev
libtraceevent-dev
debuginfod
libpfm4-dev
libslang2-dev
systemtap-sdt-dev
libperl-dev
binutils-dev
libbabeltrace-dev
libiberty-dev
libzstd-dev
```


`sudo apt install libzstd1 libdwarf-dev libdw-dev binutils-dev libcap-dev libelf-dev libnuma-dev python3 python3-dev python-setuptools libssl-dev libunwind-dev libdwarf-dev zlib1g-dev liblzma-dev libaio-dev libtraceevent-dev debuginfod libpfm4-dev libslang2-dev systemtap-sdt-dev libperl-dev binutils-dev libbabeltrace-dev libiberty-dev libzstd-dev`

Step 5: Step into the perf directory and make

`manas@sandbox: ~/kernel cd linux-6.2/tools/perf`

`manas@sandbox: ~/kernel/linux-6.2/tools/perf make`

The build configuration will display the supported features. In this case, all the required libraries are present. The build configuration will also indicate any missing libraries if they are not present in the system

```
Auto-detecting system features:
…                               dwarf: [ on ]
…                  dwarf_getlocations: [ on ]
…                               glibc: [ on ]
…                              libbfd: [ on ]
…                      libbfd-buildid: [ on ]
…                              libcap: [ on ]
…                              libelf: [ on ]
…                             libnuma: [ on ]
…              numa_num_possible_cpus: [ on ]
…                             libperl: [ on ]
…                           libpython: [ on ]
…                           libcrypto: [ on ]
…                           libunwind: [ on ]
…                  libdw-dwarf-unwind: [ on ]
…                                zlib: [ on ]
…                                lzma: [ on ]
…                           get_cpuid: [ on ]
…                                 bpf: [ on ]
…                              libaio: [ on ]
…                             libzstd: [ on ]
```

Step 6: Check perf version

```
manas@sandbox: ~/kernel/linux-6.2/tools/perf$ ./perf --version
perf version 6.2.0
```


Reference: https://medium.com/@manas.marwah/building-perf-tool-fc838f084f71