/*
   IO - Note Encryptor | io_find.c
   ------------------------------------------------------------------------------
   p34c3w41k3r <p34c3w41k3r@protonmail.ch>

   Copyright 2017 - Philentropy

   IO's is free software: you can redistribute it
   and/or modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 2 of the License,
   or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ------------------------------------------------------------------------------
*/

#include <gtk/gtk.h>

extern GtkWidget *view;
GtkWidget *text_view;
GtkWidget *search_entry;


void find(GtkTextView *text_view, const gchar *text, GtkTextIter *iter)
{
  GtkTextIter mstart, mend;
  GtkTextBuffer *buffer;
  gboolean found;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  found = gtk_text_iter_forward_search(iter, text, 0, &mstart, &mend, NULL);

  if(found)
    {
      gtk_text_buffer_select_range(buffer, &mstart, &mend);
      gtk_text_buffer_create_mark(buffer, "last_pos", &mend, FALSE);
    }
}

void win_destroy (void)
{
  gtk_main_quit();
}

void next_button_clicked (GtkWidget *next_button)
{
  //  const gchar *text;
  GtkTextBuffer *buffer;
  GtkTextMark *last_pos;
  GtkTextIter iter;

  //text = gtk_entry_get_text(GTK_ENTRY (search_entry));
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  last_pos = gtk_text_buffer_get_mark (buffer, "last_pos");

  if(last_pos == NULL)
    {
      GtkWidget *window1;
      GtkWidget *label;

      window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      gtk_window_set_title(GTK_WINDOW(window1), "Search");
      gtk_window_set_default_size(GTK_WINDOW(window1), 50, 300);

      label = gtk_label_new("End of Search !\n\n");
      gtk_container_add(GTK_CONTAINER (window1), label);
      gtk_widget_show_all(window1);

      return;
    }
  gtk_text_buffer_get_iter_at_mark(buffer, &iter, last_pos);
  find(GTK_TEXT_VIEW(text_view), (gchar *)gtk_entry_get_text((GtkEntry *)search_entry), &iter);
}

void search_button_clicked (GtkWidget *search_button)
{
  GtkTextBuffer *buffer;
  GtkTextIter iter;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  gtk_text_buffer_get_start_iter(buffer, &iter);
  find(GTK_TEXT_VIEW(text_view), (gchar *)gtk_entry_get_text((GtkEntry *)search_entry), &iter);
}

void textfind(void)
{
  GtkWidget *win;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *search_button;
  GtkWidget *next_button;

  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  vbox = gtk_vbox_new(FALSE, 2);
  gtk_container_add(GTK_CONTAINER(win), vbox);

  hbox = gtk_hbox_new(FALSE, 2);

  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

  text_view = view;
  search_entry = gtk_entry_new();

  gtk_box_pack_start(GTK_BOX(hbox), search_entry, TRUE, TRUE, 0);

  search_button = gtk_button_new_with_label("Search");
  gtk_box_pack_start(GTK_BOX(hbox), search_button, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(search_button), "clicked", G_CALLBACK(search_button_clicked), NULL);

  next_button = gtk_button_new_with_label("Next");
  gtk_box_pack_start(GTK_BOX(hbox), next_button, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(next_button), "clicked", G_CALLBACK(next_button_clicked), NULL);

  gtk_widget_show_all(win);
}
