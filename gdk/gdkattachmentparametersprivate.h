#ifndef __GDK_ATTACHMENT_PARAMETERS_PRIVATE_H__
#define __GDK_ATTACHMENT_PARAMETERS_PRIVATE_H__

#include "gdkattachmentparameters.h"

G_BEGIN_DECLS

struct _GdkAttachmentParameters
{
  GdkPoint attachment_origin;

  gboolean has_attachment_rectangle;
  GdkRectangle attachment_rectangle;

  GdkAttachmentBorder attachment_margin;
  GdkAttachmentBorder window_margin;
  GdkAttachmentBorder window_padding;

  gboolean is_right_to_left;

  GList *primary_options;
  GList *secondary_options;

  GdkAttachmentPositionCallback position_callback;
  gpointer position_callback_user_data;
  GDestroyNotify position_callback_destroy_notify;
};

G_END_DECLS

#endif /* __GDK_ATTACHMENT_PARAMETERS_PRIVATE_H__ */
