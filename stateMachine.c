#include <avr/io.h>
#include "stateMachine.h"
#include "stepper.h"
#include "serialCommunication.h"

State currentState = INIT;

void stateMachine(){

	switch (currentState){
	case INIT:
		uart_init();
		setLengthAndDirectionStepper(250, CCW, A);
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

