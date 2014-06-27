#include "nioc.h"

#include <glib/gi18n.h>
#include <girara/session.h>
#include <girara/utils.h>
#include <girara/commands.h>
#include "commands.h"
#include "callbacks.h"

nioc_t*
nioc_create(void)
{
    nioc_t* nioc = g_try_malloc0(sizeof(nioc));
    if (nioc == NULL) {
        return NULL;
    }

    /* UI */
    if ((nioc->ui.session = girara_session_create()) == NULL) {
        goto error_out;
    }

    /* Media */
    nioc->media.duration = GST_CLOCK_TIME_NONE;
    nioc->media.playbin2 = gst_element_factory_make("playbin2", "playbin2");

    if (nioc->media.playbin2 == NULL) {
        girara_error("Could not initialize gstreamer.");
        goto error_out;
    }

    nioc->ui.session->global.data = nioc;

    return nioc;

error_out:

    nioc_free(nioc);

    return NULL;
}

bool
nioc_init(nioc_t* nioc)
{
    if (nioc == NULL) {
        return false;
    }

    /* UI */
    if (girara_session_init(nioc->ui.session, "nioc") == false) {
        goto error_free;
    }

    /* page view */

    nioc->ui.renderer = gtk_drawing_area_new();
    gtk_widget_set_double_buffered(nioc->ui.renderer, FALSE);
    g_signal_connect(nioc->ui.renderer, "realize", G_CALLBACK(realize_cb), nioc);
    g_signal_connect(nioc->ui.renderer, "expose_event", G_CALLBACK(expose_cb), nioc);

    girara_set_view(nioc->ui.session, nioc->ui.renderer);
    girara_inputbar_command_add(nioc->ui.session, "open", "o", cmd_open, NULL, _("Open medium"));
    g_object_set(nioc->media.playbin2, "uri", "http://docs.gstreamer.com/media/sintel_trailer-480p.webm", NULL);
    gst_element_set_state(nioc->media.playbin2, GST_STATE_PLAYING);

    /* signals */
    //g_signal_connect(G_OBJECT(nioc->ui.session->gtk.window), "destroy", G_CALLBACK(cb_destroy), nioc);

    return true;

error_free:

    return false;
}

void
nioc_free(nioc_t* nioc)
{
    if (nioc == NULL) {
        return;
    }

    if (nioc->ui.session != NULL) {
        girara_session_destroy(nioc->ui.session);
    }

    g_free(nioc);
}
