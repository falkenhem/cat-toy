/*
 * stepper.h
 *
 *  Created on: 17 nov. 2021
 *      Author: 40118563
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#define PORT_STEPPER_A PORTD
#define STOP_STEPPER_A PRR |= 1<<PRTIM0
#define START_STEPPER_A PRR &= ~(1<<PRTIM0)

typedef enum StepperIdentification {
	A,
	B
} StepperIdentification;

typedef struct Stepper{
	uint8_t direction;
	volatile uint16_t position;
	volatile uint16_t stepsToRun;
	volatile uint8_t lengthSync;
	volatile uint8_t activeCoil;
} Stepper;

typedef enum Direction {CW = 1, CCW = 0} Direction;

void setLengthAndDirectionStepper(uint16_t lengthInDegrees, Direction direction, Stepper *stepperPointer);

void setStepperSpeed(Stepper *stepperPointer, uint8_t speed);

uint8_t positionReachedStepper(Stepper *stepperPointer);

void clearLengthSync(Stepper *stepperPointer);

void initializeSteppers();

void setZeroPosition(Stepper *stepperPointer);

void setPositionStepper(uint16_t position, Stepper *stepperPointer);

uint16_t getRandomRelevantPosition();

uint16_t getStepperPosition(Stepper *stepperPointer);

Stepper *getStepperPointer(StepperIdentification stepperIdentification);

void setPositionInSteps(uint16_t steps);


#endif /* STEPPER_H_ */
