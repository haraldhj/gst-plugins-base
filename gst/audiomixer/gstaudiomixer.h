/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 * Copyright (C) 2013      Sebastian Dröge <slomo@circular-chaos.org>
 *
 * gstaudiomixer.h: Header for GstAudioMixer element
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __GST_AUDIO_MIXER_H__
#define __GST_AUDIO_MIXER_H__

#include <gst/gst.h>
#include <gst/base/gstaggregator.h>
#include <gst/audio/audio.h>

G_BEGIN_DECLS

#define GST_TYPE_AUDIO_MIXER            (gst_audiomixer_get_type())
#define GST_AUDIO_MIXER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_AUDIO_MIXER,GstAudioMixer))
#define GST_IS_AUDIO_MIXER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_AUDIO_MIXER))
#define GST_AUDIO_MIXER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass) ,GST_TYPE_AUDIO_MIXER,GstAudioMixerClass))
#define GST_IS_AUDIO_MIXER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass) ,GST_TYPE_AUDIO_MIXER))
#define GST_AUDIO_MIXER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj) ,GST_TYPE_AUDIO_MIXER,GstAudioMixerClass))

typedef struct _GstAudioMixer             GstAudioMixer;
typedef struct _GstAudioMixerClass        GstAudioMixerClass;

typedef struct _GstAudioMixerPad GstAudioMixerPad;
typedef struct _GstAudioMixerPadClass GstAudioMixerPadClass;

/**
 * GstAudioMixer:
 *
 * The audiomixer object structure.
 */
struct _GstAudioMixer {
  GstAggregator      element;

  /* the next are valid for both int and float */
  GstAudioInfo    info;

  /* counters to keep track of timestamps */
  gint64          offset;
  /* Buffer starting at offset containing block_size samples */
  GstBuffer      *current_buffer;

  /* current caps */
  GstCaps *current_caps;
  gboolean send_caps;

  /* target caps (set via property) */
  GstCaps *filter_caps;

  GstClockTime alignment_threshold;
  GstClockTime discont_wait;

  gint64          base_time;

  /* Duration of every output buffer */
  GstClockTime output_buffer_duration;
};

struct _GstAudioMixerClass {
  GstAggregatorClass parent_class;
};

GType    gst_audiomixer_get_type (void);

#define GST_TYPE_AUDIO_MIXER_PAD            (gst_audiomixer_pad_get_type())
#define GST_AUDIO_MIXER_PAD(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_AUDIO_MIXER_PAD,GstAudioMixerPad))
#define GST_IS_AUDIO_MIXER_PAD(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_AUDIO_MIXER_PAD))
#define GST_AUDIO_MIXER_PAD_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass) ,GST_TYPE_AUDIO_MIXER_PAD,GstAudioMixerPadClass))
#define GST_IS_AUDIO_MIXER_PAD_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass) ,GST_TYPE_AUDIO_MIXER_PAD))
#define GST_AUDIO_MIXER_PAD_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj) ,GST_TYPE_AUDIO_MIXER_PAD,GstAudioMixerPadClass))

struct _GstAudioMixerPad {
  GstAggregatorPad parent;

  gdouble volume;
  gint volume_i32;
  gint volume_i16;
  gint volume_i8;
  gboolean mute;

  /* < private > */
  GstBuffer *buffer;            /* current buffer we're mixing,
                                   for comparison with collect.buffer
                                   to see if we need to update our
                                   cached values. */
  guint position, size;

  guint64 output_offset;        /* Offset in output segment that
                                   collect.pos refers to in the
                                   current buffer. */

  guint64 next_offset;          /* Next expected offset in the input segment */

  /* Last time we noticed a discont */
  GstClockTime discont_time;
};

struct _GstAudioMixerPadClass {
  GstAggregatorPadClass parent_class;
};

GType gst_audiomixer_pad_get_type (void);

G_END_DECLS


#endif /* __GST_AUDIO_MIXER_H__ */
