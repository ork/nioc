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
#if !GLIB_CHECK_VERSION(2, 31, 0)
  g_thread_init(NULL);
#endif
#if !GTK_CHECK_VERSION(3, 6, 0)
  gdk_threads_init();
#endif
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
#if !GTK_CHECK_VERSION(3, 6, 0)
  gdk_threads_enter();
#endif
  gtk_main();
#if !GTK_CHECK_VERSION(3, 6, 0)
  gdk_threads_leave();
#endif

  /* free zathura */
  nioc_free(nioc);

  return 0;
}
