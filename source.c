/*
 * SIMON GAME
 * Author: Cody Li           
 * Student Number: 6125836   
 * Lab Section:L1N     
 * Date: Nov 30 2018
 * Email: Cody.li@alumni.ubc.ca
 * Purpose: To play the game of Simon on a DAQ Module
 *
 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <DAQlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define ON 1
#define OFF 0

 /* The input channels for the four buttons */
#define BUTTON0 0
#define BUTTON1 1
#define BUTTON2 2
#define BUTTON3 3

/* The states of the leds */
#define LED0 0 
#define LED1 1
#define LED2 2
#define LED3 3

#define NUM_LEDS 4
#define SEQUENCE_LENGTH 5

/* function prototypes for all the functions implemented here */
void runSimon();
int randInt(int lower, int upper);
void generateSequence(int length, int data[]);
int getButton();
void flashLED(int LED);

int main(void)
{
	int setupNum;

	srand((unsigned)time(NULL));

	printf("Enter configuration type (0 for the device or 6 for the simulator): ");
	scanf("%d", &setupNum);

	if (setupDAQ(setupNum) == TRUE)
		runSimon();
	else
		printf("ERROR: Cannot initialise system\n");

	system("PAUSE");
	return 0;
}

void runSimon()
{
	int data[SEQUENCE_LENGTH];
	int plays = 1;

	generateSequence(SEQUENCE_LENGTH, data);

	Sleep(1000);
	
	while (continueSuperLoop() == TRUE)
	{
		//Checks if player has won 
		if (plays == SEQUENCE_LENGTH + 1) {
			//Flashes 3 green LEDS to symbolize a win
			for (int winCount = 0; winCount < 3; winCount++) {
				flashLED(LED0);
			}
			//generates a new sequence and resets the game
			generateSequence(SEQUENCE_LENGTH, data);
			plays = 0;
		}

		//displays the sequence of LEDs
		for (int displayCount = 0; displayCount < plays; displayCount++) {
			flashLED(data[displayCount]);
		}

		//reads the order of buttons pressed
		for (int readCount = 0; readCount < plays; readCount++) {
			//Checks if they get the sequence incorrect
			if (getButton() != data[readCount]) {
				//Flashes the red LED 3 times to syhbolize a loss
				for (int loseCount = 0; loseCount < 3; loseCount++) {
					flashLED(LED1);
				}
				//generates a new sequence and resets the game
				generateSequence(SEQUENCE_LENGTH,data);
				plays = 0;
			}
		}

		Sleep(1000);

		plays++;
	}
}
/*
Purpose: returns a random integer between the lower and upper bounds
parameter: lower - the lower bounds
		   upper - the upper bounds
*/
int randInt(int lower, int upper) {

	return rand() % upper;
}
/*
Purpose: Generates the sequence of LEDs that will be displayed and writes it to an array
Parameters: length - length of given array
			data - the array the sequence will be written to
*/
void generateSequence(int length, int data[]) {

	for (int index = 0; index < length; index++) {
		data[index] = randInt(0, NUM_LEDS);
	}
	
}
/*
Purpose: read the button that the user presses
Returns: the position of the button pressed
*/
int getButton() {

	//waits for user to press a button
	while (digitalRead(BUTTON0) == FALSE ||
		digitalRead(BUTTON1) == FALSE ||
		digitalRead(BUTTON2) == FALSE ||
		digitalRead(BUTTON3) == FALSE) {

		if (continueSuperLoop() == FALSE)
			return;

		if (digitalRead(BUTTON0) == TRUE) {
			//waits for user to release the button
			while (digitalRead(BUTTON0) == TRUE) {}
			return 0;
		}
		else if (digitalRead(BUTTON1) == TRUE) {
			//waits for user to release the button
			while (digitalRead(BUTTON1) == TRUE) {}
			return 1;
		}
		else if (digitalRead(BUTTON2) == TRUE) {
			//waits for user to release the button
			while (digitalRead(BUTTON2) == TRUE) {}
			return 2;
		}
		else if (digitalRead(BUTTON3) == TRUE) {
			//waits for user to release the button
			while (digitalRead(BUTTON3) == TRUE) {}
			return 3;
		}
	}
}
/*
Purpose: flashes the LED in a given position once
Parameters: LED - the position of the LED 
*/
void flashLED(int LED) {
	Sleep(250);
	digitalWrite(LED, ON);
	Sleep(250);
	digitalWrite(LED, OFF);
}