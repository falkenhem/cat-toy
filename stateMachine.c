#include "stateMachine.h"

State currentState = INIT;

void stateMachine(struct stepper *stepper){

	switch (currentState){
	case INIT:

		break;
	case RUNNING:

		break;

	case IDLE:

		break;
	}

}

void changeState(State state){
	currentState = state;
}

State getCurrentState(){
	return currentState;
}
