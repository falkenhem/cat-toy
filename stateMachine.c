#include <avr/io.h>
#include <stdlib.h>
#include "stateMachine.h"
#include "stepper.h"
#include "serialCommunication.h"


State currentState = INIT;

void stateMachine(){

	char buffer[6];

	switch (currentState){
	case INIT:
		uart_init();
		setLengthAndDirectionStepper(50, CCW, A);
		changeState(CALIBRATING);
		break;
	case CALIBRATING:
		if (positionReachedStepper(A)){
			STOP_STEPPER_A;
			setZeroPosition(A);
			changeState(RUNNING);
		}
		break;
	case RUNNING:
		if (positionReachedStepper(A)){
			STOP_STEPPER_A;
			uart_putstr(utoa(getStepperPosition(A), buffer, 10));
			uart_putstr("new random value\n");
			setPositionStepper(getRandomRelevantPosition(A), A);
		}
		break;

	case IDLE:
		if (positionReachedStepper(A)){
			STOP_STEPPER_A;

		}
		break;
	}

}

void changeState(State state){
	currentState = state;
}

State getCurrentState(){
	return currentState;
}


