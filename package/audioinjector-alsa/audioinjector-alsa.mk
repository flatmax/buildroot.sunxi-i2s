################################################################################
#
# audioinjector-alsa
#
################################################################################

AUDIOINJECTOR_ALSA_VERSION = 0.0.1
AUDIOINJECTOR_ALSA_SITE = $(BR2_EXTERNAL)/package/audioinjector-alsa
AUDIOINJECTOR_ALSA_SITE_METHOD = local
AUDIOINJECTOR_ALSA_DEPENDENCIES =

AUDIOINJECTOR_ALSA_MODULE_SUBDIRS = audioinjector-nanopi
#AUDIOINJECTOR_ALSA_CFLAGS = \
#	-I$(@D)/audioinjector-nanopi/ -I$(LINUX_DIR)/sound/soc/codecs
AUDIOINJECTOR_ALSA_MODULE_MAKE_OPTS = \
	INSTALL_MOD_DIR=kernel/sound/soc/sunxi \
	CONFIG_SND_SOC_AUDIOINJECTOR_NANOPI_NEO_STEREO=m


define AUDIOINJECTOR_ALSA_INSTALL_MOD
	$(INSTALL) -D -m 644 $(@D)/$(AUDIOINJECTOR_ALSA_MODULE_SUBDIRS)/snd-soc-audioinjector-nanopi-neo-stereo.ko \
		$(TARGET_DIR)/lib/modules/5.3.13/kernel/sound/soc/sunxi/snd-soc-audioinjector-nanopi-neo-stereo.ko
	$(INSTALL) -D -m 755 $(@D)/S30start-audio $(TARGET_DIR)/etc/init.d/S30start-audio

endef
AUDIOINJECTOR_ALSA_POST_INSTALL_TARGET_HOOKS += AUDIOINJECTOR_ALSA_INSTALL_MOD


$(eval $(kernel-module))
$(eval $(generic-package))
