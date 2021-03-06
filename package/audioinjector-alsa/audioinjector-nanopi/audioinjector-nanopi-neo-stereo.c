/*
 * ASoC Driver for AudioInjector Neo stereo soundcard
 *
 *  Created on: 10-10-2020
 *      Author: flatmax@flatmax.org
 *              based on the Audio Injector Pi soundcard driver (audioinjector-pi-soundcard.c)
 *              with help from Lars-Peter Clausen for simplifying the original code to use the dai_fmt field.
 *
 * Copyright (C) 2016-2020 Flatmax Pty. Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#include <linux/module.h>
#include <linux/types.h>

#include <sound/core.h>
#include <sound/soc.h>
#include <sound/pcm_params.h>
#include <sound/control.h>

#include "../sound/soc/codecs/wm8731.h"

static const unsigned int bcm2835_rates_12000000[] = {
	8000, 16000, 32000, 44100, 48000, 96000, 88200,
};

static struct snd_pcm_hw_constraint_list bcm2835_constraints_12000000 = {
	.list = bcm2835_rates_12000000,
	.count = ARRAY_SIZE(bcm2835_rates_12000000),
};

static int snd_audioinjector_nanopi_soundcard_startup(struct snd_pcm_substream *substream)
{
	/* Setup constraints, because there is a 12 MHz XTAL on the board */
	snd_pcm_hw_constraint_list(substream->runtime, 0,
				SNDRV_PCM_HW_PARAM_RATE,
				&bcm2835_constraints_12000000);
	return 0;
}

static int snd_audioinjector_nanopi_soundcard_hw_params(struct snd_pcm_substream *substream,
				       struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;

	// switch (params_rate(params)){
	// 	case 8000:
	// 		return snd_soc_dai_set_bclk_ratio(cpu_dai, 1);
	// 	case 16000:
	// 		return snd_soc_dai_set_bclk_ratio(cpu_dai, 750);
	// 	case 32000:
	// 		return snd_soc_dai_set_bclk_ratio(cpu_dai, 375);
	// 	case 44100:
	// 		return snd_soc_dai_set_bclk_ratio(cpu_dai, 272);
	// 	case 48000:
	// 		return snd_soc_dai_set_bclk_ratio(cpu_dai, 250);
	// 	case 88200:
	// 		return snd_soc_dai_set_bclk_ratio(cpu_dai, 136);
	// 	case 96000:
	// 		return snd_soc_dai_set_bclk_ratio(cpu_dai, 125);
	// 	default:
	// 		return snd_soc_dai_set_bclk_ratio(cpu_dai, 125);
	// }
	return 0;
}

/* machine stream operations */
static struct snd_soc_ops snd_audioinjector_nanopi_soundcard_ops = {
	.startup = snd_audioinjector_nanopi_soundcard_startup,
	.hw_params = snd_audioinjector_nanopi_soundcard_hw_params,
};

static int audioinjector_nanopi_soundcard_dai_init(struct snd_soc_pcm_runtime *rtd)
{
	int ret = snd_soc_dai_set_sysclk(rtd->codec_dai, WM8731_SYSCLK_XTAL, 12288000, SND_SOC_CLOCK_IN);
	printk("audioinjector_nanopi_soundcard_dai_init\n");
	// return snd_soc_dai_set_sysclk(rtd->codec_dai, WM8731_SYSCLK_XTAL, 12000000, SND_SOC_CLOCK_IN);
	if (ret==0)
		return snd_soc_dai_set_bclk_ratio(rtd->cpu_dai, 64);
	return ret;
}

