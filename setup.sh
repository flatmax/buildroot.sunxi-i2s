#! /bin/bash
# Author : Matt Flax <flatmax@flatmax.org>
# Date : Nov 2018

if [ $# -lt 1 ]; then
  echo usage :
  me=`basename "$0"`
  echo "     " $me path.to.buildroot.neo
  echo for example :
  echo "     " $me /home/flatmax.unencrypted/buildroot.NanoPi.Neo
else
  DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

  CUSTOM_PATH=$DIR
  BR_REPO_PATH=$1
  BR_DEFCONFIG=nanopi_neo_defconfig
  #BR_DEFCONFIG=friendlyarm_nanopi_neo_defconfig
  BR_AUDIOINJECTOR_DEFCONFIG=audioinjector_custom_defconfig

#  BR_UBOOT_CMD=boot.cmd
#  DTB_NAME=audioinjector-nanopi-neo-stereo.dtb

  if [ ! -d "$BR_REPO_PATH" ]; then
  	echo Can\'t find the directory $BR_REPO_PATH please correct the bash script.
  	return;
  fi
  if [ ! -d "$CUSTOM_PATH" ]; then
  	echo Can\'t find the directory $CUSTOM_PATH please correct the bash script.
  	return;
  fi

  if [ ! -e $CUSTOM_PATH/configs/$BR_AUDIOINJECTOR_DEFCONFIG ]; then
  	echo can\'t find the file $CUSTOM_PATH/configs/$BR_AUDIOINJECTOR_DEFCONFIG
  	echo please fix this script
  	return;
  fi

  cd $BR_REPO_PATH

  echo "###### DON'T EDIT THIS FILE, EDIT $BR_AUDIOINJECTOR_DEFCONFIG INSTEAD" > $CUSTOM_PATH/configs/$BR_DEFCONFIG
  cat $BR_REPO_PATH/configs/$BR_DEFCONFIG >> $CUSTOM_PATH/configs/$BR_DEFCONFIG
  cat $CUSTOM_PATH/configs/$BR_AUDIOINJECTOR_DEFCONFIG >> $CUSTOM_PATH/configs/$BR_DEFCONFIG

#  cat $BR_REPO_PATH/board/friendlyarm/nanopi-neo/$BR_UBOOT_CMD >> $CUSTOM_PATH/configs/$BR_UBOOT_CMD
#  sed -i "s/sun8i-h3-nanopi-neo.dtb/$DTB_NAME/" $CUSTOM_PATH/configs/$BR_UBOOT_CMD

  make BR2_EXTERNAL=$CUSTOM_PATH $BR_DEFCONFIG
fi
