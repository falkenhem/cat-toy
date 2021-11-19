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

typedef enum Direction {CW = 1, CCW = 0} Direction;

void setLengthAndDirectionStepper(uint16_t lengthInDegrees, Direction direction, StepperIdentification stepperIdentification);

void setStepperSpeed(StepperIdentification stepperIdentification, uint8_t speed);

uint8_t positionReachedStepper(StepperIdentification stepperIdentification);

void clearLengthSync(StepperIdentification stepperIdentification);

void initializeSteppers();

void setZeroPosition(StepperIdentification stepperIdentification);

void setPositionStepper(uint16_t position, StepperIdentification stepperIdentification);

uint16_t getRandomRelevantPosition(StepperIdentification stepperIdentification);

uint16_t getStepperPosition(StepperIdentification stepperIdentification);

void setPositionInSteps(uint16_t steps);


#endif /* STEPPER_H_ */
