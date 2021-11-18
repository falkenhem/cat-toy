#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "stepper.h"
#include "serialCommunication.h"


typedef struct Stepper{
	volatile uint8_t direction;
	volatile uint16_t position;
	volatile uint16_t stepsToRun;
	volatile uint8_t lengthSync;
	volatile uint8_t activeCoil;
} Stepper;

Stepper *stepperApointer, stepperA;
void runLength(Stepper *stepper, uint16_t lengthInDegress, Direction direction);


void initializeSteppers(){
	stepperApointer = &stepperA;
	stepperA.direction = CCW;
	stepperA.lengthSync = 0;
	stepperA.position = 0;
	stepperA.stepsToRun = 0;
	stepperA.activeCoil = 0;
}

ISR (TIMER0_COMPA_vect)
{
	switch (stepperA.direction){
	case CW:
		PORTB = (0b1000 >> stepperA.activeCoil);
		stepperA.position++;
		break;
	case CCW:
		PORTB = (0b0001 << stepperA.activeCoil);
		if (stepperA.position){
			stepperA.position--;
		}
		break;
	}

	if (stepperA.activeCoil < 3){
		stepperA.activeCoil++;
	} else {
		stepperA.activeCoil = 0;
	}

	//Needs protection
	if (stepperA.stepsToRun>0){
		stepperA.stepsToRun--;
	} else {
		stepperA.lengthSync = 1;
	}
}

void runLength(Stepper *stepper, uint16_t lengthInDegress, Direction direction){

	stepper->direction = direction;
	uint16_t steps = (lengthInDegress*100)/17;
	steps = steps * 3; //3 is not correct fix the math
	stepper->stepsToRun = steps;
	START_STEPPER_A;
}

uint16_t translateStepToDegrees(uint16_t steps){
	uint16_t degrees;
	degrees = steps/3;
	degrees = (degrees * 17)/100;

	return degrees;
}

void setPositionStepper(uint16_t position, StepperIdentification stepperIdentification){
	uint16_t length;
	Direction direction;

	uint16_t currentPosition;
	currentPosition = translateStepToDegrees(stepperA.position);

	switch (stepperIdentification){
	case A:
		stepperA.lengthSync = 0;

		if (position > currentPosition){
			direction = CW;
			length = position - currentPosition;
		} else {
			direction = CCW;
			length = currentPosition - position;
		}
		runLength(stepperApointer, length, direction);
		break;
	}
}

//TODO: change these to use a pointer to the stepper instead
uint8_t positionReachedStepper(StepperIdentification stepperIdentification){
	switch (stepperIdentification){
		case A:
			return stepperA.lengthSync;
			break;
		}
	return 0;
}

void setLengthAndDirectionStepper(uint16_t lengthInDegrees, Direction direction, StepperIdentification stepperIdentification){
	switch (stepperIdentification){
	case A:
		runLength(stepperApointer, lengthInDegrees, direction);
		break;
	}
}

void setZeroPosition(StepperIdentification stepperIdentification){
	switch (stepperIdentification){
	case A:
		stepperA.position = 0;
		break;
	}
}

void clearLengthSync(StepperIdentification stepperIdentification){
	switch (stepperIdentification){
	case A:
		stepperA.lengthSync = 0;
		break;
	}
}

uint16_t getStepperPosition(StepperIdentification stepperIdentification){
	return stepperA.position;
}

uint16_t getRandomRelevantPosition(StepperIdentification stepperIdentification){
	uint16_t randomPosition = rand() % 100;

	return randomPosition;
}

void setPositionInSteps(uint16_t steps){
	uint16_t stepsToMove;
	Direction direction;

	stepperA.lengthSync = 0;

	if (stepperA.position > steps) {
		direction = CCW;
		stepsToMove = stepperA.position - steps;
	} else {
		direction = CW;
		stepsToMove = steps - stepperA.position;
	}


	stepperA.direction = direction;
	stepperA.stepsToRun = stepsToMove;
}



