#include "stepper.h"

unsigned int scan_360_closest_step_count = 0;   //counter to count how many half steps since closest scanned object
int old_adc_distance = 0;                   //variable to store closest reading of adc distance since push button press
int scan_counter = 0;

unsigned char cw_control_byte = 0b00001101;     //stepper motor control byte for; enabled, clockwise, half-steps
unsigned char ccw_control_byte = 0b00001111;    //stepper motor control byte for; enabled, counterclockwise, half-steps
unsigned char off_control_byte = 0b00001100;

//rotate stepper CW 360 degrees. scan adc distance each half step.
void scanCw(unsigned int steps) {
    resetADC();
    spi_transfer(cw_control_byte);

	for(steps; steps!=0; steps--){
        findClosestWall();
        SM_STEP();
        adcDisplay();
	}
    spi_transfer(off_control_byte);
    __delay_ms(2);
}

void scanCcw(unsigned int steps) {
    resetADC();
    spi_transfer(ccw_control_byte);

	for(steps; steps!=0; steps--){
        findClosestWall();
        SM_STEP();
        adcDisplay();
	}
    spi_transfer(off_control_byte);
    __delay_ms(2);
}

//takes ADC and checks against old adc value, keeping the closest 'distance'
void findClosestWall(void) {
    int new_adc_distance = getAdcDist(getAdc());

    if (new_adc_distance <= old_adc_distance) {
        old_adc_distance = new_adc_distance;
        scan_360_closest_step_count = 0;
    } else {
        scan_360_closest_step_count++;
    }
}

//move stepper CW
void moveCW(unsigned int steps) {
    spi_transfer(cw_control_byte);
    for(steps; steps!=0; steps--){
        SM_STEP();
		__delay_ms(1);
    }
    spi_transfer(off_control_byte);
}

//move stepper CCW
void moveCCW(unsigned int steps) {
    spi_transfer(ccw_control_byte);
    for(steps; steps!=0; steps--){
        SM_STEP();
		__delay_ms(1);
    }
    spi_transfer(off_control_byte);
}

//function to clear adc distance and step counters
void resetADC(void) {
    scan_360_closest_step_count = 0;
    old_adc_distance = getAdcDist(getAdc());
}
