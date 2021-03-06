/* See LICENSE file for license and copyright information */

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
 * Pause or Resume playback of a medium
 *
 */
bool sc_play_pause(girara_session_t* session, girara_argument_t* argument,
                   girara_event_t* event, unsigned int t);

/**
 * Stop playback of a medium
 *
 * @param session The used girara session
 * @param argument_list List of passed arguments
 * @return true if no error occured
 */
bool cmd_stop(girara_session_t* session, girara_list_t* argument_list);

/**
 * Stop playback of a medium
 *
 */
bool sc_stop(girara_session_t* session, girara_argument_t* argument,
             girara_event_t* event, unsigned int t);

/**
 * Toggle mute volume
 *
 * @param session The used girara session
 * @param argument_list List of passed arguments
 * @return true if no error occured
 */
bool cmd_mute(girara_session_t* session, girara_list_t* argument_list);

/**
 * Toggle mute volume
 *
 */
bool sc_mute(girara_session_t* session, girara_argument_t* argument,
             girara_event_t* event, unsigned int t);

/**
 * Toggle fullscreen
 *
 * @param session The used girara session
 * @param argument_list List of passed arguments
 * @return true if no error occured
 */
bool cmd_fullscreen(girara_session_t* session, girara_list_t* argument_list);

/**
 * Toggle mute volume
 *
 */
bool sc_fullscreen(girara_session_t* session, girara_argument_t* argument,
                   girara_event_t* event, unsigned int t);

/**
 * Close nioc
 *
 * @param session The used girara session
 * @param argument_list List of passed arguments
 * @return true if no error occured
 */
bool cmd_quit(girara_session_t* session, girara_list_t* argument_list);

#endif // COMMANDS_H
