#!/usr/bin/env bash

HOST=audioinjector.local
sleep_time=0
BR_PATH=/home/flatmax.unencrypted/buildroot.NanoPi.Neo
SUNXI_PATH=$BR_PATH/output/build/linux-5.3.13/sound/soc/sunxi
AI_PATH=$BR_PATH/output/build/audioinjector-alsa-0.0.1/audioinjector-nanopi
HOST_SUNXI_PATH=/lib/modules/5.3.13/kernel/sound/soc/sunxi
scp $AI_PATH/snd-soc-audioinjector-nanopi-neo-stereo.ko $SUNXI_PATH/sun4i-i2s.ko  root@audioinjector.local:$HOST_SUNXI_PATH

ssh root@$HOST "rmmod snd-soc-audioinjector-nanopi-neo-stereo snd-soc-wm8731 sun4i-i2s snd_soc_core snd_pcm_dmaengine snd_pcm snd_timer snd"
ssh root@$HOST "/etc/init.d/S30start-audio start"
ssh root@$HOST "alsactl restore"
