/*
 * stepper.h
 *
 *  Created on: 17 nov. 2021
 *      Author: 40118563
 */

#ifndef STEPPER_H_
#define STEPPER_H_

typedef enum StepperIdentification {
	A,
	B
} StepperIdentification;

typedef enum Direction {CW = 1, CCW = 0} Direction;

void setLengthAndDirectionStepper(uint8_t length, Direction direction, StepperIdentification stepperIdentification);


#endif /* STEPPER_H_ */
