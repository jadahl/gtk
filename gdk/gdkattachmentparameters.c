#include "config.h"

#include "gdkattachmentparameters.h"
#include "gdkattachmentparametersprivate.h"

GdkAttachmentParameters *
gdk_attachment_parameters_new (void)
{
  return g_new0 (GdkAttachmentParameters, 1);
}

gpointer
gdk_attachment_parameters_copy (gconstpointer src,
                                gpointer      data)
{
  GdkAttachmentParameters *copy;
  const GdkAttachmentParameters *parameters;

  if (!src)
    return NULL;

  parameters = src;

  copy = g_memdup (parameters, sizeof (*parameters));

  copy->primary_options = g_list_copy (parameters->primary_options);
  copy->secondary_options = g_list_copy (parameters->secondary_options);

  return copy;
}

void
gdk_attachment_parameters_free (gpointer data)
{
  GdkAttachmentParameters *parameters;

  if (!data)
    return;

  parameters = data;

  if (parameters->position_callback_user_data && parameters->position_callback_destroy_notify)
    parameters->position_callback_destroy_notify (parameters->position_callback_user_data);

  g_list_free (parameters->secondary_options);
  g_list_free (parameters->primary_options);

  g_free (parameters);
}

void
gdk_attachment_parameters_set_attachment_origin (GdkAttachmentParameters *parameters,
                                                 const GdkPoint          *origin)
{
  GdkPoint zero = { 0 };

  g_return_if_fail (parameters);

  parameters->attachment_origin = origin ? *origin : zero;
}

void
gdk_attachment_parameters_set_attachment_rectangle (GdkAttachmentParameters *parameters,
                                                    const GdkRectangle      *rectangle)
{
  g_return_if_fail (parameters);

  if (rectangle)
    {
      parameters->has_attachment_rectangle = TRUE;
      parameters->attachment_rectangle = *rectangle;
    }
  else
    parameters->has_attachment_rectangle = FALSE;
}

void
gdk_attachment_parameters_set_attachment_margin (GdkAttachmentParameters   *parameters,
                                                 const GdkAttachmentBorder *margin)
{
  GdkAttachmentBorder zero = { 0 };

  g_return_if_fail (parameters);

  parameters->attachment_margin = margin ? *margin : zero;
}

void
gdk_attachment_parameters_set_window_margin (GdkAttachmentParameters   *parameters,
                                             const GdkAttachmentBorder *margin)
{
  GdkAttachmentBorder zero = { 0 };

  g_return_if_fail (parameters);

  parameters->window_margin = margin ? *margin : zero;
}

void
gdk_attachment_parameters_set_window_padding (GdkAttachmentParameters   *parameters,
                                              const GdkAttachmentBorder *padding)
{
  GdkAttachmentBorder zero = { 0 };

  g_return_if_fail (parameters);

  parameters->window_padding = padding ? *padding : zero;
}

void
gdk_attachment_parameters_set_right_to_left (GdkAttachmentParameters *parameters,
                                             gboolean                 is_right_to_left)
{
  g_return_if_fail (parameters);

  parameters->is_right_to_left = is_right_to_left;
}

static void
add_options (GList               **list,
             GdkAttachmentOption   first_option,
             va_list               args)
{
  GdkAttachmentOption option;

  for (option = first_option; option; option = va_arg (args, GdkAttachmentOption))
    *list = g_list_append (*list, GINT_TO_POINTER (option));
}

void
gdk_attachment_parameters_add_primary_options (GdkAttachmentParameters *parameters,
                                               GdkAttachmentOption      first_option,
                                               ...)
{
  va_list args;

  g_return_if_fail (parameters);

  va_start (args, first_option);

  add_options (&parameters->primary_options, first_option, args);

  va_end (args);
}

void
gdk_attachment_parameters_add_secondary_options (GdkAttachmentParameters *parameters,
                                                 GdkAttachmentOption      first_option,
                                                 ...)
{
  va_list args;

  g_return_if_fail (parameters);

  va_start (args, first_option);

  add_options (&parameters->secondary_options, first_option, args);

  va_end (args);
}

void
gdk_attachment_parameters_set_position_callback (GdkAttachmentParameters       *parameters,
                                                 GdkAttachmentPositionCallback  callback,
                                                 gpointer                       user_data,
                                                 GDestroyNotify                 destroy_notify)
{
  g_return_if_fail (parameters);

  parameters->position_callback = callback;

  if (user_data != parameters->position_callback_user_data)
    {
      if (parameters->position_callback_user_data && parameters->position_callback_destroy_notify)
        parameters->position_callback_destroy_notify (parameters->position_callback_user_data);

      parameters->position_callback_user_data = user_data;
      parameters->position_callback_destroy_notify = destroy_notify;
    }
  else if (user_data)
    g_warning ("%s (): parameters already owns user data", G_STRFUNC);
}
