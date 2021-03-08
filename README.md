# NanoPi Neo buildroot system

# Thank you

Thanks to Code Kipper for their initial inspiration on patching the sunxi i2s Linux driver and continued discussions.

# Initial setup

Clone buildroot. For example :

```
cd yourPath
git clone git://git.busybox.net/buildroot buildroot.neo
git checkout 34cce93adb06608992023c44fa3245d1f1a3deb4
```

Make sure you have requirements :
```
sudo apt-get install -y build-essential gcc g++ autoconf automake libtool bison flex gettext
sudo apt-get install -y patch texinfo wget git gawk curl lzma bc quilt
```

Clone the NanoPi.Neo external buildroot tree :
```
git clone git@github.com:flatmax/buildroot.nanopi.neo.git
```

# To make the system

```
. buildroot.nanopi.neo/setup.sh yourPath/buildroot.neo
```

# ensure you have your buildroot net downloads directory setup

```
mkdir yourPath/buildroot.dl
```

# build the system

```
make
```

# installing

Insert your sdcard into your drive and make sure it isn't mounted. Write the image to the disk.

NOTE: The following command will overwrite any disk attached to /dev/sdg
NOTE: It tries to guard against you overwriting your root partition of your build computer.

```
OF=/dev/sdb; rootDrive=`mount | grep " / " | grep $OF`; if [ -z $rootDrive ]; then sudo umount $OF[123456789]; sudo dd if=output/images/sdcard.img of=$OF; else echo you are trying to overwrite your root drive; fi
```

# using

ssh in as user root, no pass. Or connect to the console debug uart with a serial cable.
