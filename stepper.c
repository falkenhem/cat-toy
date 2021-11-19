#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "stepper.h"
#include "serialCommunication.h"

Stepper *stepperApointer, stepperA;
void runLength(Stepper *stepper, uint16_t lengthInDegress, Direction direction);


void initializeSteppers(){
	//TODO: protect these from isr corruption
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
		PORTD = (0b10000000 >> stepperA.activeCoil);
		stepperA.position++;
		break;
	case CCW:
		PORTD = (0b00010000 << stepperA.activeCoil);
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

void runLength(Stepper *stepperPointer, uint16_t lengthInDegress, Direction direction){

	stepperPointer->direction = direction;
	uint16_t steps = (lengthInDegress*100)/17;
	steps = steps * 3; //3 is not correct fix the math
	stepperPointer->stepsToRun = steps;
	START_STEPPER_A;
}

uint16_t translateStepToDegrees(uint16_t steps){
	uint16_t degrees;
	degrees = steps/3;
	degrees = (degrees * 17)/100;

	return degrees;
}

void setPositionStepper(uint16_t position, Stepper *stepperPointer){
	uint16_t length;
	Direction direction;
	uint16_t currentPosition;

	currentPosition = translateStepToDegrees(stepperA.position);

	stepperPointer->lengthSync = 0;

	if (position > currentPosition){
		direction = CW;
		length = position - currentPosition;
	} else {
		direction = CCW;
		length = currentPosition - position;
	}
	runLength(stepperPointer, length, direction);


}

uint8_t positionReachedStepper(Stepper *stepperPointer){
	return stepperPointer->lengthSync;
}

void setLengthAndDirectionStepper(uint16_t lengthInDegrees, Direction direction, Stepper *stepperPointer){
	runLength(stepperPointer, lengthInDegrees, direction);

}

void setZeroPosition(Stepper *stepperPointer){
	stepperPointer->position = 0;
}

void clearLengthSync(Stepper *stepperPointer){
	stepperPointer->lengthSync = 0;
}

uint16_t getStepperPosition(Stepper *stepperPointer){
	return stepperPointer->position;
}

uint16_t getRandomRelevantPosition(){
	uint16_t randomPosition = rand() % 100;

	return randomPosition;
}

Stepper *getStepperPointer(StepperIdentification stepperIdentification){
	//expand if more steppers are needed
	return stepperApointer;
}




