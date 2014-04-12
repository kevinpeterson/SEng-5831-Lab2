/**
 * MSSE SEng 5831 Lab2 main file.
 */
//#define WELCOME_MSG

#include <pololu/orangutan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "scheduler.h"
#include "motor.h"
#include "command_line.h"
#include "serial.h"
#include "logger.h"
#include "motor_encoder.h"
#include "pd_controller.h"


#ifdef WELCOME_MSG

void print_welcome_message() {
	char* msg = "Kevin's Lab 2";
	int8_t i = -strlen(msg);
	for(;i < 16;i++){
		clear();
		char* m;
		int x = i;
		if(i < 0){
			x = 0;
			m = msg+abs(i);
		} else {
			m = msg;
		}
		lcd_goto_xy(x,0);
		print(m);
		lcd_goto_xy(1,1);
		print("MSSE SEng 5831");
		delay_ms(300);
	}
}
#endif


/**
 * There isn't much in this file... it initializes things and launches the
 * tasks that the scheduler schedules.
 *
 * Most of the interesting stuff is in pd_controller.c, motor.c, etc...
 */
int main(void) {
	clear();

	// intitialize things
	initialize_scheduler();
	initialize_serial();
	initialize_command_line();
	initialize_motor_encoder();
	initialize_motor();
	initialize_pd_controller();

	sei();

	print("Starting...");
	delay_ms(500);
	clear();

	while(1) {
		// release tasks
		release_ready_tasks();
	}

} //end main