SND_SOC_DAILINK_DEFS(audioinjector_nanopi,
	// DAILINK_COMP_ARRAY(COMP_CPU("sun4i-i2s.0")),
	DAILINK_COMP_ARRAY(COMP_EMPTY()),

	DAILINK_COMP_ARRAY(COMP_CODEC(NULL, "wm8731-hifi")),

	// DAILINK_COMP_ARRAY(COMP_PLATFORM("sun4i-i2s.0")));
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

static struct snd_soc_dai_link audioinjector_nanopi_soundcard_dai[] = {
	{
		.name = "AudioInjector audio",
		.stream_name = "AudioInjector audio",
		.ops = &snd_audioinjector_nanopi_soundcard_ops,
		.init = audioinjector_nanopi_soundcard_dai_init,
		// .dai_fmt = SND_SOC_DAIFMT_CBM_CFM|SND_SOC_DAIFMT_I2S|SND_SOC_DAIFMT_NB_NF,
		.dai_fmt = SND_SOC_DAIFMT_CBM_CFM|SND_SOC_DAIFMT_I2S|SND_SOC_DAIFMT_NB_NF,
		SND_SOC_DAILINK_REG(audioinjector_nanopi),
	},
};

static const struct snd_soc_dapm_widget wm8731_dapm_widgets[] = {
	SND_SOC_DAPM_HP("Headphone Jack", NULL),
	SND_SOC_DAPM_SPK("Ext Spk", NULL),
	SND_SOC_DAPM_LINE("Line In Jacks", NULL),
	SND_SOC_DAPM_MIC("Microphone", NULL),
};

static const struct snd_soc_dapm_route audioinjector_audio_map[] = {
	/* headphone connected to LHPOUT, RHPOUT */
	{"Headphone Jack", NULL, "LHPOUT"},
	{"Headphone Jack", NULL, "RHPOUT"},

	/* speaker connected to LOUT, ROUT */
	{"Ext Spk", NULL, "ROUT"},
	{"Ext Spk", NULL, "LOUT"},

	/* line inputs */
	{"Line In Jacks", NULL, "Line Input"},

	/* mic is connected to Mic Jack, with WM8731 Mic Bias */
	{"Microphone", NULL, "Mic Bias"},
};

static struct snd_soc_card snd_soc_audioinjector = {
	.name = "audioinjector-nanopi-soundcard",
	.dai_link = audioinjector_nanopi_soundcard_dai,
	.num_links = ARRAY_SIZE(audioinjector_nanopi_soundcard_dai),

	.dapm_widgets = wm8731_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(wm8731_dapm_widgets),
	.dapm_routes = audioinjector_audio_map,
	.num_dapm_routes = ARRAY_SIZE(audioinjector_audio_map),
};

static int audioinjector_nanopi_soundcard_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &snd_soc_audioinjector;
	int ret;

	card->dev = &pdev->dev;

	printk("before of_node\n");
	if (!pdev->dev.of_node){
		dev_err(&pdev->dev, "of_node not present\n");
		return -EINVAL;
	}

	struct snd_soc_dai_link *dai = &audioinjector_nanopi_soundcard_dai[0];
	struct device_node *i2s_node = of_parse_phandle(pdev->dev.of_node,
							"i2s-controller", 0);

	printk("i2s_node %p\n",i2s_node);

	if (i2s_node) {
		dai->cpus->of_node = i2s_node;
		dai->platforms->of_node = i2s_node;
	} else {
		dev_err(&pdev->dev, "Property 'i2s-controller' missing or invalid\n");
		return -EINVAL;
	}

	dai->codecs->of_node = of_parse_phandle(pdev->dev.of_node,
			"nanopi,audio-codec", 0);
	if (!dai->codecs->of_node) {
		dev_err(&pdev->dev, "Property 'nanopi,audio-codec' missing or invalid\n");
		return -EINVAL;
	}

	printk("after of_node\n");
	if ((ret = devm_snd_soc_register_card(&pdev->dev, card))) {
		dev_err(&pdev->dev, "snd_soc_register_card failed (%d)\n", ret);
	}
	return ret;
}

static const struct of_device_id audioinjector_nanopi_soundcard_of_match[] = {
	{ .compatible = "ai,audioinjector-nanopi-soundcard", },
	{},
};
MODULE_DEVICE_TABLE(of, audioinjector_nanopi_soundcard_of_match);

static struct platform_driver audioinjector_nanopi_soundcard_driver = {
	.driver         = {
		.name   = "audioinjector-stereo",
		.owner  = THIS_MODULE,
		.of_match_table = audioinjector_nanopi_soundcard_of_match,
	},
	.probe          = audioinjector_nanopi_soundcard_probe,
};

module_platform_driver(audioinjector_nanopi_soundcard_driver);
MODULE_AUTHOR("Matt Flax <flatmax@flatmax.org>");
MODULE_DESCRIPTION("AudioInjector.net Neo Soundcard");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:audioinjector-nanopi-neo-stereo");
