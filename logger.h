/**
 * Simple logging infrastructure to capture system events and status.
 */
#ifndef LOGGER_H_
#define LOGGER_H_

/**
 * The current level of information to log.
 */
#define LOG_LEVEL INFO

typedef enum {
  DEBUG = 0,
  INFO = 1,
  WARN = 2,
  ERROR = 3
} LogLevel;

/**
 * Log the given message. Logging will procede if the LogLevel
 * is equal to or greater than the current "LOG_LEVEL."
 */
void log_msg(char* msg, LogLevel level);

#endif /* LOGGER_H_ */
