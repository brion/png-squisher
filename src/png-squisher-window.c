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
  GtkProgressBar      *progressor;
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
  gtk_widget_class_bind_template_child (widget_class, PngSquisherWindow, progressor);
  gtk_widget_class_bind_template_child (widget_class, PngSquisherWindow, displayer);
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
      printf("YEAH %s\n", filename);

      // @todo open file and load it in the displayer

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
  printf("BYEEEE\n");
}

static void
png_squisher_window_init (PngSquisherWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
  g_signal_connect(self->opener, "clicked", (GCallback)on_opener_clicked, (void *)self);
  g_signal_connect(self->saver, "clicked", (GCallback)on_saver_clicked, (void *)self);
  gtk_widget_set_state_flags(GTK_WIDGET(self->saver), GTK_STATE_FLAG_INSENSITIVE, TRUE);
}

