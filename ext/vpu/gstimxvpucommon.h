/* gstreamer-imx: GStreamer plugins for the i.MX SoCs
 * Copyright (C) 2019  Carlos Rafael Giani
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
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef GST_IMX_VPU_COMMON_H
#define GST_IMX_VPU_COMMON_H

#include <gst/gst.h>
#include <gst/video/video.h>
#include <imxvpuapi2/imxvpuapi2.h>


/* These are common internal functions for the GStreamer i.MX VPU elements. */


G_BEGIN_DECLS



/* Functionality for autogenerating GStreamer element entries in the
 * GStreamer registry out of the list of supported codecs.
 *
 * This is used for both en- and decoder elements, though in different ways.
 *
 * Decoders:
 *
 * There is one decoder element for each supported compression format. However,
 * there is no source file for each format. Instead, these elements are
 * autogenerated, using GstImxVpuDec as the base class. The information stored
 * in GstImxVpuCodecDetails is used for that purpose. There is one global
 * static table that contains one GstImxVpuCodecDetails entry for each
 * compression format. A GType is generated out of these entries, and using
 * that (and an autogenerated element name), a new element is registered.
 * This is done by gst_imx_vpu_dec_register_decoder_type().
 *
 * Encoders:
 *
 * Unlike with decoders, there _is_ one source file for each format.
 * This is because encoders sometimes have additional, format specific
 * parameters that can then be adjusted through extra GObject properties.
 * Still, GstImxVpuCodecDetails is used by the GstImxVpuEnc base class for
 * setting up element metadata etc.
 */

typedef struct _GstImxVpuCodecDetails GstImxVpuCodecDetails;

struct _GstImxVpuCodecDetails
{
	/* Format specific element name suffix. This is used for generating
	 * element names that reflect the type the elements can handle. For
	 * example, decoder element names are "imxvpudec_<element_name_suffix>". */
	gchar const *element_name_suffix;
	/* GLib class name suffix. For example, for h.264, this would be
	 * "H264". This is useful for constructing the GLib class name on the
	 * fly. Example: GstImxVpuDecH264 */
	gchar const *class_name_suffix;
	/* Format specific description. Useful for element class metadata. */
	gchar const *desc_name;
	/* The rank the GStreamer element handling this format should have. */
	guint rank;
	/* The compression format the element handles. */
	ImxVpuApiCompressionFormat compression_format;
	/* If TRUE, then out-of-band codec data is required for decoding.
	 * Unused in encoders. */
	gboolean requires_codec_data;
};


/* Helper macro to access the libimxvpuapi compression format that is stored
 * inside a GObject class. */
#define GST_IMX_VPU_GET_ELEMENT_COMPRESSION_FORMAT(obj) \
	((ImxVpuApiCompressionFormat)g_type_get_qdata(G_OBJECT_CLASS_TYPE(GST_OBJECT_GET_CLASS(obj)), gst_imx_vpu_compression_format_quark()))

/* GQuark for accessing the libimxvpuapi compression format in en/decoder classes. */
GQuark gst_imx_vpu_compression_format_quark(void);

/* Get this format's GstImxVpuCodecDetails from the static internal global table. */
GstImxVpuCodecDetails const * gst_imx_vpu_get_codec_details(ImxVpuApiCompressionFormat compression_format);



/* Miscellaneous functions. */

gboolean gst_imx_vpu_get_caps_for_format(ImxVpuApiCompressionFormat compression_format, ImxVpuApiCompressionFormatSupportDetails const *details, GstCaps **encoded_caps, GstCaps **raw_caps, gboolean for_encoder);

guint gst_imx_vpu_get_default_quantization(ImxVpuApiCompressionFormatSupportDetails const *details);

gboolean gst_imx_vpu_color_format_to_gstvidfmt(GstVideoFormat *gst_video_format, ImxVpuApiColorFormat imxvpuapi_format);
gboolean gst_imx_vpu_color_format_from_gstvidfmt(ImxVpuApiColorFormat *imxvpuapi_format, GstVideoFormat gst_video_format);

gboolean gst_imx_vpu_color_format_is_semi_planar(GstVideoFormat gst_video_format);
gboolean gst_imx_vpu_color_format_has_10bit(GstVideoFormat gst_video_format);

gchar const * gst_imx_vpu_get_string_from_structure_field(GstStructure *s, gchar const *field_name);

void gst_imx_vpu_api_setup_logging(void);



G_END_DECLS


#endif /* GST_IMX_VPU_COMMON_H */
