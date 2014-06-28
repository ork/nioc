/* See LICENSE file for license and copyright information */

#ifndef NIOC_H
#define NIOC_H

#include <stdbool.h>
#include <girara/types.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <gst/gst.h>

#include "types.h"

struct nioc_s {
    struct
        {
        girara_session_t* session; /**< girara interface session */
        GtkWidget* renderer; /**< Widget to show the rendered graphic media */
    } ui;

    struct
        {
        GstElement* playbin; /**< Our one and only pipeline */
        GstState state; /**< Current state of the pipeline */
        gboolean fullscreen; /**< Full screen status */
        GstBus* bus; /**< Gstreamer bus */
        gint64 duration; /**< Duration of the clip, in nanoseconds */
    } media;
};

/**
 * Creates a nioc session
 *
 * @return nioc session object or NULL if nioc could not be creeated
 */
nioc_t* nioc_create(void);

/**
 * Initializes nioc
 *
 * @param nioc The nioc session
 * @return true if initialization has been successful
 */
bool nioc_init(nioc_t* nioc);

/**
 * Free nioc session
 *
 * @param nioc The nioc session
 */
void nioc_free(nioc_t* nioc);

#endif // NIOC_H
