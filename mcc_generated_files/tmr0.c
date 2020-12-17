/**
  TMR0 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr0.c

  @Summary
    This is the generated driver implementation file for the TMR0 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for TMR0.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.4
        Device            :  PIC16LF19186
        Driver Version    :  3.10
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.20 and above
        MPLAB 	          :  MPLAB X 5.40
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "tmr0.h"
#include "mylib.h"

/**
  Section: Global Variables Definitions
*/

volatile uint16_t timer0ReloadVal16bit;

/**
  Section: TMR0 APIs
*/

void (*TMR0_InterruptHandler)(void);

void TMR0_Initialize(void)
{
    // Set TMR0 to the options selected in the User Interface

    // T0CS LFINTOSC; T0CKPS 1:1; T0ASYNC synchronised; 
    T0CON1 = 0x80;

    // TMR0H 243; 
    TMR0H = 0xF3;

    // TMR0L 228; 
    TMR0L = 0xE4;

    // Load TMR0 value to the 16-bit reload variable
    timer0ReloadVal16bit = (TMR0H << 8) | TMR0L;

    // Clear Interrupt flag before enabling the interrupt
    PIR0bits.TMR0IF = 0;

    // Enabling TMR0 interrupt.
    INTCONbits.GIE = 1;
    PIE0bits.TMR0IE = 1;

    // Set Default Interrupt Handler
    TMR0_SetInterruptHandler(TMR0_ISR);

    // T0OUTPS 1:1; T0EN enabled; T016BIT 16-bit; 
    T0CON0 = 0x90;
}

void TMR0_StartTimer(void)
{
    // Start the Timer by writing to TMR0ON bit
    T0CON0bits.T0EN = 1;
}

void TMR0_StopTimer(void)
{
    // Stop the Timer by writing to TMR0ON bit
    T0CON0bits.T0EN = 0;
}

uint16_t TMR0_ReadTimer(void)
{
    uint16_t readVal;
    uint8_t readValLow;
    uint8_t readValHigh;

    readValLow  = TMR0L;
    readValHigh = TMR0H;
    readVal  = ((uint16_t)readValHigh << 8) + readValLow;

    return readVal;
}

void TMR0_WriteTimer(uint16_t timerVal)
{
    // Write to the Timer0 register
    TMR0H = timerVal >> 8;
    TMR0L = (uint8_t) timerVal;
}

void TMR0_Reload(void)
{
    // Write to the Timer0 register
    TMR0H = timer0ReloadVal16bit >> 8;
    TMR0L = (uint8_t) timer0ReloadVal16bit;
}

uint8_t buzzerTimer =0;
void TMR0_ISR(void)
{
    // clear the TMR0 interrupt flag
    PIR0bits.TMR0IF = 0;
    // Write to the Timer0 register
    TMR0H = timer0ReloadVal16bit >> 8;
    TMR0L = (uint8_t) timer0ReloadVal16bit;
    tmr0Cnt++ ;
    if(tmr0Cnt % 20 == 0)
        OperationCheckTimer = ON;
    if( thermoTimer.T_chkBuzzer == 1 )
    {
        thermoTimer.chkBuzzerCnt++;
        if(thermoTimer.chkBuzzerCnt == 2 ) TRISDbits.TRISD3 = 0;
        if(thermoTimer.chkBuzzerCnt == 3 ) 
        {
            TRISDbits.TRISD3 = 1;
            thermoTimer.T_chkBuzzer = 0;
        }
    }
    if( thermoTimer.T_chkBuzzer == 2 )
    {
        thermoTimer.chkBuzzerCnt++;
        if(thermoTimer.chkBuzzerCnt == 2 ) TRISDbits.TRISD3 = 0;
        if(thermoTimer.chkBuzzerCnt == 3 ) TRISDbits.TRISD3 = 1;
        if(thermoTimer.chkBuzzerCnt == 4 ) TRISDbits.TRISD3 = 0;
        if(thermoTimer.chkBuzzerCnt == 5 ) 
        {
            TRISDbits.TRISD3 = 1;
            thermoTimer.T_chkBuzzer = 0;
        }
    }
//    if( thermoTimer.T_chkBuzzer == 3 )     // alarm over 38.4
    if(Tobject > 37.5 && thermoTimer.T_chkBuzzer == 0)
    {
        thermoTimer.chkBuzzerCnt++;
        if((thermoTimer.chkBuzzerCnt % 10) == 0 && thermoTimer.chkBuzzerCnt <= 50)
        {
            TRISDbits.TRISD3 = 0;
        }
        else
        {
            TRISDbits.TRISD3 = 1;
        }
        
    }

    if(tmr0Cnt % 4 == 0) blinkOn = 0;         
    else                blinkOn = 1;    

    if(tmr0Cnt % 50 == 0) thermoTimer.T_chkBattery = EXPIRE; 
    
    if(thermoTimer.T_chkMode == ON )
    {
        if(thermoTimer.chkModeCnt++ == 50)  //5sec
        {
            thermoTimer.T_chkMode = EXPIRE;
            thermoTimer.chkModeCnt = 0; 
        }
    }
    if(thermoTimer.T_chkTest == ON )
    {
        if(thermoTimer.chkTestCnt++ == 20)  //5sec
        {
            thermoTimer.T_chkTest = EXPIRE;
            thermoTimer.chkTestCnt = 0; 
        }
    }
    if(thermoTimer.T_chkSilent == ON )
    {
        if(thermoTimer.chkSilentCnt++ == 250)  //15sec
        {
            thermoTimer.T_chkSilent = EXPIRE;
            thermoTimer.chkSilentCnt = 0; 
        }
    }

    // Write to the Timer0 register
    TMR0H = timer0ReloadVal16bit >> 8;
    TMR0L = (uint8_t) timer0ReloadVal16bit;
   
    // add your TMR0 interrupt custom code
}


void TMR0_SetInterruptHandler(void (* InterruptHandler)(void))
{
    TMR0_InterruptHandler = InterruptHandler;
}

void TMR0_DefaultInterruptHandler(void)
{
    TMR0_ISR();
    // add your TMR0 interrupt custom code
    // or set custom function using TMR0_SetInterruptHandler()
}

/**
  End of File
*/
