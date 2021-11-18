#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr\delay.h>
#include "stateMachine.h"
#include "stepper.h"
#include "serialCommunication.h"

State currentState = INIT;

void stateMachine(){

	switch (currentState){
	case INIT:
		uart_init();
		setLengthAndDirectionStepper(200, CCW, A);
		changeState(CALIBRATING);
		break;
	case CALIBRATING:
		if (positionReachedStepper(A)){
			STOP_STEPPER_A;
			uart_putstr("reached pos 0/n");
			setZeroPosition(A);
			setPositionStepper(50, A);
			changeState(RUNNING);
		}
		break;
	case RUNNING:
		if (positionReachedStepper(A)){
			STOP_STEPPER_A;
			//setPositionStepper(getRandomRelevantPosition(A), A);
			setPositionStepper(60, A);
			changeState(IDLE);
		}
		break;

	case IDLE:
		if (positionReachedStepper(A)){
			STOP_STEPPER_A;
			uart_putstr("idling/n");
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

