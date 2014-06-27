#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>
#include <gdk/gdkx.h>
#include "callbacks.h"
#include "nioc.h"

void realize_cb(GtkWidget* widget, nioc_t* nioc)
{
    GdkWindow* window = gtk_widget_get_window(widget);
    guintptr window_handle;

    if (!gdk_window_ensure_native(window))
        g_error("Couldn't create native window needed for GstXOverlay!");

/* Retrieve window handler from GDK */
#if defined(GDK_WINDOWING_WIN32)
    window_handle = (guintptr)GDK_WINDOW_HWND(window);
#elif defined(GDK_WINDOWING_QUARTZ)
    window_handle = gdk_quartz_window_get_nsview(window);
#elif defined(GDK_WINDOWING_X11)
    window_handle = GDK_WINDOW_XID(window);
#endif
    /* Pass it to playbin2, which implements XOverlay and will forward it to the video sink */
    gst_x_overlay_set_window_handle(GST_X_OVERLAY(nioc->media.playbin2), window_handle);
}

/* This function is called when the PLAY button is clicked */
void play_cb(GtkButton* GIRARA_UNUSED(button), nioc_t* nioc)
{
    gst_element_set_state(nioc->media.playbin2, GST_STATE_PLAYING);
}

/* This function is called when the PAUSE button is clicked */
void pause_cb(GtkButton* GIRARA_UNUSED(button), nioc_t* nioc)
{
    gst_element_set_state(nioc->media.playbin2, GST_STATE_PAUSED);
}

/* This function is called when the STOP button is clicked */
void stop_cb(GtkButton* GIRARA_UNUSED(button), nioc_t* nioc)
{
    gst_element_set_state(nioc->media.playbin2, GST_STATE_READY);
}

/* This function is called when the main window is closed */
void delete_event_cb(GtkWidget* GIRARA_UNUSED(widget), GdkEvent* GIRARA_UNUSED(event), nioc_t* nioc)
{
    stop_cb(NULL, nioc);
    gtk_main_quit();
}

/* This function is called everytime the video window needs to be redrawn (due to damage/exposure,
 * rescaling, etc). GStreamer takes care of this in the PAUSED and PLAYING states, otherwise,
 * we simply draw a black rectangle to avoid garbage showing up. */
gboolean draw_cb(GtkWidget* widget, cairo_t *cr, nioc_t* nioc)
{
    if (nioc->media.state < GST_STATE_PAUSED) {
        GtkAllocation allocation;
        GdkWindow* window = gtk_widget_get_window(widget);

        /* Cairo is a 2D graphics library which we use here to clean the video window.
     * It is used by GStreamer for other reasons, so it will always be available to us. */
        gtk_widget_get_allocation(widget, &allocation);
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_rectangle(cr, 0, 0, allocation.width, allocation.height);
        cairo_fill(cr);
    }

    return FALSE;
}
