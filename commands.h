#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>
#include <girara/types.h>

/**
 * Display media information
 *
 * @param session The used girara session
 * @param argument_list List of passed arguments
 * @return true if no error occured
 */
bool cmd_info(girara_session_t* session, girara_list_t* argument_list);

/**
 * Opens a multimedia file
 *
 * @param session The used girara session
 * @param argument_list List of passed arguments
 * @return true if no error occured
 */
bool cmd_open(girara_session_t* session, girara_list_t* argument_list);

/**
 * Pause or Resume playback of a medium
 *
 * @param session The used girara session
 * @param argument_list List of passed arguments
 * @return true if no error occured
 */
bool cmd_play_pause(girara_session_t* session, girara_list_t* argument_list);

/**
 * Stop playback of a medium
 *
 * @param session The used girara session
 * @param argument_list List of passed arguments
 * @return true if no error occured
 */
bool cmd_stop(girara_session_t* session, girara_list_t* argument_list);

/**
 * Close nioc
 *
 * @param session The used girara session
 * @param argument_list List of passed arguments
 * @return true if no error occured
 */
bool cmd_quit(girara_session_t* session, girara_list_t* argument_list);

#endif // COMMANDS_H
