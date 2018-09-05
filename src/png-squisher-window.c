/* png-squisher-window.c
 *
 * Copyright 2018 Brion Vibber
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the sale,
 * use or other dealings in this Software without prior written
 * authorization.
 */

#include "png-squisher-config.h"
#include "png-squisher-window.h"

struct _PngSquisherWindow
{
  GtkApplicationWindow  parent_instance;

  /* Template widgets */
  GtkHeaderBar        *header_bar;
  GtkButton           *opener;
  GtkButton           *saver;
  GtkLabel            *statuser;
  GtkImage            *displayer;
};

G_DEFINE_TYPE (PngSquisherWindow, png_squisher_window, GTK_TYPE_APPLICATION_WINDOW)

static void
png_squisher_window_class_init (PngSquisherWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/Png-Squisher/png-squisher-window.ui");
  gtk_widget_class_bind_template_child (widget_class, PngSquisherWindow, header_bar);
  gtk_widget_class_bind_template_child (widget_class, PngSquisherWindow, opener);
  gtk_widget_class_bind_template_child (widget_class, PngSquisherWindow, saver);
  gtk_widget_class_bind_template_child (widget_class, PngSquisherWindow, statuser);
  gtk_widget_class_bind_template_child (widget_class, PngSquisherWindow, displayer);
}

static void
on_size_allocate (GtkWidget         *sender,
                  GdkRectangle      *allocation,
                  PngSquisherWindow *self)
{
  // hackkkkkk
  //self->scroller
}

static void
on_opener_clicked (GtkButton *sender, PngSquisherWindow *self)
{
  GtkWidget *chooser = gtk_file_chooser_dialog_new ("Open image file",
                                                    GTK_WINDOW (self),
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    GTK_STOCK_CANCEL,
                                                    GTK_RESPONSE_CANCEL,
                                                    GTK_STOCK_OPEN,
                                                    GTK_RESPONSE_ACCEPT,
                                                    NULL);
  gint res = gtk_dialog_run (GTK_DIALOG (chooser));
  if (res == GTK_RESPONSE_ACCEPT)
    {
      char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (chooser));

      gtk_image_set_from_file (GTK_IMAGE (self->displayer), filename);
      
      // Enable the save button
      gtk_widget_set_state_flags (GTK_WIDGET (self->saver), GTK_STATE_FLAG_NORMAL, TRUE);

      g_free (filename);
    }

  gtk_widget_destroy (chooser);
}

static void
on_saver_clicked (GtkButton *sender, PngSquisherWindow *self)
{
  GtkWidget *chooser = gtk_file_chooser_dialog_new ("Save PNG file",
                                                    GTK_WINDOW (self),
                                                    GTK_FILE_CHOOSER_ACTION_SAVE,
                                                    GTK_STOCK_CANCEL,
                                                    GTK_RESPONSE_CANCEL,
                                                    GTK_STOCK_SAVE,
                                                    GTK_RESPONSE_ACCEPT,
                                                    NULL);
  gint res = gtk_dialog_run (GTK_DIALOG (chooser));
  if (res == GTK_RESPONSE_ACCEPT)
    {
      char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (chooser));

      // Warning: we don't own a reference here. Don't free it!
      GdkPixbuf *pixbuf = gtk_image_get_pixbuf (self->displayer);

      GDateTime *now = g_date_time_new_now_local();
      char *creation_time = g_date_time_format (now, "%c");

      if (gdk_pixbuf_save (pixbuf, filename, "png", NULL,
                           "tEXt::Software", "png-squisher",
                           "tEXt::Creation Time", creation_time,
                           NULL))
        {
          GDateTime *then = g_date_time_new_now_local ();
          GTimeSpan delta_us = g_date_time_difference (then, now);
          long delta_ms = delta_us / 1000;

          gchar *delta_label = g_strdup_printf ("%ld ms", delta_ms);
          gtk_label_set_label (self->statuser, delta_label);
          g_free(delta_label);

          printf("HEY40\n");
          g_date_time_unref (then);
        }
      else
        {
          gchar *err_label = g_strdup_printf ("Error saving %s", filename);
          gtk_label_set_label (self->statuser, err_label);
          g_free(err_label);
        }

      g_free (creation_time);

      g_date_time_unref (now);

      g_free (filename);
    }

  gtk_widget_destroy (chooser);
}

static void
png_squisher_window_init (PngSquisherWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  g_signal_connect(self, "size_allocate", (GCallback)on_size_allocate, (void *)self);
  g_signal_connect(self->opener, "clicked", (GCallback)on_opener_clicked, (void *)self);
  g_signal_connect(self->saver, "clicked", (GCallback)on_saver_clicked, (void *)self);

  gtk_widget_set_state_flags(GTK_WIDGET(self->saver), GTK_STATE_FLAG_INSENSITIVE, TRUE);
}

