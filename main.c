/* See LICENSE file for license and copyright information */

#define _BSD_SOURCE
#define _XOPEN_SOURCE 700

#include <girara/utils.h>
#include <glib/gstdio.h>

#include "nioc.h"

/* main function */
int
main(int argc, char* argv[])
{
    /* init gtk */
    gtk_init(&argc, &argv);
    gst_init(&argc, &argv);

    /* create zathura session */
    nioc_t* nioc = nioc_create();
    if (nioc == NULL) {
        return -1;
    }

    /* Init zathura */
    if (nioc_init(nioc) == false) {
        girara_error("Could not initialize nioc.");
        nioc_free(nioc);
        return -1;
    }

    /* run zathura */
    gtk_main();

    /* free zathura */
    nioc_free(nioc);

    return 0;
}
