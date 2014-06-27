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
    gst_element_set_state(nioc->media.playbin2, GST_STATE_READY);

    g_object_set(nioc->media.playbin2, "uri",
                 girara_list_nth(argument_list, 0), NULL);
    gst_element_set_state(nioc->media.playbin2, GST_STATE_PLAYING);
  } else {
    girara_notify(session, GIRARA_ERROR, _("No arguments given."));
    return false;
  }

  return true;
}

bool
cmd_quit(girara_session_t* session, girara_list_t* GIRARA_UNUSED(argument_list))
{
  //sc_quit(session, NULL, NULL, 0);

  return true;
}
