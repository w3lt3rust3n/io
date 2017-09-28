/*
   IO - Note Encryptor | io_extra.c
   ------------------------------------------------------------------------------
   p34c3w41k3r <p34c3w41k3r@protonmail.ch>

   Copyright 2017 - Philentropy

   IO's is free software: you can redistribute it
   and/or modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the License,
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
#include <string.h>


void show_about(void)
{
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("/usr/share/io/io_about_logo.png", NULL);
  GtkWidget *dialog = gtk_about_dialog_new();

  //window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(dialog), "About IO ");
  gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 300);
  gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "IO");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "version 0.0.7");
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"(c) Philentropy");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),
     "Take notes and encrypt them easily with IO.");
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),
     "http://www.philentropy.org");
  gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
  g_object_unref(pixbuf), pixbuf = NULL;
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
  //label = gtk_label_new("Take notes and encrypt them.\nVersion 0.1\n\nCopyright 2016 - Philentropy\nThis software is free software. See COPYING\n");
  //  gtk_container_add(GTK_CONTAINER(dialog), label);
  gtk_widget_show_all(dialog);
}

void show_help(void)
{
  GtkWidget *window1;
  GtkWidget *label;
  char help[5000];
  window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (window1), "Help - IO - Write, encrypt and decrypt your notes");
  gtk_window_set_default_size(GTK_WINDOW(window1), 300, 300);

  strcpy(help,"\nTo Create a New file, choose file -> New. The application displays a new file in the text editor.\n\nTo Save a File\n - To save changes in an existing file, choose File -> Save.\n - To save a new file or to save an existing file under a new filename, choose File -> Save As. Enter a name for the file in the Save As dialog, then click Save.\n\nTo Edit text\n\n - To copy the selected text to a buffer, choose Edit -> Copy.\n - To delete the selected text from the file and move the selected text to a buffer, choose Edit -> Cut.\n - To insert the contents of the buffer at the cursor position, choose Edit -> Paste.You must cut or copy text before you can paste text into the file.\n - To select all of the text in the file, choose Edit -> Select All.\n\nTo Find Text\n\n - Choose Search -> Find to display the Find dialog.\n-Type the string that you want to find, in the text box.\n- Click Find to search the file for the first occurence of the string.\n If the editor finds the string, application moves the cursor position to the string, and selects the string.\n- To find the next occurence of the string, click Next\n\nTo Find and Replace Text\n\nTo search a file for a string, and replace the string with an alternative string, perform the following steps:\n\n-Choose Search -> Replace to display the Replace dialog.\n-Type the string that you want to find, in the Search For field.\n-Click Find to search the file for the first occurence of the string.\n  If the editor finds the string, application moves the cursor position to the string, and selects the string.\n- Click Replace to replace the occurences of the string with the text in the Replace with field. \n- To find the next occurence of the string,click Next.");

  strcat(help,"\0");

  label = gtk_label_new (help);
  gtk_container_add(GTK_CONTAINER(window1), label);
  gtk_widget_show_all (window1);
}
