/*
 * stateMachine.h
 *
 *  Created on: 17 nov. 2021
 *      Author: 40118563
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

typedef enum {
	INIT,
	RUNNING,
	IDLE
} State;

typedef enum {
	STARTRUNNING,
	REACHED_DESTINATION
} Event;

void stateMachine();

void changeState(State state);
State getCurrentState();


#endif /* STATEMACHINE_H_ */
