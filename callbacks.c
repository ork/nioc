#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>
#include <gdk/gdkx.h>
#include <glib/gi18n.h>
#include <girara/session.h>
#include "callbacks.h"
#include "commands.h"
#include "nioc.h"

void cb_destroy(GtkWidget* GIRARA_UNUSED(widget), nioc_t* nioc)
{
    if (nioc != NULL && nioc->ui.session != NULL) {
        cmd_stop(nioc->ui.session, NULL);
    }

    gtk_main_quit();
}

void realize_cb(GtkWidget* widget, nioc_t* nioc)
{
    GdkWindow* window = gtk_widget_get_window(widget);
    guintptr window_handle;

    if (!gdk_window_ensure_native(window))
        girara_notify(nioc->ui.session, GIRARA_ERROR,
                      _("Couldn't create native window needed for GstXOverlay!"));

/* Retrieve window handler from GDK */
#if defined(GDK_WINDOWING_WIN32)
    window_handle = (guintptr)GDK_WINDOW_HWND(window);
#elif defined(GDK_WINDOWING_QUARTZ)
    window_handle = gdk_quartz_window_get_nsview(window);
#elif defined(GDK_WINDOWING_X11)
    window_handle = GDK_WINDOW_XID(window);
#endif
    /* Pass it to playbin2, which implements XOverlay and will forward it to the video sink */
    gst_x_overlay_set_window_handle(GST_X_OVERLAY(nioc->media.playbin), window_handle);
}

/* This function is called everytime the video window needs to be redrawn (due to damage/exposure,
 * rescaling, etc). GStreamer takes care of this in the PAUSED and PLAYING states, otherwise,
 * we simply draw a black rectangle to avoid garbage showing up. */
gboolean draw_cb(GtkWidget* widget, cairo_t* cr, nioc_t* nioc)
{
    if (nioc->media.state < GST_STATE_PAUSED) {
        GtkAllocation allocation;

        /* Cairo is a 2D graphics library which we use here to clean the video window.
         * It is used by GStreamer for other reasons, so it will always be available to us. */
        gtk_widget_get_allocation(widget, &allocation);
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_rectangle(cr, 0, 0, allocation.width, allocation.height);
        cairo_fill(cr);
    }

    return FALSE;
}
