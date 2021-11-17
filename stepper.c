#include <avr/io.h>
#include "stepper.h"

#define PORT_STEPPER_A PORTB
#define STOP_STEPPER_A PRR |= 1<<PRTIM0
#define START_STEPPER_A PRR &= ~(1<<PRTIM0)

typedef struct Stepper{
	volatile uint8_t direction;
	uint16_t position;
	volatile uint16_t stepsToRun;
	uint8_t lengthSync;
	volatile uint8_t activeCoil;
} Stepper;

void setLengthAndDirectionStepper(uint16_t lengthInDegrees, Direction direction, StepperIdentification stepperIdentification){

}

static void runLength(Stepper *stepper, uint16_t lengthInDegress, uint8_t speed, uint8_t direction){

	stepper->direction = direction;
	uint16_t steps = (lengthInDegress*100)/17;
	steps = steps * 3; //3 is not correct
	stepper->stepsToRun = steps;
	START_STEPPER_A;
}

