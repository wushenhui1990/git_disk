soc-core.c
module_init(snd_soc_init);c0421da0
platform_driver_register(&soc_driver);
static int soc_probe(struct platform_device *pdev)
int snd_soc_register_card(struct snd_soc_card *card)
snd_soc_instantiate_cards();

rk30_i2s.c
module_init(rockchip_i2s_init);c0421f78
platform_driver_register(&rockchip_i2s_driver);
static int __devinit rockchip_i2s_probe(struct platform_device *pdev)
int snd_soc_register_dai(struct device *dev,
		struct snd_soc_dai_driver *dai_drv)
snd_soc_instantiate_cards();

pcm1792a.c
module_init(pcm1792a_modinit);c042c3bc
platform_driver_register(&pcm1792a_codec_driver);
static int pcm1792a_platform_probe(struct platform_device *pdev)
int snd_soc_register_codec(struct device *dev,
			   const struct snd_soc_codec_driver *codec_drv,
			   struct snd_soc_dai_driver *dai_drv,
			   int num_dai)
snd_soc_register_dais(dev, dai_drv, num_dai);
snd_soc_instantiate_cards();

snd_soc_instantiate_cards();


soc-core.c
static void snd_soc_instantiate_card(struct snd_soc_card *card)
static int soc_probe_dai_link(struct snd_soc_card *card, int num)
static int soc_new_pcm(struct snd_soc_pcm_runtime *rtd, int num)
int snd_pcm_new(struct snd_card *card, const char *id, int device,
		int playback_count, int capture_count,
	        struct snd_pcm ** rpcm)
static struct snd_device_ops ops = {
		.dev_free = snd_pcm_dev_free,
		.dev_register =	snd_pcm_dev_register,
		.dev_disconnect = snd_pcm_dev_disconnect,
	};

static void snd_soc_instantiate_card(struct snd_soc_card *card)
int snd_card_register(struct snd_card *card)
int snd_device_register_all(struct snd_card *card)
static int snd_pcm_dev_register(struct snd_device *device)
