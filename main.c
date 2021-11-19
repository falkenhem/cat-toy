#include <avr/io.h>
#include <avr/interrupt.h>
#include "stateMachine.h"
#include "stepper.h"

volatile uint8_t directionStepper1;
volatile uint16_t stepper1Position;
volatile uint16_t stepper1StepsToRun;
volatile uint8_t stepper1LengthSync = 0;


static uint8_t calculateTimerFromStepInterval(uint8_t stepIntervalInMs);


int main()
{

	DDRD = 0xFF;

	TCCR0A |= 1<<WGM01; //Clear timer when comparison kicks in
	TCCR0B = (1<<CS00) | (1<<CS02);; //Set prescalar to 1024
	OCR0A = calculateTimerFromStepInterval(3);
	TIMSK0 |= 1 << OCIE0A;

	DDRB = 0xFF;
	TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
	TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
	OCR2A = 130;

	sei();
	initializeSteppers();

	while(1)
	{
		stateMachine();

	}

}

static uint8_t calculateTimerFromStepInterval(uint8_t stepIntervalInMs){
	//Presumes prescalar set to 1024

	uint16_t stepIntervalInMicroSeconds = stepIntervalInMs * 1000;

	uint8_t microSecondsPerTick = 64;

	uint8_t timerStopCount = stepIntervalInMicroSeconds/microSecondsPerTick;


	return timerStopCount;

}

