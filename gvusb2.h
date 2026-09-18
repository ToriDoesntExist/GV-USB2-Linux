/* SPDX-License-Identifier: GPL-2.0-only OR BSD-3-Clause */
/*
 * Copyright (c) 2019 Isaac Lozano <109lozanoi@gmail.com>
 *
 * This code is released using a dual license strategy: BSD/GPL
 * You can choose the licence that better fits your requirements.
 *
 * Released under the terms of 3-clause BSD License
 * Released under the terms of GNU General Public License Version 2.0
 *
 */

#ifndef __GVUSB2_H__
#define __GVUSB2_H__

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/usb.h>

#define GVUSB2_NUM_URBS 4
#define GVUSB2_NUM_ISOCH_PACKETS 0x100
#define GVUSB2_MAX_AUDIO_PACKET_SIZE 0x100
#define GVUSB2_MAX_VIDEO_PACKET_SIZE 0xc00
#define GVUSB2_VENDOR_ID  0x04bb
#define GVUSB2_PRODUCT_ID 0x0532

#define gvusb2_dbg(dev, fmt, args...) \
	dev_info(dev, fmt, ## args)

struct gvusb2_dev {
	struct usb_device *udev;
};

int gvusb2_read_reg(struct gvusb2_dev *dev, u16 reg, u8 *value);
int gvusb2_write_reg(struct gvusb2_dev *dev, u16 reg, u8 value);
int gvusb2_set_reg_mask(struct gvusb2_dev *dev, u16 reg, u8 mask, u8 value);
int gvusb2_init(struct gvusb2_dev *dev, struct usb_device *udev);
int gvusb2_free(struct gvusb2_dev *dev);
int gvusb2_snd_reset_adc(struct gvusb2_dev *dev);

struct gvusb2_snd {
	struct gvusb2_dev gv;
	struct usb_interface *intf;
	struct usb_endpoint_descriptor *ep;

	/* urb */
	struct urb *urbs[GVUSB2_NUM_URBS];

	/* alsa */
	struct snd_card *card;
	struct snd_pcm *pcm;
	struct snd_pcm_substream *substream;
	int dma_offset;
	int avail;
	int hw_ptr;
	spinlock_t lock;
};

void gvusb2_snd_process_pcm(struct gvusb2_snd *dev, unsigned char *buf, unsigned int len);
int gvusb2_snd_alsa_init(struct gvusb2_snd *dev);
//void gvusb2_snd_cancel_isoc(struct gvusb2_snd *dev);
void gvusb2_snd_free_isoc(struct gvusb2_snd *dev);
void gvusb2_snd_process_isoc(struct gvusb2_snd *dev, struct urb *urb);
int gvusb2_snd_probe(struct usb_interface *intf, const struct usb_device_id *id);
void gvusb2_snd_disconnect(struct usb_interface *intf);

#endif

