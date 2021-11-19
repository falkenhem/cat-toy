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
		setLengthAndDirectionStepper(50, CCW, getStepperPointer(A));
		changeState(CALIBRATING);
		break;
	case CALIBRATING:
		if (positionReachedStepper(getStepperPointer(A))){
			STOP_STEPPER_A;
			setZeroPosition(A);
			changeState(RUNNING);
		}
		break;
	case RUNNING:
		if (positionReachedStepper(getStepperPointer(A))){
			STOP_STEPPER_A;
			uart_putstr(utoa(getStepperPosition(getStepperPointer(A)), buffer, 10));
			uart_putstr("new random value\n");
			setPositionStepper(getRandomRelevantPosition(), getStepperPointer(A));
			OCR2A = getRandomRelevantPosition();
		}
		break;

	case IDLE:
		if (positionReachedStepper(getStepperPointer(A))){
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


