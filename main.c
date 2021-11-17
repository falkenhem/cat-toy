#include <avr/io.h>
#include <avr/interrupt.h>
#include "stateMachine.h"

#define PORT_STEPPER_A PORTB
#define STOP_STEPPER_A PRR |= 1<<PRTIM0
#define START_STEPPER_A PRR &= ~(1<<PRTIM0)

volatile uint8_t directionStepper1;
volatile uint16_t stepper1Position;
volatile uint16_t stepper1StepsToRun;
volatile uint8_t stepper1LengthSync = 0;


static uint8_t calculateTimerFromStepInterval(uint8_t stepIntervalInMs);


typedef struct {
	volatile uint8_t direction;
	uint16_t position;
	volatile uint16_t stepsToRun;
	uint8_t lengthSync;
	volatile uint8_t activeCoil;
}stepper;

stepper *stepperApointer, stepperA;

static void runStepperLength(stepper *stepper, uint16_t lengthInDegress, uint8_t speed, uint8_t direction);

enum Direction {CW = 1, CCW = 0};


int main()
{
	stepperApointer = &stepperA;
	stepperA.direction = CCW;
	stepperA.lengthSync = 0;
	stepperA.position = 0;
	stepperA.stepsToRun = 0;
	stepperA.activeCoil = 0;

	DDRB = 0xFF;

	TCCR0A |= 1<<WGM01; //Clear timer when comparison kicks in
	TCCR0B = (1<<CS00) | (1<<CS02);; //Set prescalar to 1024
	OCR0A = calculateTimerFromStepInterval(2);
	TIMSK0 |= 1 << OCIE0A;

	sei();

	//init
	//runStepperLength(&stepperA, 80, 5, CCW);

	while(1)
	{

		if (getCurrentState() == INIT){
			runStepperLength(&stepperA, 80, 5, CCW);
			changeState(IDLE);
		}
		if (getCurrentState() == IDLE && stepperA.stepsToRun == 0){
			STOP_STEPPER_A;
		}

	}

}

static void runStepperLength(stepper *stepper, uint16_t lengthInDegress, uint8_t speed, uint8_t direction){
	stepper->direction = direction;
	uint16_t steps = (lengthInDegress*100)/17;
	steps = steps * 3; //3 is not correct
	stepper->stepsToRun = steps;
	START_STEPPER_A;
}

ISR (TIMER0_COMPA_vect)
{
	switch (stepperA.direction){
	case CW:
		PORTB = (0b1000 >> stepperA.activeCoil);
		break;
	case CCW:
		PORTB = (0b0001 << stepperA.activeCoil);
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
	}
}



static uint8_t calculateTimerFromStepInterval(uint8_t stepIntervalInMs){
	//Presumes prescalar set to 1024

	uint16_t stepIntervalInMicroSeconds = stepIntervalInMs * 1000;

	uint8_t microSecondsPerTick = 64;

	uint8_t timerStopCount = stepIntervalInMicroSeconds/microSecondsPerTick;


	return timerStopCount;

}

