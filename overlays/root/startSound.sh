#!/usr/bin/env bash
if [ $# -eq "0" ]; then
  HOST=192.168.1.4
else
  HOST=$1
fi
ssh-keygen -f ~/.ssh/known_hosts -R $HOST
sleep_time=0
BUILD=/home/flatmax.unencrypted/buildroot.neo4/output/build
scp $BUILD/audioinjector-alsa-0.0.1/audioinjector-nanopi/*.ko root@$HOST:/lib64/modules/4.4.143/kernel/sound/soc/rockchip
scp $BUILD/akm-codecs-0.0.1/codecs/*.ko root@$HOST:/lib64/modules/4.4.143/kernel/sound/soc/codecs
ssh root@$HOST "echo 1; modprobe -r snd_soc_audioinjector_nanopi_akm; sleep $sleep_time"
ssh root@$HOST "echo 2; rmmod snd-soc-ak4137; sleep $sleep_time"
ssh root@$HOST "echo 3; rmmod snd-soc-ak5572; sleep $sleep_time"
ssh root@$HOST "echo 4; rmmod snd-soc-ak4490; sleep $sleep_time"
ssh root@$HOST "echo 5; modprobe snd-soc-ak4137; sleep $sleep_time"
ssh root@$HOST "echo 6; modprobe snd-soc-ak5572; sleep $sleep_time"
ssh root@$HOST "echo 7; modprobe snd-soc-ak4490; sleep $sleep_time"
ssh root@$HOST "echo 8; modprobe snd_soc_audioinjector_nanopi_akm"
# sleep 1
# ssh root@$HOST "aplay test.wav"
# # echo aplay test.wav
# echo aplay -B 100000 -Dhw:0,2 tone.wav
# #echo aplay -c 2 -f S32_LE -r 48000 /dev/random
