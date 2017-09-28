/*
   IO - Note Encryptor | io_editor.c
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
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "io_fileselect.c"
#include "io_menu.c"

// Prototypes
static void err_msg(const gchar *msg);
static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
GdkPixbuf *create_pixbuf(const gchar * filename);
void open_file();
void kill_main_menu_io();
void safe_gtk_main_quit();
int main_menu_io();
void editor();

// Text view widget where all the typing is done
GtkWidget *view;

// Gtk - display error message
static void err_msg(const gchar *msg)
{
  GtkWidget *dialog;

  // Pop up window with only OK button
  dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, msg);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  // Destroy the window after ok is pressed
}

/*
   When the user clicks on quit button or quits the app fire the delete event
   signal but first check if the file is modified or not
*/
static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  return !save_if_modified();
}

GdkPixbuf *create_pixbuf(const gchar * filename)
{
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);

   if(!pixbuf)
     {
       fprintf(stderr, "%s\n", error->message);
       g_error_free(error);
     }
   return pixbuf;
}

void open_file()
{
  load_file(NULL, fname_enc);
}

void kill_main_menu_io(GtkWidget *window)
{
  gtk_widget_destroy(window);
}

void safe_gtk_main_quit()
{
  save_if_modified();
}

int main_menu_io()
{
  gtk_rc_parse("/usr/share/io/themes/io-lamia/gtk-2.0/gtkrc");
  GtkWidget *windows;
  GtkWidget *fixed;
  GtkWidget *logo;
  GtkWidget *slog;
  GtkWidget *btn1;
  GtkWidget *btn2;
  GtkWidget *btn3;
  GtkWidget *align;
  GtkWidget *intro;
  GtkWidget *topintro;
  GtkWidget *bottom;
  GtkWidget *license;
  GtkWidget *topalign;
  GtkWidget *halign2;

  GdkPixbuf *icon;

  windows = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(windows), "IO - Note Encryption [main menu]");
  gtk_window_set_default_size(GTK_WINDOW(windows), 550, 320);
  gtk_window_set_position(GTK_WINDOW(windows), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(windows), 5);

  icon = create_pixbuf("/usr/share/io/io_logo.png");
  gtk_window_set_icon(GTK_WINDOW(windows), icon);
  g_signal_connect(G_OBJECT(windows), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_object_unref(icon);

  fixed = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(windows), fixed);

  logo = gtk_image_new_from_file("/usr/share/io/io_about_logo.png");
  gtk_container_add(GTK_CONTAINER(fixed), logo);
  gtk_fixed_put(GTK_FIXED(fixed), logo, 5, 5);
  gtk_widget_set_size_request(logo, 80, 80);

  slog = gtk_image_new_from_file("/usr/share/io/io_desc.png");
  gtk_container_add(GTK_CONTAINER(fixed), slog);
  gtk_fixed_put(GTK_FIXED(fixed), slog, 65, 5);
  gtk_widget_set_size_request(slog, 310, 80);

  align = gtk_alignment_new(0, 1, 0, 0);
  gtk_container_add(GTK_CONTAINER(fixed), align);

  topintro = gtk_label_new("    Getting Start with IO");

  topalign = gtk_alignment_new(0, 1, 0, 1);
  gtk_container_add(GTK_CONTAINER(topalign), topintro);
  //gtk_container_add(GTK_CONTAINER(fixed), topalign);
  gtk_fixed_put(GTK_FIXED(fixed), topalign, 0, 95);

  intro = gtk_label_new("    Thank you to use IO to protect your notes.\n\
    Start the editor and write a new file or open an existing one.\n\n\
    We are taking care about security and we are working hard to\n\
    improve this software. If you have any issues or request, please\n\
    contact the developer team [Go to About].");

  halign2 = gtk_alignment_new(0, 1, 0, 1);
  gtk_container_add(GTK_CONTAINER(halign2), intro);
  //gtk_container_add(GTK_CONTAINER(fixed), halign2);
  gtk_fixed_put(GTK_FIXED(fixed), halign2, 0, 130);

  btn1 = gtk_button_new_with_label("New File");
  gtk_fixed_put(GTK_FIXED(fixed), btn1, 430, 100);
  gtk_widget_set_size_request(btn1, 80, 30);

  btn2 = gtk_button_new_with_label("Open File");
  gtk_fixed_put(GTK_FIXED(fixed), btn2, 430, 160);
  gtk_widget_set_size_request(btn2, 80, 30);

  btn3 = gtk_button_new_with_label("Quit");
  gtk_fixed_put(GTK_FIXED(fixed), btn3, 430, 220);
  gtk_widget_set_size_request(btn3, 80, 30);

  bottom = gtk_alignment_new(0, 1, 0, 0);
  //gtk_container_add(GTK_CONTAINER(fixed), bottom);
  license = gtk_label_new("    Copyright (c) Philentropy");
  gtk_fixed_put(GTK_FIXED(fixed), license, 0, 300);

  license = gtk_alignment_new(1, 0, 0, 0);
  //gtk_container_add(GTK_CONTAINER(fixed), license);
  //gtk_container_add(GTK_CONTAINER(fixed), bottom);
  gtk_fixed_put(GTK_FIXED(fixed), bottom, 0, 300);

  g_signal_connect(G_OBJECT(btn1), "clicked",
    G_CALLBACK(editor), NULL);
  g_signal_connect(G_OBJECT(btn2), "clicked",
    G_CALLBACK(open_file), NULL);
  g_signal_connect(G_OBJECT(btn3), "clicked",
    G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(windows), "destroy",
    G_CALLBACK(safe_gtk_main_quit), NULL);

  gtk_widget_show_all(windows);

  return 0;
}

