#ifndef __GDK_ATTACHMENT_PARAMETERS_H__
#define __GDK_ATTACHMENT_PARAMETERS_H__

#if !defined (__GDK_H_INSIDE__) && !defined (GDK_COMPILATION)
#error "Only <gdk/gdk.h> can be included directly."
#endif

#include <gdk/gdktypes.h>
#include <gdk/gdkversionmacros.h>

G_BEGIN_DECLS

enum _GdkAttachmentOption
{
  GDK_ATTACHMENT_END_OPTIONS,
  GDK_ATTACHMENT_FORCE_FIRST_OPTION,
  GDK_ATTACHMENT_FORCE_FIRST_OPTION_IF_PRIMARY_FORCED,
  GDK_ATTACHMENT_FORCE_LAST_OPTION,
  GDK_ATTACHMENT_FORCE_LAST_OPTION_IF_PRIMARY_FORCED,
  GDK_ATTACHMENT_ATTACH_TOP_EDGE,
  GDK_ATTACHMENT_ATTACH_LEFT_EDGE,
  GDK_ATTACHMENT_ATTACH_RIGHT_EDGE,
  GDK_ATTACHMENT_ATTACH_BOTTOM_EDGE,
  GDK_ATTACHMENT_ATTACH_FORWARD_EDGE,
  GDK_ATTACHMENT_ATTACH_BACKWARD_EDGE,
  GDK_ATTACHMENT_ALIGN_TOP_EDGES,
  GDK_ATTACHMENT_ALIGN_LEFT_EDGES,
  GDK_ATTACHMENT_ALIGN_RIGHT_EDGES,
  GDK_ATTACHMENT_ALIGN_BOTTOM_EDGES,
  GDK_ATTACHMENT_ALIGN_FORWARD_EDGES,
  GDK_ATTACHMENT_ALIGN_BACKWARD_EDGES,
  GDK_ATTACHMENT_CENTER_HORIZONTALLY,
  GDK_ATTACHMENT_CENTER_VERTICALLY,
  GDK_ATTACHMENT_CENTER_ON_TOP_EDGE,
  GDK_ATTACHMENT_CENTER_ON_LEFT_EDGE,
  GDK_ATTACHMENT_CENTER_ON_RIGHT_EDGE,
  GDK_ATTACHMENT_CENTER_ON_BOTTOM_EDGE,
  GDK_ATTACHMENT_CENTER_ON_FORWARD_EDGE,
  GDK_ATTACHMENT_CENTER_ON_BACKWARD_EDGE,
  GDK_ATTACHMENT_ATTACH_ABOVE_CENTER,
  GDK_ATTACHMENT_ATTACH_BELOW_CENTER,
  GDK_ATTACHMENT_ATTACH_LEFT_OF_CENTER,
  GDK_ATTACHMENT_ATTACH_RIGHT_OF_CENTER,
  GDK_ATTACHMENT_ATTACH_FORWARD_OF_CENTER,
  GDK_ATTACHMENT_ATTACH_BACKWARD_OF_CENTER
};

struct _GdkAttachmentBorder
{
  gint top;
  gint left;
  gint right;
  gint bottom;
};

typedef enum _GdkAttachmentOption GdkAttachmentOption;
typedef struct _GdkAttachmentBorder GdkAttachmentBorder;
typedef struct _GdkAttachmentParameters GdkAttachmentParameters;
typedef void (*GdkAttachmentPositionCallback) (GdkWindow           *window,
                                               GdkAttachmentOption  primary_option,
                                               GdkAttachmentOption  secondary_option,
                                               const GdkPoint      *position,
                                               const GdkPoint      *offset,
                                               gpointer             user_data);

GDK_AVAILABLE_IN_3_18
GdkAttachmentParameters * gdk_attachment_parameters_new                      (void);

GDK_AVAILABLE_IN_3_18
gpointer                  gdk_attachment_parameters_copy                     (gconstpointer                  src,
                                                                              gpointer                       data);

GDK_AVAILABLE_IN_3_18
void                      gdk_attachment_parameters_free                     (gpointer                       data);

GDK_AVAILABLE_IN_3_18
void                      gdk_attachment_parameters_set_attachment_origin    (GdkAttachmentParameters       *parameters,
                                                                              const GdkPoint                *origin);

GDK_AVAILABLE_IN_3_18
void                      gdk_attachment_parameters_set_attachment_rectangle (GdkAttachmentParameters       *parameters,
                                                                              const GdkRectangle            *rectangle);

GDK_AVAILABLE_IN_3_18
void                      gdk_attachment_parameters_set_attachment_margin    (GdkAttachmentParameters       *parameters,
                                                                              const GdkAttachmentBorder     *margin);

GDK_AVAILABLE_IN_3_18
void                      gdk_attachment_parameters_set_window_margin        (GdkAttachmentParameters       *parameters,
                                                                              const GdkAttachmentBorder     *margin);

GDK_AVAILABLE_IN_3_18
void                      gdk_attachment_parameters_set_window_padding       (GdkAttachmentParameters       *parameters,
                                                                              const GdkAttachmentBorder     *padding);

GDK_AVAILABLE_IN_3_18
void                      gdk_attachment_parameters_set_right_to_left        (GdkAttachmentParameters       *parameters,
                                                                              gboolean                       is_right_to_left);

GDK_AVAILABLE_IN_3_18
void                      gdk_attachment_parameters_add_primary_options      (GdkAttachmentParameters       *parameters,
                                                                              GdkAttachmentOption            first_option,
                                                                              ...) G_GNUC_NULL_TERMINATED;

GDK_AVAILABLE_IN_3_18
void                      gdk_attachment_parameters_add_secondary_options    (GdkAttachmentParameters       *parameters,
                                                                              GdkAttachmentOption            first_option,
                                                                              ...) G_GNUC_NULL_TERMINATED;

GDK_AVAILABLE_IN_3_18
void                      gdk_attachment_parameters_set_position_callback    (GdkAttachmentParameters       *parameters,
                                                                              GdkAttachmentPositionCallback  callback,
                                                                              gpointer                       user_data,
                                                                              GDestroyNotify                 destroy_notify);

G_END_DECLS

#endif /* __GDK_ATTACHMENT_PARAMETERS_H__ */
