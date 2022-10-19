/*
 * log.h
 *
 *      Author: sebastian.popa
 */

#ifndef APP_LOG_LOG_H_
#define APP_LOG_LOG_H_

/* Includes */

/* Public function declaration*/
/**
 * @brief Logging function that appends data to _log_queue.
 *        Adds time stamp in [m]:[s].[ms] format
 *        Variable arguments (printf style) can be entered.
 *        No new line required at the end of message.
 */
void log_msg(char const* fmt, ...);

#endif /* APP_LOG_LOG_H_ */

/* End of File */
