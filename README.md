# NanoPi Neo4 buildroot system

# Initial setup

Clone buildroot. For example :

```
cd yourPath
git clone git://git.busybox.net/buildroot buildroot.neo
# if the HEAD is unstable, this works :
#git checkout e3c86f5c9e466ed5135e824d6dcebcfd7f5ac1ab
```

Make sure you have requirements :
```
sudo apt-get install -y build-essential gcc g++ autoconf automake libtool bison flex gettext
sudo apt-get install -y patch texinfo wget git gawk curl lzma bc quilt
```

Clone the NanoPi.Neo external buildroot tree :
```
# this doesn't exist yet
#git clone git@github.com:flatmax/NanoPi.Neo4.buildroot.external.git NanoPi.Neo
```

# To make the system

```
. NanoPi.Neo.buildroot.external/setup.sh yourPath/buildroot.neo
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
NOTE: Be super careful here!

```
sudo dd if=output/images/sdcard.img of=/dev/sdxx
```

# using

ssh in as user root, no pass. Or connect to the console debug uart with a serial cable.
