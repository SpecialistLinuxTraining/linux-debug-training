# Instructions for downloading vmlinux file in Ubuntu 22.04


vmlinux is the name of the Linux kernel image file that contains the statically linked executable kernel. It is the uncompressed and unstripped version of the Linux kernel binary. The vmlinux file includes all the kernel code and symbols, making it a valuable resource for various debugging and analysis tasks.

Step 1: Add DDEBS repository 

```
echo "deb http://ddebs.ubuntu.com $(lsb_release -cs)-updates main restricted universe multiverse
deb http://ddebs.ubuntu.com $(lsb_release -cs)-security main restricted universe multiverse
deb http://ddebs.ubuntu.com $(lsb_release -cs)-proposed main restricted universe multiverse" | \
sudo tee -a /etc/apt/sources.list.d/ddebs.list

sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 428D7C01
```

Step 2: Install kernel debug symbols

```
sudo apt-get update
sudo apt-get install linux-image-$(uname -r)-dbgsym
```

Step 3: Vmlinux file location

`/usr/lib/debug/boot/vmlinux-$(uname -r)`