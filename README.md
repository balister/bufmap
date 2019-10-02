This repository is a collection of material related to an effort to
show how to map the virtual addresses of the gnuradio buffer space
into the actual physical pages used. From this information, you
should be able to write fpga code that can use the ACP port (to avoid
worrying about caching issues) to implement zero copy fgpa based
GNURadio accelerators.

The pieces of the puzzle are:

1. A kernel driver that pins the buffer into physical memory and
calculates the physical address.

1. A simple GNURadio flowgraph to exercise the driver.

1. Use with this gnuradio branch: https://github.com/balister/gnuradio/tree/call-bufmap

Steps to run:

1. Compile the kernel module
$ make -C/lib/modules/$(uname -r)/build M=$(pwd) modules

1. Install the kernel module
$ sudo insmod bufmap.ko

1. Run the buftest.grc flowgraph

1.Use journalctl to inspect the kernel messages


