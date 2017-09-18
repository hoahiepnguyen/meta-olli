# Meta-olli
by hiep@olli-ai.com

[Home Page] (https://olli-ai.com)

Version: 1.0.0 (09/18/2017)

## Yocto Info

The yocto version is 2.2.1 the [morty] branch

The 4.4.30 Linux kernel comes from the olli-linux repository.

The u-boot version is 2014.04

The root filesymtems base on meta-ti layer

Device tree binatries are generated and installed that support

1. Beaglebone black (am335x-boneblack.dtb)
2. Beaglebone green wireless (am335x-bonegreen-wireless.dtb)

## Ubuntu Setup

The Ubuntu version is 16.04 64-bit servers for builds. Older version should work.

* You will need at least the following packages installed

```
build-essential
chrpath
diffstat
gawk
git
libncurses5-dev
pkg-config
subversion
texi2html
texinfo
```

* For 16.04 you also need to install python 2.7 package that the Yocto 2.2 branch requires
 #python2.7

And then create a link for it in `/usr/bin`

```
# sudo ln -sf /usr/bin/python2.7 /usr/bin/python
```

* For all version of Ubuntu, you should change the default Ubuntu shell from `dash` to `bash` by running this command from a shell
```
# sudo dpkg-reconfigure dash
```
Choose #No to dash when prompted

## Clone the repository

### Create the `yocto` folder to build

```
dark&hiepnguyen:~$ mkdir yocto
dark&hiepnguyen:~$ cd yocto
```

* First the main Yocto project `poky` repository

```
dark&hiepnguyen:/yocto~$ git clone -b morty git://git.yoctoproject.org/poky.git poky
```

* Then the `meta-openembedded` repository
```
dark@hiepnguyen:~$ cd poky
dark@hiepnguyen:~/yocto/poky$ git clone -b morty git://git.openembedded.org/meta-openembedded
```

* Clone the `meta-olli` repository

```
dark@hiepnguyen:~/yocto/poky$ git clone git://github.com:hoahiepnguyen/meta-olli.git
```

## Building

### Initialize the build directory

Much of the following are only the conventions that I use. All of the paths to the meta-layers are configurable.

Use the Yocto environment script `oe-init-build-env` like this passing in the path to the build directory

```
dark&hiepnguyen:/yocto~$ source poky/oe-init-build-env build
```

The Yocto environment script will create the build directory if it does not already exist.

### Customize the configuration files

There are some sample configuration files in the `meta-olli/conf` directory.

Copy them to the `build/conf` directory:

```
dark@hiepnguyen:~/yocto/poky$ cp meta-olli/conf/local.conf build/conf/local.conf
dark@hiepnguyen:~/yocto/poky$ cp meta-olli/conf/bblayres.conf build/conf/bblayers.conf
```

### Run the build

You need to source the Yocto environment into your shell before you can `use bitbake`. The `oe-init-build-env` will not overwrite your customized conf files.

```
dark@hiepnguyen:~/yocto$ source poky/oe-init-build-env build/

### Shell environment set up for builds. ###

You can now run 'bitbake <target>'

Common targets are:
    core-image-minimal
    core-image-sato
    meta-toolchain
    meta-ide-support

You can also run generated qemu images with a command like 'runqemu qemux86'
dark@hiepnguyen:~/yocto/build$ 

```

This is one custom images available in the meta-olli layers. The recipes for the images can be found in `meta-olli/images/`:

* core-olli-image.bb

To build the `core-olli-image` run the following command

```
#dark@hiepnguyen:~/yocto/build$ bitbake core-olli-image
```

## Copying the binaries to eMMC on Mainboard

After the build completes, the bootloader, kernel and rootfs image files can be found in `<TMPDIR>/deploy/images/beaglebone/`.

The `meta-olli/scipts` directory has some helper scripts to format and copy the files to a eMMC flash memory.

### usb_flasher

This binary will boot the FIT image to external RAM on mainboard through micro-usb cable. Then, mount eMMC flash into PC Linux.

Run `usbflash` by command:
```
dark@hiepnguyen:~/yocto/meta-olli/scripts/booting/$ sudo ./usb_flasher

```

### mk2parts.sh

This script will partition an eMMC with the minimal 2 partitions required for the boards.

Power on the mainboard and change to usb boot.

`lsblk` is convenient for fiding the eMMC card.

For exmple:
```dark@hiepnguyen:~$ lsblk 
NAME   MAJ:MIN RM   SIZE RO TYPE MOUNTPOINT
sdb      8:16   0 931,5G  0 disk 
├─sdb2   8:18   0 685,6G  0 part 
└─sdb1   8:17   0 245,9G  0 part 
sdc      8:32   0   3,7G  0 disk 
├─sdc2   8:34   0   3,6G  0 part /media/dark/ROOT
└─sdc1   8:33   0    64M  0 part /media/dark/BOOT
sda      8:0    0 232,9G  0 disk 
├─sda2   8:2    0 224,5G  0 part /
├─sda3   8:3    0   7,9G  0 part [SWAP]
└─sda1   8:1    0   512M  0 part /boot/efi
```

Mainboard mounted at `sdc` on this Linux PC.

#### BE CAREFULY with this script. It will format any disk on your workstation

```
dark@hiepnguyen:~/yocto/meta-olli/scripts$ sudo ./mk2part.sh sdc

```

You only have to format the eMMC flash once.

### copy_boot.sh

This script copies the bootloader(MLO and u-boot) to the boot partition of the eMMC flash memory.

This script also copies a `uEnv.tx` file to the boot partition if it find one in either.

```
<TMPDIR>/deploy/images/beaglebone/
```
or in the local directory where the script is run from.

#### /media/card ####

You will need to create a mount point on your workstation for the copy script to use.

```
dark@hiepnguyen:~$ sudo mkdir /media/card
```
You only have to create this directory once.


This script need to know the TMPDIR to find the binaries. It looks for an environment variable called OETMP.

So, you can export this environment variable before running `copy_boot.sh`

```
dark@hiepnguyen:~/yocto/meta-olli/scripts$ export OETMP=/home/dark/yocto/build/tmp

```

Then run the `copy_boot.sh` script passing the location of eMMC flash.

```
dark@hiepnguyen:~/yocto/meta-olli/scripts$ ./copy_boot.sh sdc

```

This cript should be run very fast.

### copy_rootfs.sh

This script copies the zImage kernel, the device tree binaries and the rest of the operating system to the root file system partition of the eMMC flash.

Here's an example of how to run `copy_rootfs.sh`:

```
dark@hiepnguyen:~/yocto/meta-olli/scripts$ sudo ./copy_rootfs.sh sdc

```

### bring_up.sh

This script includes all the functions listed above. You must power on the mainboard, press the "boot configuration" button to boot from usb. It will call usb_flasher to boot FIT image on external RAM. After, mounted the eMMC flash memory into PC linux. Then, copies bootloader, zImage, device tree and rootfile system to the partition of the eMMC.

This cripts need to OETMP path to yocto direction. Type command below:

```
dark@hiepnguyen:~/yocto/meta-olli/booting$ sudo ./bring_up.sh /home/dark/yocto/build/tmp 

```

# OLLI-FIRMWARE-TEAM
## We are a small group of engineers and researchers who strongly believe in the uptapped power of Artificial Intelligence
