/*
   IO - Note Encryptor | io_fileselect.c
   ------------------------------------------------------------------------------
   p34c3w41k3r <p34c3w41k3r@protonmail.ch>

   Copyright 2017 - Philentropy

   IO's is free software: you can redistribute it
   and/or modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 2s of the License,
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
#include "io_encryptutils.c"
#define CHAR_BUF 2000

// Main gtk window
GtkWidget *window;
// Textview buffer
GtkTextBuffer *buf;
char *filename;


// Load the file in the UTF-8 format else error
static void load_file(char *fname, char fname_enc[])
{
  FILE *f;
  //char *fname_encr = NULL;
  // Iterator is an object that represents positions between two characters in a buffer
  GtkTextIter p;

  char fbuf[CHAR_BUF];
  // Read the contents of file in size_t l
  size_t l;

  if(!fname)
    {
      // Create file selection dialog
      GtkWidget *dialog = gtk_file_selection_new("Open Note...");
      int resp = gtk_dialog_run(GTK_DIALOG(dialog));

      if(resp == GTK_RESPONSE_OK)
        {
	  // fname contains the file selected by the user and show the file selection dialog
	  fname = g_strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(dialog)));

	  if(decrypt_note(fname) == 1)
	    {
	      g_error("IO cannot decrypt note.");
	      return;
	    }
	  gtk_widget_destroy(dialog);
	}
      else
        {
	  // cancel
	  gtk_widget_destroy(dialog);
	  return;
	}
    }

  // If file cannot be opened
  if(!(f = fopen(fname_enc, "r")))
    {
      g_printerr("%s: %s\n", fname_enc, g_strerror(errno));
      return;
    }

  if(fname_enc != filename)
    {
      gchar *wt = g_strdup_printf("IO Note Encryptor (%s)", fname_enc);
      //g_free(filename);
      filename = fname_enc;
      // Set titlebar as the file name
      gtk_window_set_title(GTK_WINDOW(window), wt);
      g_free(wt);
    }

  // Put the ending iterator in p of buffer
  gtk_text_buffer_get_end_iter(buf, &p);

  while((l = fread(fbuf, 1, sizeof(fbuf), f)) > 0)
    {
      // Only text files in Utf-8 format can be opened
      GError *err = NULL;
      gsize br, bw;
      gchar *text;

      if(!(text = g_locale_to_utf8(fbuf, l, &br, &bw, &err)))
	{
	  g_printerr("File is not in UTF-8 format : %s\n", err->message);
	  g_clear_error(&err);
	  // If an invalid file is opened enter the file name as NULL
	  filename = NULL;
	  gtk_window_set_title(GTK_WINDOW(window), "Text Editor (Insert file name)");
	  fclose(f);
	  return;
	}

      gtk_text_buffer_insert(buf, &p, text, bw);
      g_free(text);
    }

  gtk_text_buffer_set_modified(buf, FALSE);
  gtk_text_buffer_get_start_iter(buf, &p);
  gtk_text_buffer_place_cursor(buf, &p);

  if(ferror(f))
    {
      g_printerr("%s: %s\n", fname_enc, g_strerror(errno));
      fclose(f);
      return;
    }

  if(fclose(f) == EOF)
    g_printerr("%s: %s\n", fname_enc, g_strerror(errno));


  if(remove(fname_enc) == -1)
    {
      perror("remove:");
      return;
    }
  else
    {
      printf("\033[32m[INFO]\033[0m: The uncrypted note file is correctly deleted.\n");
    }
  fname = NULL;
}

// Save file dialog box
static gboolean save_file(char *fname)
{
  FILE *f;
  int ok = TRUE;
  int fsize = 0;
  //int extloc = 0;

  if(!fname)
    {
      GtkWidget *dialog = gtk_file_selection_new("Save and encrypt note as...");
      int resp = gtk_dialog_run(GTK_DIALOG(dialog));

      if(resp == GTK_RESPONSE_OK)
	{
	  fname = g_strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(dialog)));
	  gtk_widget_destroy(dialog);
	}
      else
	{
	  gtk_widget_destroy(dialog);
	  return FALSE;
	}
    }

  // Error opening file to write
  if(!(f = fopen(fname, "w")))
    {
      g_printerr("%s: %s\n", fname, g_strerror(errno));
      ok = FALSE;
    }
  else
    {
      GtkTextIter start, end, p;

      // Get the starting and ending position

      gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buf), &start, &end);
      p = start;

      while(!gtk_text_iter_equal(&start, &end))
	{
	  gchar *buf, *fbuf;
	  gsize br, bw;
	  GError *err = NULL;

	  gtk_text_iter_forward_chars(&p, CHAR_BUF);
	  buf = gtk_text_iter_get_slice(&start, &p);
	  fbuf = g_locale_from_utf8(buf, -1, &br, &bw, &err);
	  g_free(buf);

	  if(!fbuf)
	    {
	      g_printerr("Failed UTF-8 to locale conversion: %s\n", err->message);
	      g_clear_error(&err);
	      ok = FALSE;
	      break;
	    }

	  fwrite(fbuf, bw, 1, f);
	  g_free(fbuf);

	  if(ferror(f))
	    {
	      g_printerr("%s: %s\n", fname, g_strerror(errno));
	      ok = FALSE;
	      break;
	    }
	  start = p;
	}
      if(fclose(f) == EOF)
	{
	  g_printerr("%s: %s\n", fname, g_strerror(errno));
	  ok = FALSE;
	}
    }

  if(ok)
    {
      gtk_text_buffer_set_modified(buf, FALSE);
      if(fname != filename)
	{
	  gchar *wt = g_strdup_printf("IO Note Encryptor (%s)", fname);
    //g_free(filename);
	  filename = fname;
	  gtk_window_set_title(GTK_WINDOW(window), wt);
	  g_free(wt);

	}
      //DOIS ETRE ICI QUE LON MODIFIE LE NOM DU FICHIER POUR EVITER LES EXTENSIONS A RALONGE
      printf("FNAME FOR ENCRYPT IS %s\n", fname);
      if(strchr(fname, '.') != NULL)
      {
        fsize = strlen(fname);
        printf("extension find for %s and lenght of file name is %d\n", fname, fsize);
      }
      else
      {
        printf("extension not found for %s.\n", fname);
      }
      if(encrypt_note(fname) == 1)
	{
	  puts("Error encrypting note.\n");

	  return 1;
	}
    }
  return ok;
}

/*
   If the buffer  is modified then prompt the user to save if he
   quits the application or loads a new file
*/
static gboolean save_if_modified(void)
{
  int resp;
  GtkWidget *dialog;

  if(!gtk_text_buffer_get_modified(GTK_TEXT_BUFFER(buf)))
    {
      return TRUE;
    }

  dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0, GTK_MESSAGE_QUESTION,
				  GTK_BUTTONS_NONE,
				  "Modified note. Do you want to save it now?");
  gtk_dialog_add_buttons(GTK_DIALOG(dialog), GTK_STOCK_YES, GTK_RESPONSE_YES,
			 GTK_STOCK_NO, GTK_RESPONSE_NO,
			 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
  resp = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

  switch(resp)
    {
    case GTK_RESPONSE_YES:
      return save_file(filename);
    case GTK_RESPONSE_NO:
      return TRUE;
    case GTK_RESPONSE_CANCEL:
    case GTK_RESPONSE_DELETE_EVENT:
      return FALSE;
    default:
      g_printerr("Error response %d from dialog\n", resp);
      return FALSE;
    }
}
