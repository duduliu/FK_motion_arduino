/*****************************************************************************/
//	Function:    Get the accelemeter of X/Y/Z axis and print out on the 
//					serial monitor.
//  Hardware:    3-Axis Digital Accelerometer(��16g)
//	Arduino IDE: Arduino-1.0
//	Author:	 Frankie.Chu		
//	Date: 	 Jan 11,2013
//	Version: v1.0
//	by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/

#include <Wire.h>
#include <ADXL345.h>


#define ACC_THRESHOLD 1.0
#define XYZ_THRESHOLD 15
int led = 9;
int ledflash = 0;
int x0, y0, z0;

ADXL345 adxl; //variable adxl is an instance of the ADXL345 library

void setup(){
  Serial.begin(9600);
  for (led = 9; led>=2; led--)
  {
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  }
  led = 9;
  ledflash = 0;
  
  adxl.powerOn();

  //set activity/ inactivity thresholds (0-255)
  adxl.setActivityThreshold(75); //62.5mg per increment
  adxl.setInactivityThreshold(75); //62.5mg per increment
  adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
  adxl.setActivityX(1);
  adxl.setActivityY(1);
  adxl.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  adxl.setInactivityX(1);
  adxl.setInactivityY(1);
  adxl.setInactivityZ(1);
 
  //look of tap movement on this axes - 1 == on; 0 == off
  adxl.setTapDetectionOnX(0);
  adxl.setTapDetectionOnY(0);
  adxl.setTapDetectionOnZ(1);
 
  //set values for what is a tap, and what is a double tap (0-255)
  adxl.setTapThreshold(50); //62.5mg per increment
  adxl.setTapDuration(15); //625us per increment
  adxl.setDoubleTapLatency(80); //1.25ms per increment
  adxl.setDoubleTapWindow(200); //1.25ms per increment
 
  //set values for what is considered freefall (0-255)
  adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
 
  //setting all interrupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
  //register interrupt actions - 1 == on; 0 == off  
  adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
  
  adxl.readXYZ(&x0, &y0, &z0);
}

int getAcc()
{	//Boring accelerometer stuff   
	int x,y,z;  
        int returnval;
	adxl.readXYZ(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
	// Output x,y,z values 
	Serial.print("values of X , Y , Z: ");
	Serial.print(x);
	Serial.print(" , ");
	Serial.print(y);
	Serial.print(" , ");
	Serial.println(z);
	
	double xyz[3];
	double ax,ay,az;
	adxl.getAcceleration(xyz);
	ax = xyz[0];
	ay = xyz[1];
	az = xyz[2];
	Serial.print("X=");
	Serial.print(ax);
    Serial.println(" g");
	Serial.print("Y=");
	Serial.print(ay);
    Serial.println(" g");
	Serial.print("Z=");
	Serial.println(az);
    Serial.println(" g");
	Serial.println("**********************");

       if (abs(x-x0)>XYZ_THRESHOLD || abs(y-y0)>XYZ_THRESHOLD || abs(z-z0)>XYZ_THRESHOLD)
           returnval = 1;
        else
           returnval = 0;
          x0=x;
          y0=y;
          z0=z;
          return returnval;
          

/*
       if (abs(ax)>ACC_THRESHOLD || abs(ay)>ACC_THRESHOLD || abs(az)>ACC_THRESHOLD)
           return 1;
        else
        return 0;*/
	//delay(500);
}

int getLed(int td)
{
    if (td < 2) td = td + 8;
    if (td > 9) td = td - 8;
    return td;
}

int checkcount=0;
void loop()
{
    if (getAcc())
    {
        //ledflash ++;
        //if (ledflash > 8) ledflash = 8;
        ledflash = 8;
        //led = 9;
    }
    else
    {
        ledflash --;
        if (ledflash < 0) ledflash = 0;
    }
      
    if (ledflash)
    {
    digitalWrite(getLed(led), HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(getLed(led+1), LOW);   // turn the LED on (HIGH is the voltage level)
    led--; 
    led = getLed(led);
    }
    else
    {
      for (int td=9; td>=2; td--)
        digitalWrite(getLed(td), LOW);
       led = 9;
    }
    delay(100);               // wait for a second
}

