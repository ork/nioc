#include <girara/types.h>
#include <gtk/gtk.h>
#include <girara/macros.h>
#include "types.h"

void cb_destroy(GtkWidget* widget, nioc_t* nioc);

void realize_cb(GtkWidget* widget, nioc_t* nioc);

/* This function is called when the PLAY button is clicked */
void play_cb(GtkButton* button, nioc_t* nioc);

/* This function is called when the PAUSE button is clicked */
void pause_cb(GtkButton* GtkButton, nioc_t* nioc);

/* This function is called when the STOP button is clicked */
void stop_cb(GtkButton* button, nioc_t* nioc);

/* This function is called when the main window is closed */
void delete_event_cb(GtkWidget* widget, GdkEvent* event, nioc_t* nioc);

/* This function is called everytime the video window needs to be redrawn (due to damage/exposure,
 * rescaling, etc). GStreamer takes care of this in the PAUSED and PLAYING states, otherwise,
 * we simply draw a black rectangle to avoid garbage showing up. */
gboolean draw_cb(GtkWidget* widget, cairo_t* cr, nioc_t* nioc);
