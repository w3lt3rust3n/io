/*
   IO - Note Encryptor | io_menu.c
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

#include <glib.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
// Include source files
#include "io_extra.c"
#include "io_find.c"
#include "io_findreplace.c"

// Prototypes
void show_help(void);
void show_about(void);
// TextView
GtkWidget *view;
// Used for selecting the text
GdkAtom sel_atom = GDK_SELECTION_CLIPBOARD;
// identify the requests menu handler will manage.
GtkItemFactory *main_menu;
// Used for font color.
GtkWidget *colorseldlg = NULL;
GtkWidget *drawingarea = NULL;

// Item factory creates a menu from array of itemfactory entries
#define MENU_NEW 1
#define MENU_OPEN 2
#define MENU_SAVE 3
#define MENU_SAVE_AS 4
#define MENU_QUIT 5
#define MENU_CUT 6
#define MENU_COPY 7
#define MENU_PASTE 8
#define MENU_USE_CLIPBOARD 9
#define MENU_SELECT_ALL 10
#define MENU_FIND 11
#define MENU_REPLACE 12
#define MENU_HELP 14
#define MENU_ABOUT 15


static void menu_show(gpointer data, guint action, GtkWidget *widget)
{
  GtkTextIter p;

  switch(action)
    {
    case MENU_NEW:
      // Call save if modified wen user opens a new file
      if(save_if_modified())
	{
	  // Get all the current tag table n put them in the new buffer
	  buf = gtk_text_buffer_new(gtk_text_buffer_get_tag_table(buf));
	  gtk_text_view_set_buffer(GTK_TEXT_VIEW(view), buf);
	  g_object_unref(G_OBJECT(buf));
	  // Needed for freeing memory by the buffer wen a new buffer is created
	}
      break;
    case MENU_OPEN:
      if(save_if_modified())
	{
	  // Call save if modified wen user opens a new file
	  buf = gtk_text_buffer_new(gtk_text_buffer_get_tag_table(buf));
	  gtk_text_view_set_buffer(GTK_TEXT_VIEW(view), buf);

	  // Needed for freeing memory by the buffer wen a new buffer is created
	  g_object_unref(G_OBJECT(buf));
	  load_file(NULL, fname_enc);
	}
      break;

    case MENU_SAVE:
      save_file(filename);
      break;

    case MENU_SAVE_AS:
      save_file(NULL);
      break;

    case MENU_QUIT:
      // Call save if modified wen user opens a new file
      if(save_if_modified())
	gtk_widget_destroy(window);
      break;

    case MENU_CUT:
      gtk_text_buffer_cut_clipboard(buf,gtk_clipboard_get(sel_atom), TRUE);
      break;

    case MENU_COPY:
      gtk_text_buffer_copy_clipboard(buf,gtk_clipboard_get(sel_atom));
      break;

    case MENU_PASTE:
      // If null text is inserted at the current cursor position
      gtk_text_buffer_paste_clipboard(buf,gtk_clipboard_get(sel_atom), NULL, TRUE);
      break;

    case MENU_FIND:
      textfind();
      break;

    case MENU_REPLACE:
      text_find_replace();
      break;

    case MENU_SELECT_ALL:
      // Get the starting pt of the buffer
      gtk_text_buffer_get_start_iter(buf, &p);
      // Ignore the selection made by the mouse
      gtk_text_buffer_place_cursor(buf, &p);
      // Get the ending pt of the buffer
      gtk_text_buffer_get_end_iter(buf, &p);
      gtk_text_buffer_move_mark_by_name(buf, "selection_bound", &p);
      break;

    case MENU_HELP:
      show_help();
      break;

    case MENU_ABOUT:
      show_about();
      break;

    default:
      // Error checking
      g_printerr("Menu action not defined : %u\n", action);
      break;
    }
}

void select_font(GtkWidget *widget)
{
  GtkResponseType result;
  GtkWidget *dialog = gtk_font_selection_dialog_new("Select Font");

  result = gtk_dialog_run(GTK_DIALOG(dialog));

  if(result == GTK_RESPONSE_OK || result == GTK_RESPONSE_APPLY)
    {
      PangoFontDescription *font_desc;
      gchar *fontname = gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(dialog));

      font_desc = pango_font_description_from_string(fontname);
      gtk_widget_modify_font(GTK_WIDGET(view), font_desc);
      g_free(fontname);
    }
  gtk_widget_destroy(dialog);
}

// Contains menu entries and stuff.
GtkItemFactoryEntry menu_def[] =
  {
    { (char *)"/_File",            NULL,                  NULL,        0,                     (char *)"<Branch>",    NULL },
  //  { (char *)"/Note/_New",        (char *)"<control>N",  menu_show,   MENU_NEW,              (char *)"<StockItem>", GTK_STOCK_NEW },
  //  { (char *)"/File/_Open...",    (char *)"<control>O",  menu_show,   MENU_OPEN,             (char *)"<StockItem>", GTK_STOCK_OPEN },
    //{ (char *)"/File/_Save",       (char *)"<control>S",  menu_show,   MENU_SAVE,             (char *)"<StockItem>", GTK_STOCK_SAVE },
    { (char *)"/File/Save _As...", NULL,                  menu_show,   MENU_SAVE_AS,          (char *)"<StockItem>", GTK_STOCK_SAVE_AS },
    { (char *)"/File/sep",         NULL,                  NULL,        0,                     (char *)"<Separator>", NULL },
    { (char *)"/File/_Close editor",       (char *)"<control>Q",  menu_show,   MENU_QUIT,             (char *)"<StockItem>", GTK_STOCK_QUIT },
    { (char *)"/_Edit",            NULL,                  NULL,        0,                     (char *)"<Branch>",    NULL },
    { (char *)"/Edit/C_ut",        (char *)"<control>X",  menu_show,   MENU_CUT,              (char *)"<StockItem>", GTK_STOCK_CUT },
    { (char *)"/Edit/_Copy",       (char *)"<control>C",  menu_show,   MENU_COPY,             (char *)"<StockItem>", GTK_STOCK_COPY },
    { (char *)"/Edit/_Paste",      (char *)"<control>V",  menu_show,   MENU_PASTE,            (char *)"<StockItem>", GTK_STOCK_PASTE },
    { (char *)"/Edit/sep",         NULL,                  NULL,        0,                     (char *)"<Separator>", NULL },
    { (char *)"/Edit/Select All",  NULL,                  menu_show,   MENU_SELECT_ALL,       NULL,                  NULL },
    { (char *)"/_Fonts",           NULL,                  select_font, 0,                     NULL,                  NULL },
    { (char *)"/_Search",          NULL,                  NULL,        0,                     (char *)"<Branch>",    NULL },
    { (char *)"/Search/_Find",     (char *)"<control>F" , menu_show,   MENU_FIND,             (char *)"<StockItem>", GTK_STOCK_FIND },
    { (char *)"/Search/_Replace",  (char *)"<control>R" , menu_show,   MENU_REPLACE,          (char *)"<StockItem>", GTK_STOCK_FIND },
    { (char *)"/_Help",            NULL,                  NULL,        0,                     (char *)"<Branch>",    NULL },
    { (char *)"/Help/_Contents",   (char *)"<control>H",  menu_show,   MENU_HELP,             (char *)"<StockItem>", GTK_STOCK_HELP },
    { (char *)"/Help/_About",      NULL ,                 menu_show,   MENU_ABOUT,            "<Item>" },
  };
