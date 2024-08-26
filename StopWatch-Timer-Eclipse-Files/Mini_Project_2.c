/*
 * MINI_PROJECT_TWO.C
 *
 *  Created on: Jan 28, 2024
 *      Author: Karim Amr
 *      Description : Creating a stop-watch timer on AtMega32 using Internal TIMER1A and External Interrupts
 *      Iteration = 2
 */

/*********** Required Libraries for AtMega32 ***************/

#include <avr/io.h>
#include <avr/interrupt.h>

#define SEGMENT_NUMBERS 6 /* No. of Segments in the project */

/**********************************************************/


/**************Preprocessor macros for easier and more understandable bitwise operation***********/

#define SET_BIT(REG,BIT_NUM) (REG |=(1<<BIT_NUM)) /* Set Bit to ONE */
#define CLEAR_BIT(REG,BIT_NUM) (REG &= ~(1<<BIT_NUM)) /* Set Bit to ZERO */
#define SET_BIT_INPUT(REG,BIT_NUM) (REG &= ~(1<<BIT_NUM)) /* Set Bit to Input by setting pin to ZERO */
#define SET_BIT_OUTPUT(REG,BIT_NUM) (REG |=(1<<BIT_NUM)) /* Set Bit to Output by setting pin to ONE */

unsigned char Interrupt_Flag = 0; /* Timer1 Flag variable */

/**************************************************************************************************/


/******************* Timer1 Initialization *******************/
void TIMER1_Init(void)
{
	TCCR1A = (1<<FOC1A) | (1<<FOC1B) ;
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10) ;
	SET_BIT(TIMSK,OCIE1A); /* Enable Timer1 INT */
	TCNT1 = 0; /* Initialize counter with ZERO */
	OCR1A = 977 ; /* Required TOP value to Make an interrupt after 1 Second */
}

/*************************************************************/

/******************* INT0 Initialization *******************/
void INT0_Init(void) // Stop-Watch RESET */
{
	SET_BIT_INPUT(DDRD,PD2);  /* Set as input */
	SET_BIT_OUTPUT(PORTD,PD2); /* internal Pull-up */
	SET_BIT(GICR,INT0); /* INT0 enable */
	SET_BIT(MCUCR,ISC01) ; /* Falling edge configuration */
}

/*************************************************************/

/******************* INT1 Initialization *******************/
void INT1_Init(void) /* Stop-Watch PAUSE */
{
	SET_BIT_INPUT(DDRD,PD3); /* Set as input */
	SET_BIT(GICR,INT1); /* INT1 enable */
	MCUCR |= (1<<ISC11) | (1<<ISC10) ; /* Raising Edge configuration */
}

/*************************************************************/

/******************* INT2 Initialization *******************/
void INT2_Init(void) /* Stop-Watch RESUME */
{
	SET_BIT_INPUT(DDRB,PB2); /* Set as input */
	SET_BIT_OUTPUT(PORTB,PB2); /* internal Pull-up configuration */
	SET_BIT(GICR,INT2); /* INT2 enable */
	CLEAR_BIT(MCUCSR,ISC2) ; /* Falling Edge configuration */
}

/******************************************************************/


/**********************Clock Digits HH:MM:SS***********************/

unsigned int Tick = 0 ; /* counter for seconds */
unsigned char Seconds = 0; /* Seconds value */
unsigned char Minutes = 0; /* Minutes value */
unsigned char Hours = 0; /* Hours Value */


/*Array of number of req. digits [HH:MM:SS] that contains the value of current digit*/
/*As we have two digits for each time sector , each sector will be divided once by /10
 * to get the Most Significant digit , and the other by %10 to get the least significant digit
 * according the counter value each tick*/

/*Set to global variable to be accessed by req. functions*/
/*****************************************************************/


