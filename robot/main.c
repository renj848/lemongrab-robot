#include "main.h"

//set all push button flags to 0
volatile bit pb_start_pressed = 0;
volatile bit pb_scan_pressed = 0;
volatile bit pb_drive_4m_pressed = 0;
volatile bit pb_drive_square_pressed = 0;
volatile bit pb_find_wall_pressed = 0;

//calls all other module setup functions
void setup (void) {
    __delay_ms(5000);               //5 second startup delay to filter out irobot create serial crap
    
    setupInterrupt();               //calls pic interrupt setup function
    setupSPI();                     //calls spi setup function for stepper motor control
    setupADC();                     //calls adc setup function for ir reading control
    setupLCD();                     //calls lcd setup function to allow writing to lcd
    setupIRobot();                  //calls irobot create setup function
}

//main program. starts by calling setup, then loops with pushbutton flag checks and displaying adc distance continuously
void main (void) {
    setup();
    while (1) {
        buttonControl();
        adcDisplay();
    }
}

//checks all push button flags. if one is true then call that desired function
void buttonControl (void) {
    //pb_start_pressed is temporary code. remove when ready
    if (pb_start_pressed) {
        moveStraight();
        pb_start_pressed = 0;
    }
    if (pb_scan_pressed) {
        scan360(400);
        pb_scan_pressed = 0;
    }
    if (pb_drive_4m_pressed) {
        startTest();
        pb_drive_4m_pressed = 0;
    }
    if (pb_drive_square_pressed) {
        moveCCW(100);
        pb_drive_square_pressed = 0;
    }
    if (pb_find_wall_pressed) {
        moveCCW(8);
        pb_find_wall_pressed = 0;
    }
}

//this is temporary driving test code. remove when ready
void startTest (void) {
        drive(DRIVE,0,200,0,200);
        __delay_ms(1000);
        drive(DRIVE,0,50,0,50);
        __delay_ms(4000);
        stop();
        figureEightTest();
        __delay_ms(5000);
        stop();
}
