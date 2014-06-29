#include "nioc.h"

#include <glib/gi18n.h>
#include <girara/session.h>
#include <girara/utils.h>
#include <girara/commands.h>
#include <girara/config.h>
#include <girara/shortcuts.h>
#include <girara/settings.h>
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
    g_mutex_init(&nioc->media.lock);
    g_mutex_lock(&nioc->media.lock);
    nioc->media.playbin = gst_element_factory_make("playbin2", "playbin");
    g_mutex_unlock(&nioc->media.lock);

    if (nioc->media.playbin == NULL) {
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

    /* Drawing area */
    nioc->ui.renderer = gtk_drawing_area_new();
    gtk_widget_set_double_buffered(nioc->ui.renderer, FALSE);

    /* Commands */
    girara_inputbar_command_add(nioc->ui.session, "open", "o", cmd_open, NULL, _("Open medium"));
    girara_inputbar_command_add(nioc->ui.session, "play-pause", "p", cmd_play_pause, NULL, _("Start/Pause/Resume playback"));
    girara_inputbar_command_add(nioc->ui.session, "stop", "s", cmd_stop, NULL, _("Stop playback"));
    girara_inputbar_command_add(nioc->ui.session, "mute", "m", cmd_mute, NULL, _("(Un)Mute playback"));
    girara_inputbar_command_add(nioc->ui.session, "fullscreen", "f", cmd_fullscreen, NULL, _("Toggle fullscreen"));

    /* Shortcuts */
    girara_shortcut_add(nioc->ui.session, 0, GDK_KEY_p, NULL, sc_play_pause, 0, 0, NULL);
    girara_shortcut_add(nioc->ui.session, 0, GDK_KEY_s, NULL, sc_stop, 0, 0, NULL);
    girara_shortcut_add(nioc->ui.session, 0, GDK_KEY_m, NULL, sc_mute, 0, 0, NULL);
    girara_shortcut_add(nioc->ui.session, 0, GDK_KEY_f, NULL, sc_fullscreen, 0, 0, NULL);
    girara_shortcut_add(nioc->ui.session, 0, GDK_KEY_q, NULL, girara_sc_quit, 0, 0, NULL);

    /* Settings */
    girara_setting_set(nioc->ui.session, "window-icon", "totem");

    /* signals */
    g_signal_connect(nioc->ui.renderer, "realize", G_CALLBACK(realize_cb), nioc);
    g_signal_connect(nioc->ui.renderer, "draw", G_CALLBACK(draw_cb), nioc);
    g_signal_connect(G_OBJECT(nioc->ui.session->gtk.window), "destroy", G_CALLBACK(cb_destroy), nioc);

    girara_set_view(nioc->ui.session, nioc->ui.renderer);

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

    gst_element_set_state(nioc->media.playbin, GST_STATE_VOID_PENDING);

    g_free(nioc);
}
