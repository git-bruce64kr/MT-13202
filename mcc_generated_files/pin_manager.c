/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.4
        Device            :  PIC16LF19186
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.20 and above
        MPLAB             :  MPLAB X 5.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "pin_manager.h"
#include "mylib.h"




#if (__XC8_VERSION <= 1410) 

typedef union {
    struct {
        unsigned T2INPPS                :6;
    };
    struct {
        unsigned T2INPPS0               :1;
        unsigned T2INPPS1               :1;
        unsigned T2INPPS2               :1;
        unsigned T2INPPS3               :1;
        unsigned T2INPPS4               :1;
        unsigned T2INPPS5               :1;
    };
} T2INPPSbits_t;
extern volatile T2INPPSbits_t T2INPPSbits @ (&T2AINPPS);

typedef union {
    struct {
        unsigned T4INPPS                :6;
    };
    struct {
        unsigned T4INPPS0               :1;
        unsigned T4INPPS1               :1;
        unsigned T4INPPS2               :1;
        unsigned T4INPPS3               :1;
        unsigned T4INPPS4               :1;
        unsigned T4INPPS5               :1;
    };
} T4INPPSbits_t;
extern volatile T4INPPSbits_t T4INPPSbits @ (&T4AINPPS);
#endif

void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */
    LATE = 0x00;
    LATD = 0x20;
    LATA = 0x00;
    LATF = 0x00;
    LATB = 0x00;
    LATC = 0x01;

    /**
      TRISx registers
    */
    TRISE = 0x07;
    TRISF = 0x3F;
    TRISA = 0x08;
    TRISB = 0x80;
    TRISC = 0xC2;
    TRISD = 0x80;

    /**
    ANSELx registers
    */
    ANSELD = 0x00;
    ANSELC = 0x00;
    ANSELB = 0x80;
    ANSELE = 0x07;
    ANSELF = 0x3F;
    ANSELA = 0x00;

    /**
    WPUx registers
    */
    WPUD = 0x00;
    WPUF = 0x00;
    WPUE = 0x00;
    WPUB = 0x00;
    WPUA = 0x00;
    WPUC = 0x00;

    /**
    ODx registers
    */
    ODCONE = 0x00;
    ODCONF = 0x00;
    ODCONA = 0x00;
    ODCONB = 0x00;
    ODCONC = 0x01;
    ODCOND = 0x20;

    /**
    SLRCONx registers
    */
    SLRCONA = 0xDF;
    SLRCONB = 0xFF;
    SLRCONC = 0xDF;
    SLRCOND = 0xFF;
    SLRCONE = 0x03;
    SLRCONF = 0xFF;

    /**
    INLVLx registers
    */
    INLVLA = 0xFF;
    INLVLB = 0xFF;
    INLVLC = 0xDF;
    INLVLD = 0xFF;
    INLVLE = 0x0B;
    INLVLF = 0xFF;


    

// SCAN_BUTTON
 //   IOCAPbits.IOCAP5 = 1;   // rising edge
    IOCANbits.IOCAN5 = 1;   // falling edge
//MODE BUTTON
    IOCCNbits.IOCCN1 = 1;   // falling edge, RC1
// SELECT BUTTON   
//    IOCDNbits.IOCDN7 = 1;   // falling edge, RC1
    // Enable IOCI interrupt 
    PIE0bits.IOCIE = 1; 
   
	
		
	RB6PPS = 0x0D;   //RB6->EUSART1:TX1;    
	
    RD6PPS = 0x0C;   //RD6->PWM4:PWM4OUT;   	
		
    RD3PPS = 0x0B;   //RD3->PWM3:PWM3OUT;    
}
  
void PIN_MANAGER_IOC(void)
{   
    if(IOCCFbits.IOCCF1 == 1)
    {
        IOCCFbits.IOCCF1 = 0;
        if(MODE_BUTTON == LOW) 
        {
            mode_keyInterrupt.falling = 1;
        }
        else 
        {
             mode_keyInterrupt.rising = 1;
        }
        if(SCAN_BUTTON == LOW) 
        {
            scan_keyInterrupt.falling = 1;
        }
        else 
        {
            scan_keyInterrupt.rising = 1;
        }       
    }
}

/**
 End of File
*/