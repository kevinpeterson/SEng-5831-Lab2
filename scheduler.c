/*
 * A simple non-preemptive interrupt-driven scheduler.
 */
#include <pololu/orangutan.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include "scheduler.h"

#define MAX_TASKS 10

volatile static Task* tasks[MAX_TASKS] = {0};

volatile uint32_t counter = 0;
volatile uint32_t hyperperiod = 0;

void print_int(int i) {
	clear();
	char tempBuffer[10];
	itoa(hyperperiod, tempBuffer, 10);
	print(tempBuffer);
}

uint32_t gcd(uint32_t x, uint32_t y) {
	uint32_t z;
	while(x) {
		z = x;
		x = y % x;
		y = z;
	}
	return y;
}

uint32_t lcm(uint32_t x, uint32_t y) {
	if(!x) {
		return y;
	}
	return x / gcd(x, y) * y;
}


void initialize_scheduler() {
	// timer interrupt
	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS02);
	OCR0A = 78;
}

void release_ready_tasks() {
	uint8_t i;
	for (i = 0; i < MAX_TASKS && tasks[i]; i++) {
		volatile Task* task = tasks[i];
		// if a task has been marked as released, fire its function
		if (task->released) {
			task->interrupt_function();
			task->released = 0;
		}
	}
}

void _set_hyperperiod() {
	hyperperiod = 0;
	uint8_t i;
	for(i = 0;i < MAX_TASKS; i++){
		if(tasks[i]){
			hyperperiod = lcm(hyperperiod, tasks[i]->period);
		} else {
			break;
		}
	}
}

void recalculate_hyperperiod() {
	_set_hyperperiod();
}

void register_task(volatile Task* task) {
	uint8_t i;
	for(i = 0;i < MAX_TASKS; i++) {
		if(! tasks[i]) {
			tasks[i] = task;
			break;
		}
	}
	_set_hyperperiod();

	if(! (TIMSK0 & _BV(OCIE0A))){
		TIMSK0 = _BV(OCIE0A);
	}
}


// assume a 1000 tick hyperperiod -- release tasks (jobs) as necessary
ISR(TIMER0_COMPA_vect) {
	if (counter == hyperperiod) {
		counter = 0;
	}

	uint8_t i;
	for (i = 0; i < MAX_TASKS && tasks[i]; i++) {
		volatile Task* task = tasks[i];
		// release a job at the start of its period
		if (task->period && (counter % task->period == 0)) {
			task->released = 1;
		}
	}

	counter++;
}
