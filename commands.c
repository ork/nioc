/* See LICENSE file for license and copyright information */

#include <string.h>
#include <stdlib.h>
#include <glib/gi18n.h>

#include "commands.h"
#include "nioc.h"

#include <girara/session.h>
#include <girara/settings.h>
#include <girara/commands.h>
#include <girara/datastructures.h>
#include <girara/utils.h>

static gchar*
get_uri_from_string(const gchar* path)
{
    g_return_val_if_fail(path != NULL, NULL);

    gchar* uri = NULL;
    gchar* scheme = g_uri_parse_scheme(path);

    if (scheme == NULL) {
        gchar* tmp = NULL;

        if (g_path_is_absolute(path)) {
            tmp = g_strdup(path);
        } else {
            tmp = g_build_filename(g_get_current_dir(), path, NULL);
        }

        if (g_file_test(tmp, G_FILE_TEST_EXISTS)) {
            uri = g_filename_to_uri(tmp, NULL, NULL);
        }

        g_free(tmp);
    } else if (g_str_has_prefix(scheme, "http")) {
        uri = g_strdup(path);
    }

    return uri;
}

bool
cmd_open(girara_session_t* session, girara_list_t* argument_list)
{
    g_return_val_if_fail(session != NULL, false);
    g_return_val_if_fail(session->global.data != NULL, false);
    nioc_t* nioc = session->global.data;

    const int argc = girara_list_size(argument_list);
    if (argc > 2) {
        girara_notify(session, GIRARA_ERROR, _("Too many arguments."));
        return false;
    } else if (argc >= 1) {
        gst_element_set_state(nioc->media.playbin, GST_STATE_READY);

        g_object_set(nioc->media.playbin, "uri",
                     get_uri_from_string(girara_list_nth(argument_list, 0)),
                     NULL);
        if (gst_element_set_state(nioc->media.playbin, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
            girara_notify(session, GIRARA_ERROR, _("Could not open URI."));
        }
    } else {
        girara_notify(session, GIRARA_ERROR, _("No arguments given."));
        return false;
    }

    return true;
}

bool
cmd_play_pause(girara_session_t* session, girara_list_t* GIRARA_UNUSED(argument_list))
{
    g_return_val_if_fail(session != NULL, false);
    g_return_val_if_fail(session->global.data != NULL, false);
    nioc_t* nioc = session->global.data;
    bool ret;

    switch (GST_STATE(nioc->media.playbin)) {
    case GST_STATE_PAUSED:
    case GST_STATE_READY:
        ret = (gst_element_set_state(nioc->media.playbin, GST_STATE_PLAYING) != GST_STATE_CHANGE_FAILURE);
        break;
    default:
        ret = (gst_element_set_state(nioc->media.playbin, GST_STATE_PAUSED) != GST_STATE_CHANGE_FAILURE);
        break;
    }

    return ret;
}

bool
sc_play_pause(girara_session_t* session, girara_argument_t* GIRARA_UNUSED(argument),
              girara_event_t* GIRARA_UNUSED(event), unsigned int GIRARA_UNUSED(t))
{
    return cmd_play_pause(session, NULL);
}

bool
cmd_stop(girara_session_t* session, girara_list_t* GIRARA_UNUSED(argument_list))
{
    g_return_val_if_fail(session != NULL, false);
    g_return_val_if_fail(session->global.data != NULL, false);
    nioc_t* nioc = session->global.data;

    return gst_element_set_state(nioc->media.playbin, GST_STATE_READY) != GST_STATE_CHANGE_FAILURE;
}

bool
sc_stop(girara_session_t* session, girara_argument_t* GIRARA_UNUSED(argument),
        girara_event_t* GIRARA_UNUSED(event), unsigned int GIRARA_UNUSED(t))
{
    return cmd_stop(session, NULL);
}

bool
cmd_mute(girara_session_t* session, girara_list_t* GIRARA_UNUSED(argument_list))
{
    g_return_val_if_fail(session != NULL, false);
    g_return_val_if_fail(session->global.data != NULL, false);
    nioc_t* nioc = session->global.data;
    gboolean mute;

    g_object_get(nioc->media.playbin, "mute", &mute, NULL);
    g_object_set(nioc->media.playbin, "mute", !mute, NULL);

    return true;
}

bool
sc_mute(girara_session_t* session, girara_argument_t* GIRARA_UNUSED(argument),
        girara_event_t* GIRARA_UNUSED(event), unsigned int GIRARA_UNUSED(t))
{
    return cmd_mute(session, NULL);
}

bool
cmd_fullscreen(girara_session_t* session, girara_list_t* GIRARA_UNUSED(argument_list))
{
    g_return_val_if_fail(session != NULL, false);
    g_return_val_if_fail(session->global.data != NULL, false);
    nioc_t* nioc = session->global.data;

    if (nioc->media.fullscreen) {
        gtk_window_unfullscreen(GTK_WINDOW(nioc->ui.session->gtk.window));
        nioc->media.fullscreen = false;
    } else {
        gtk_window_fullscreen(GTK_WINDOW(nioc->ui.session->gtk.window));
        nioc->media.fullscreen = true;
    }

    return true;
}

bool
sc_fullscreen(girara_session_t* session, girara_argument_t* GIRARA_UNUSED(argument),
              girara_event_t* GIRARA_UNUSED(event), unsigned int GIRARA_UNUSED(t))
{
    return cmd_fullscreen(session, NULL);
}

bool
cmd_quit(girara_session_t* session, girara_list_t* GIRARA_UNUSED(argument_list))
{
    cmd_stop(session, NULL);
    //sc_quit(session, NULL, NULL, 0);

    return true;
}