// main function
void editor()
{
  // TEST
  gtk_rc_parse("/usr/share/io/themes/io-lamia/gtk-2.0/gtkrc");
  //gtk_rc_parse("/usr/share/io/themes/io-default/gtk-2.0/gtkrc");

  GtkWidget *box, *scroll, *widget;
  GtkAccelGroup *accel;
  GdkPixbuf *icon;
  // Gtk initializer can take filename as an argument
  //gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  // If the user quits the app
  g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(delete_event), window);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(kill_main_menu_io), NULL);
  gtk_window_set_title(GTK_WINDOW(window), "IO - Note Encryption [editor]");
  // Window size y & x values
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
  icon = create_pixbuf("/usr/share/io/io_logo.png");
  gtk_window_set_icon(GTK_WINDOW(window), icon);
  //g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_object_unref(icon);

/*
     Create a container where the menus n text widget goes
     false if u dont want extra padding in the box, 0 for spacing
*/
  box = gtk_vbox_new(FALSE, 0);

  // Add the box wid in the window
  gtk_container_add(GTK_CONTAINER(window), box);

  // Create an accerelator for the item factory, keyboard shortcuts
  accel = gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(window), accel);

  // Create a menu above the text view widget
  main_menu = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<TextView>", accel);
  gtk_item_factory_create_items(main_menu,sizeof(menu_def) / sizeof(*menu_def),menu_def, NULL);

  // Return the actual menu bar created by the item factory.
  widget = gtk_item_factory_get_widget(main_menu, "<TextView>");

  // Put the menu in the menu container
  gtk_box_pack_start(GTK_BOX(box), widget, FALSE, FALSE, 0);

  // Put a scroll window widget
  scroll = gtk_scrolled_window_new(NULL, NULL);

  // Put the scroll window in the text container
  gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, 0);

  // Create the text view widget
  view = gtk_text_view_new();

  // Create a buffer for the text view where everything gets stored
  buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

  // Put the text widget in the scroll window
  gtk_container_add(GTK_CONTAINER(scroll), view);

  // Show all widgets in the window
  gtk_widget_show_all(window);

  // Error checking
  g_set_printerr_handler(err_msg);
}

int main(int argc, char *argv[])
{
  gtk_init(&argc, &argv);

  main_menu_io();

  gtk_main();

  return 0;
}
