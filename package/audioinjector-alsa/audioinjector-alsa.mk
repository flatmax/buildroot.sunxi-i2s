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

$(eval $(kernel-module))
$(eval $(generic-package))
