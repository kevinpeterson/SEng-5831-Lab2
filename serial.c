/**
 * A basic serial communications interface.
 */
#include <pololu/orangutan.h>
#include <string.h>
#include <stdlib.h>
#include "scheduler.h"
#include "command_parse.h"

void check_for_new_bytes_received();
void _monitor_serial();

static volatile char echo_back = 1;

/**
 * The serial monitoring task to be scheduled.
 */
volatile Task monitor_serial_task = { .period = 5, .interrupt_function =
		&_monitor_serial, .released = 0, .name = "Monitor Serial Task" };

/**
 * A linked-list of callback functions.
 */
typedef struct CallbackFn {
	void (*callback)(char c);
	struct CallbackFn* next;
} CallbackFn;

static char receive_buffer[32];
static unsigned char receive_buffer_position;

CallbackFn* callback_list = NULL;

void register_incoming_callback(void (*callback_)(char c)) {
	if(callback_list == NULL) {
		CallbackFn *f = (CallbackFn*) malloc(sizeof(CallbackFn));
		f->callback = callback_;
		f->next = NULL;
		callback_list = f;
	} else {
		CallbackFn* fn = callback_list;
		while(fn->next != NULL) {
			fn = fn->next;
		}
		CallbackFn *f = (CallbackFn*) malloc(sizeof(CallbackFn));
		f->callback = callback_;
		f->next = NULL;
		fn->next = f;
	}
}


//-------------------------------------------------------------------------------------------
// wait_for_sending_to_finish:  Waits for the bytes in the send buffer to
// finish transmitting on USB_COMM.  We must call this before modifying
// send_buffer or trying to send more bytes, because otherwise we could
// corrupt an existing transmission.
void _wait_for_sending_to_finish() {
	while(!serial_send_buffer_empty(USB_COMM)) {
		serial_check();		// USB_COMM port is always in SERIAL_CHECK mode
	}
}

// A generic function for whenever you want to print to your serial comm window.
// Provide a string and the length of that string. My serial comm likes "\r\n" at
// the end of each string (be sure to include in length) for proper linefeed.
void serial_to_send( char *buffer, int n ) {
	serial_send( USB_COMM, buffer, n );
	_wait_for_sending_to_finish();
}

void _monitor_serial() {
	serial_check();
	check_for_new_bytes_received();
}

//------------------------------------------------------------------------------------------
// Initialize serial communication through USB and print menu options
// This immediately readies the board for serial comm
void initialize_serial() {
	// Set the baud rate to 9600 bits per second.  Each byte takes ten bit
	// times, so you can get at most 960 bytes per second at this speed.
	serial_set_baud_rate(USB_COMM, 9600);

	// Start receiving bytes in the ring buffer.
	serial_receive_ring(USB_COMM, receive_buffer, sizeof(receive_buffer));

	serial_to_send("\r\n", 2);
	serial_to_send("****************************\r\n", 30);
	serial_to_send("Lab 2 Command Line Interface\r\n", 30);
	serial_to_send("****************************\r\n", 30);

	register_task(&monitor_serial_task);
}

//---------------------------------------------------------------------------------------
// If there are received bytes to process, this function loops through the receive_buffer
// accumulating new bytes (keystrokes) in another buffer for processing.
void check_for_new_bytes_received() {
	/*
	The receive_buffer is a ring buffer. The call to serial_check() (you should call prior to this function) fills the buffer.
	serial_get_received_bytes is an array index that marks where in the buffer the most current received character resides.
	receive_buffer_position is an array index that marks where in the buffer the most current PROCESSED character resides.
	Both of these are incremented % (size-of-buffer) to move through the buffer, and once the end is reached, to start back at the beginning.
	This process and data structures are from the Pololu library. See examples/serial2/test.c and src/OrangutanSerial/

	A carriage return from your comm window initiates the transfer of your keystrokes.
	All key strokes prior to the carriage return will be processed with a single call to this function (with multiple passes through this loop).
	On the next function call, the carriage return is processes with a single pass through the loop.
	The menuBuffer is used to hold all keystrokes prior to the carriage return. The "received" variable, which indexes menuBuffer, is reset to 0
	after each carriage return.
	*/

	// while there are unprocessed keystrokes in the receive_buffer, grab them and buffer
	// them into the menuBuffer
	while(serial_get_received_bytes(USB_COMM) != receive_buffer_position)
	{
	    if(echo_back) {
		    serial_to_send(&receive_buffer[receive_buffer_position], 1);
		}

		CallbackFn* fn = callback_list;
		while(fn != NULL) {
			fn->callback(receive_buffer[receive_buffer_position]);
			fn = fn->next;
		}

		// Increment receive_buffer_position, but wrap around when it gets to
		// the end of the buffer.
		if ( receive_buffer_position == sizeof(receive_buffer) - 1 ) {
			receive_buffer_position = 0;
		}
		else {
			receive_buffer_position++;
		}
	}
}

void set_echo_back(char echo) {
	echo_back = echo;
}
