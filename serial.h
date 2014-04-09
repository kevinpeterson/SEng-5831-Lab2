/**
 * A basic serial communications interface.
 */
#ifndef SERIAL_H_
#define SERIAL_H_

/**
 * Initialize serial communications.
 */
void initialize_serial();

/**
 * Send a string of a given length via serial.
 */
void serial_to_send( char *buffer, int n );

/**
 * Register a function callback that will be called whenever
 * serial traffic is read. Any logic wanting to listen to incoming
 * serial transmissions should register here.
 */
void register_incoming_callback(void (*callback)(char c));

void set_echo_back(char echo);

#endif /* SERIAL_H_ */