int main()
{
	DDRA |= 0x3F; /* setting first 6 pins in PORTA as output pins to control each 7 segment */
	/* set SEGMENT_NUMBERS as a variable for number of required digits */
	DDRC |= 0x0F; /* Setting first 4 pins of PORTC as output pins for 7 segment value */
	SREG |= (1<<7); /* Enables Global Interrupt */

	INT0_Init();
	INT1_Init();
	INT2_Init();
	TIMER1_Init();

	unsigned char DigitDecode[10] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};
	/*The Previous array holds the required digit value in HEX to be sent to the decoder
	   to display the required digit on stop-watch while preserving the last four pins in
	   PORTC without altering them  */

	unsigned char SegmentsLooping=0; /*Variable that will loop around the digits */

	while(1)
	{
		/*An Array that holds the value of each sector of stop watch into digits */
		unsigned char StopWatch[SEGMENT_NUMBERS] = {(Seconds%10),(Seconds/10),\
													(Minutes%10),(Minutes/10),\
													(Hours%10),(Hours/10)};

		if(SegmentsLooping==SEGMENT_NUMBERS)
			SegmentsLooping=0;

		/*Looping on the 6 Segments to show and display the current value simultaneously */

		PORTA &= 0xC0; /* Disable all 7 segments */
		PORTC &= 0xF0; /* Clear the output pins and set them to ZERO */

		PORTC |= DigitDecode[StopWatch[SegmentsLooping]];
		/*Sends required value of the current digit to the decoder*/

		SET_BIT(PORTA,SegmentsLooping); /* Enable the required 7-segment */

		SegmentsLooping++; /*Increment the variable to loop on the next 7-segment*/



		if(1==Interrupt_Flag) /*If interrupt flag is raised , counter code will execute*/
		{
			/*********Digits Value Counter********/

			if(59==Seconds)
			{
				Seconds=0;
				if(59==Minutes && 3600==Tick)
				/*Checks if the minutes digits had completed 1 Hour (3600 seconds = 60Minutes*60Seconds) */
				{
					Minutes=0;
					if(99==Hours && 360000==Tick)
					/*Checks if the Hours digits had completed 100 Hours
					 *  (360000 seconds = 100Hours*60Minutes*60Seconds) */
					{
						/*OverFlow Occurs at the end of counting */
						Hours=0;
						Minutes=0;
						Seconds=0;
					}
					else
					Hours++;
				}
				else
				Minutes++;
			}
			else
			Seconds++;

			/**************************************/

			Interrupt_Flag=0; /*Set the flag variable back to ZERO -> OFF*/
		}
	}
}

/*****************INT0 Interrupt Execution****************/
ISR(INT0_vect)
{
  //Timer Reset

	/*Turns Off Timer1 Source */
	CLEAR_BIT(TCCR1B,CS10);
	CLEAR_BIT(TCCR1B,CS12);

	Tick = 0 ; /* Set Counter back to ZERO */

	 /* Reset the stop-watch digits back to ZERO */
	Seconds = 0;
	Minutes = 0;
	Hours = 0;


	/*Turns ON Timer1 Source */
	SET_BIT(TCCR1B,CS10);
	SET_BIT(TCCR1B,CS12);

}
/*********************************************************/

/*****************INT1 Interrupt Execution****************/
ISR(INT1_vect)
{
    /* Timer PAUSE */
	/* Turns Off Timer1 Source */
	CLEAR_BIT(TCCR1B,CS10);
	CLEAR_BIT(TCCR1B,CS12);
}
/*********************************************************/

/*****************INT2 Interrupt Execution****************/
ISR(INT2_vect)
{
    /* Timer RESUME */
	/*Turns ON Timer1 Source */
	SET_BIT(TCCR1B,CS10);
	SET_BIT(TCCR1B,CS12);
}
/********************************************************/


ISR(TIMER1_COMPA_vect)
{
	Tick++; /*Increment Tick counter*/
	Interrupt_Flag = 1; /*Set Flag variable to one to exit the interrupt as quick as possible */

}

/**********************************************************************************************************/

