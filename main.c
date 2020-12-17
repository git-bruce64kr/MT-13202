/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.4
        Device            :  PIC16LF19186
        Driver Version    :  2.00
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
#include <xc.h>      // Processor specific header file
#include <stdio.h>   // Standard I/O - required for printf() function
#include <math.h>

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/adcc.h"
#include "mcc_generated_files/tmr0.h"
#include "mcc_generated_files/lcd.h"

#include "mcc_generated_files/mylib.h"

#define SINGLE_BUTTON

#define INPUT_LOW   0
#define OUTPUT_HIGH  1
#define HIGH    1
#define LOW     0

#define OK   1
#define NOK  0
#define TRUE   1
#define FALSE 0
#define PARAMS_ADDR 0xF0

typedef enum {
    THERMOSTATE_INIT = 0,
    THERMOSTATE_SETMODE ,
    THERMOSTATE_NORMAL,
    THERMOSTATE_TEST,        
    THERMOSTATE_STOREDREAD,
} thermoState_t ;

thermoState_t   runState = THERMOSTATE_NORMAL;

volatile uint16_t testCnt = 0;
volatile uint8_t percent = 80;

void testDisplay(void);
void modeDisplay(uint8_t mode);

void tempDisplay(void);
adc_result_t ADC_CalcAvg(adcc_channel_t channel) ;
adc_result_t ADC_CalcMax(adcc_channel_t channel); 

void setMode( void );
void writeEEPROM( uint8_t address, uint16_t data);
uint16_t readEEPROM( uint8_t address);
double celToFah( double temp);
void displayGetTobject( uint16_t temp);
uint8_t battMeasure(void);
void battDisplay(void);
uint16_t getMax( uint16_t *arr, uint8_t len);
uint16_t getMin( uint16_t *arr, uint8_t len);
void buzzer( uint8_t count);
void setColor(uint8_t color);

/*
                         Main application
 */

void main(void)
{
    uint8_t i = 0;
    uint8_t eeprom;
    adc_result_t getAdcObj, getAdcAmb; 
    uint8_t nextReadAddress = 0;
    uint16_t storedParms;

    // initialize the device
    SYSTEM_Initialize();
      
    MAIN_POWER = PWR_ON;     // main power
    
    setColor(GREEN);
    testDisplay(); 
    
    __delay_ms(1000);
// LCDCST Max contrast (Min Resistance); 
    LCDREF = 0x02;          // 3/7
    
    TMR0_StartTimer();
    thermoTimer.T_chkSilent = OFF;
    thermoTimer.T_chkBattery = OFF;
    scan_keyInterrupt.falling = OFF;
    scan_keyInterrupt.rising = OFF;
    mode_keyInterrupt.falling = OFF;
    storeTemp.disp = OFF;
    runState = THERMOSTATE_NORMAL;
#if 1
    if (CHK_EEPROM_DATA != eeprom_read(CHK_EEPROM_ADDR))
    {        
        eeprom_write(CHK_EEPROM_ADDR, CHK_EEPROM_DATA);
        eeprom_write(F0_ADDR, BODY);
        eeprom_write(F1_ADDR, CELSIUS);
        eeprom_write(F2_ADDR, SOUND);
        eeprom_write(F3_ADDR, 0);
        eeprom_write(F8_ADDR, ON);      // BLE power mode``
        
    }
#endif
    while (1)
    {
        switch (runState)
        {
            case THERMOSTATE_INIT :
                if(scan_keyInterrupt.falling == ON)
                {
                    thermoTimer.T_chkMode = ON;
                    thermoTimer.chkModeCnt = 0;
                    thermoTimer.T_chkSilent = ON;
                    thermoTimer.chkSilentCnt = 0;    // checking action when init
                }
                if(SCAN_BUTTON == HIGH && thermoTimer.T_chkMode == ON)
                {
                    runState = THERMOSTATE_NORMAL;
                    thermoTimer.T_chkMode = OFF;
                }
                if(SCAN_BUTTON == LOW && thermoTimer.T_chkMode == EXPIRE)
                {
                    thermoTimer.T_chkMode = OFF;
                    runState = THERMOSTATE_SETMODE;
                }
                if(mode_keyInterrupt.falling == ON )
                {
                    thermoTimer.chkTestCnt = 0; 
                    thermoTimer.T_chkTest = ON;
                }
                if(thermoTimer.T_chkTest == ON && MODE_BUTTON == HIGH)
                {
                    storeTemp.num++;
                    storeTemp.real = eeprom_read(F3_ADDR + storeTemp.num );
                    storeTemp.disp = ON;
                    tempDisplay(); 
                    storeTemp.disp = OFF;
                    thermoTimer.T_chkTest = OFF;
                    mode_keyInterrupt.falling = OFF;
                }
                if(thermoTimer.T_chkTest == EXPIRE && MODE_BUTTON == HIGH)
                {
                    runState = THERMOSTATE_TEST;
                    thermoTimer.T_chkTest = OFF;
                    mode_keyInterrupt.falling = OFF;
                }
                if(thermoTimer.T_chkSilent == EXPIRE)
                {
#if 0
                    LATDbits.LATD4 = LOW;     // main power
                    LATDbits.LATD7 = HIGH;     // BLE power
#endif
                    setColor(COLOR_OFF);
                    LCD_Disable();
                }
#if 1
                if(percent == 0) if( blinkOn == 0) LCD_Clear();
                battDisplay();
#else
                if(thermoTimer.T_chkBattery == EXPIRE)
                {
                    while(!EUSART1_is_tx_ready()) ;
                    EUSART1_Write(0xbb);
                    while(!EUSART1_is_tx_ready()) ;
                    EUSART1_Write(battMeasure());               //    battDisplay();
                    while(!EUSART1_is_tx_ready()) ;
                    EUSART1_Write(0xcc);
                    thermoTimer.T_chkBattery = OFF;
                }
#endif
                break;
            case THERMOSTATE_SETMODE :
                LCD_Clear();
                buzzer(1);
                setMode();
                break;
            case THERMOSTATE_NORMAL:
                LCD_Enable();
                LCD_Clear();
                if(percent > 0 ) tempDisplay();
                if(percent == 0) if( blinkOn == 0) LCD_Clear();
                battDisplay();
                scan_keyInterrupt.falling = OFF; //after display flag off
                buzzer(2);
                runState = THERMOSTATE_INIT;
                break;
            case THERMOSTATE_TEST :
                if(OperationCheckTimer == ON)
                {
#if 0
                    while(!EUSART1_is_tx_ready()) ;
                        EUSART1_Write(0xaa);
                        while(!EUSART1_is_tx_ready()) ;
                        EUSART1_Write(3);
                         
                        while(!EUSART1_is_tx_ready()) ;
                        EUSART1_Write(4);
                        while(!EUSART1_is_tx_ready());
                        EUSART1_Write(5);
                        while(!EUSART1_is_tx_ready());
                        EUSART1_Write(6);
                        while(!EUSART1_is_tx_ready()) ;
                        EUSART1_Write(0xcc);
#endif
                        OperationCheckTimer = OFF;
                }
                break;
            default:
                break;
        }
    }
}

void setMode( void )
{
    uint8_t modeStep = 0; 
    
    thermoTimer.chkSilentCnt = 0;    // checking action when setMode
    thermoTimer.T_chkSilent = ON;    

    mode_keyInterrupt.falling = OFF;     
    
    setDisplay.mode0 = eeprom_read(F0_ADDR);
    setDisplay.mode1 = eeprom_read(F1_ADDR);
    setDisplay.mode2 = eeprom_read(F2_ADDR);
    setDisplay.toggle = OFF;
    modeDisplay(modeStep);
    while(1)
    {
        if(scan_keyInterrupt.falling == ON)
        {
            thermoTimer.T_chkMode = ON;
            thermoTimer.chkModeCnt = 0;
            thermoTimer.T_chkSilent = ON;
            thermoTimer.chkSilentCnt = 0;    // checking action when init
        }
        switch(modeStep)
        {
            case 0 :    //F1
                if(eeprom_read(F0_ADDR == SURFACE )) 
                {
                    if( blinkOn == 1)  SURFACE_SYM11ON;  //surface
                    else               SURFACE_SYM11OFF;  //surface
                }
                else 
                {
                    if( blinkOn == 1)  BODY_SYM03ON; //body
                    else               BODY_SYM03OFF; //body
                }
                if(setDisplay.toggle == ON)
                {
                    if(eeprom_read(F0_ADDR == SURFACE ))    eeprom_write(F0_ADDR, BODY);
                    else                                    eeprom_write(F0_ADDR, BODY);
                    buzzer(1);
                    setDisplay.toggle = OFF;
                }
                break;
            case 1 : //F2
                if(eeprom_read(F1_ADDR == CELSIUS ))  
                {
                    if( blinkOn == 1)  CEL_SYM05ON ;  //celsius
                    else               CEL_SYM05OFF;  //celsius
                }
                else  
                {
                    if( blinkOn == 1)  FAR_SYM04ON;  //
                    else               FAR_SYM04OFF ;  //
                }
                if(setDisplay.toggle == ON)
                {
                    if(eeprom_read(F1_ADDR == CELSIUS)) eeprom_write(F1_ADDR,FAHRENHEIT);
                    else                                eeprom_write(F1_ADDR, CELSIUS);
                    
                    buzzer(1);
                    setDisplay.toggle = OFF;
                }
                break;
            case 2 : //F3   
                if(eeprom_read(F2_ADDR) == SOUND)
                {
                    if( blinkOn == 1) SPK_SYM12ON;
                    else              SPK_SYM12OFF;  
                }
                else
                {
                    SPK_SYM12OFF;
                }
                if(setDisplay.toggle == ON)
                {
                    if(eeprom_read(F2_ADDR) == NO_SOUND)    eeprom_write(F2_ADDR,SOUND);
                    else                                    eeprom_write(F2_ADDR, NO_SOUND);
                    buzzer(1);
                    setDisplay.toggle = OFF;
                }
                break;
            case 3 : // F4 memory
                storeTemp.disp = ON;
                while(1)
                {
                    storeTemp.num = eeprom_read(F3_ADDR);
                    storeTemp.real = eeprom_read(F3_ADDR + storeTemp.num );

                    tempDisplay(); 
                    storeTemp.disp = OFF;
                    while(1)
                    {
                        if(SELECT_BUTTON == LOW) 
                        {
                            storeTemp.num--;
                            break;
                        }
                        if(mode_keyInterrupt.falling == ON)
                        {
                            storeTemp.num++;
                            mode_keyInterrupt.falling = OFF;
                            break;
                        }
                    }
                }                
                break;
            default:
                break;
        }
        if(thermoTimer.T_chkMode == ON && SCAN_BUTTON == HIGH)
        {
            modeStep++;
            buzzer(1);
            if(modeStep == 4)
            {
                runState = THERMOSTATE_INIT;
                scan_keyInterrupt.falling = OFF;
                return;
            }
            else
            {
                LCD_Clear();    
                modeDisplay(modeStep);
            }
            
            thermoTimer.T_chkMode = OFF;
            scan_keyInterrupt.falling = OFF;
        }
        if(thermoTimer.T_chkMode == EXPIRE && SCAN_BUTTON == LOW)
        {
            runState = THERMOSTATE_INIT;
            thermoTimer.T_chkMode = OFF;
            scan_keyInterrupt.falling = OFF;
            return;
        }
        if(mode_keyInterrupt.falling == ON)
        {
            setDisplay.toggle = ON;
            mode_keyInterrupt.falling = OFF;                   
        }
        if(thermoTimer.T_chkSilent == EXPIRE)
        {
 #if 0
                    LATDbits.LATD4 = LOW;     // main power
                    LATDbits.LATD7 = HIGH;     // BLE power
#endif
            setColor(COLOR_OFF);
            LCD_Disable();
        }
    }
}
double celToFah( double temp)
{
    return (1.8 * temp + 32);
}
double fahToCel( double temp)
{
    return ( ((temp - 32)/1.8));
}

void modeDisplay(uint8_t mode)
{
    switch(mode)
    {
        case 0 :
            //F
            NO_S1_SYM10AON; NO_S1_SYM10BOFF; NO_S1_SYM10COFF; NO_S1_SYM10DOFF; NO_S1_SYM10EON; NO_S1_SYM10FON; NO_S1_SYM10GON;
            //1
            NO_S2_SYM09AOFF; NO_S2_SYM09BON; NO_S2_SYM09CON; NO_S2_SYM09DOFF; NO_S2_SYM09EOFF; NO_S2_SYM09FOFF; NO_S2_SYM09GOFF;
            break;
        case 1 :    
            //F
            NO_S1_SYM10AON; NO_S1_SYM10BOFF; NO_S1_SYM10COFF; NO_S1_SYM10DOFF; NO_S1_SYM10EON; NO_S1_SYM10FON; NO_S1_SYM10GON;
            //2
            NO_S2_SYM09AON; NO_S2_SYM09BON; NO_S2_SYM09COFF; NO_S2_SYM09DON; NO_S2_SYM09EON; NO_S2_SYM09FOFF; NO_S2_SYM09GON;
            NO_1_SYM06GON; NO_2_SYM00GON;  NO_3_SYM07GON; NO_4_SYM08GON;
            break;
        case 2 :
            //F
            NO_S1_SYM10AON; NO_S1_SYM10BOFF; NO_S1_SYM10COFF; NO_S1_SYM10DOFF; NO_S1_SYM10EON; NO_S1_SYM10FON; NO_S1_SYM10GON;
            //3
            NO_S2_SYM09AON; NO_S2_SYM09BON; NO_S2_SYM09CON; NO_S2_SYM09DON; NO_S2_SYM09EOFF; NO_S2_SYM09FOFF; NO_S2_SYM09GON;
            NO_1_SYM06GON; NO_2_SYM00GON;  NO_3_SYM07GON; NO_4_SYM08GON;
            break;
        case 3 :
            //F
            NO_S1_SYM10AON; NO_S1_SYM10BOFF; NO_S1_SYM10COFF; NO_S1_SYM10DOFF; NO_S1_SYM10EON; NO_S1_SYM10FON; NO_S1_SYM10GON;
            //4
            NO_S2_SYM09AOFF; NO_S2_SYM09BON; NO_S2_SYM09CON; NO_S2_SYM09DOFF; NO_S2_SYM09EOFF; NO_S2_SYM09FON; NO_S2_SYM09GON;
            LCD_DisplayOff_NO_1_SYM06Num(); 
            LCD_DisplayOff_NO_2_SYM00Num();
            LCD_NO_3_SYM07Num(0);
            LCD_NO_4_SYM08Num(0);
            DOT_SYM02ON;
            break;

        default :
            break;
}    
}

void tempDisplay(void)
{
    uint8_t i = 0;
    uint16_t realPart;

    uint8_t firstNum = 0;
    uint8_t secondNum = 0;
    uint8_t thirdNum = 0;
    uint8_t fourthNum = 0;

    if(storeTemp.disp == OFF)
    {    
        Tobject = adjustTemp();
    //    Tobject = roundf(Tobject * 10) / 10;   

        if(20 <= Tobject && Tobject <= 42)
        {
            storeTemp.real = (Tobject - 20) * 10;
            storeTemp.num = eeprom_read(F3_ADDR);
            if(storeTemp.num++ == 31) storeTemp.num = 1;
            eeprom_write(F3_ADDR + storeTemp.num, storeTemp.real);
            eeprom_write(F3_ADDR, storeTemp.num);
        }
    }
    else
    {
        Tobject = (storeTemp.real / 10 ) + 20;
    }
    if(eeprom_read(F1_ADDR) == FAHRENHEIT)
    {
        Tobject = celToFah(Tobject);
        realPart = (uint16_t)(Tobject * 10); 
    }
    else
        realPart = (uint16_t)(Tobject * 10);    

    while (realPart != 0)
    {
        if(i == 0) fourthNum = realPart % 10;
        if(i == 1) thirdNum = realPart % 10;
        if(i == 2) secondNum = realPart % 10;
        if(i == 3) firstNum = realPart % 10;
        realPart /= 10;
        i++;
    }

    if(Tobject >= 0)
    {
        if(firstNum == 0)   LCD_DisplayOff_NO_1_SYM06Num();
        else                LCD_NO_1_SYM06Num(firstNum);
                            
    }
    else
    {
            NO_1_SYM06GON;
    }
    if(firstNum == 0 && secondNum == 0 )  LCD_DisplayOff_NO_2_SYM00Num();
    else                                  LCD_NO_2_SYM00Num(secondNum); 
    LCD_NO_3_SYM07Num(thirdNum);
    LCD_NO_4_SYM08Num(fourthNum);

#if 0    
    LCD_NO_S1_SYM10Num(thirdNum1);
    LCD_NO_S2_SYM09Num(fourthNum1);
#endif

    DOT_SYM02ON; 
    if (eeprom_read(F0_ADDR) == SURFACE )   SURFACE_SYM11ON;
    else                                    BODY_SYM03ON;  
    if (eeprom_read(F1_ADDR) == CELSIUS )   CEL_SYM05ON;
    else                                    FAR_SYM04ON;
    if (eeprom_read(F2_ADDR) == SOUND )     SPK_SYM12ON;
        
    MEM_SYM14ON;
    LCD_NO_S1_SYM10Num(storeTemp.num % 10);
    LCD_NO_S2_SYM09Num((storeTemp.num / 10) % 10);
    
#if 1
    if(Tobject <= 37.0)
    {
        setColor(GREEN); //R1:200, R2:39k
        SMILE_SYM15ON;
    }    
    if(Tobject > 37.0 && Tobject <= 37.5)   setColor(ORANGE); //orange
    if(Tobject > 37.5)                      setColor(RED);    
#endif
#if 1
    while(!EUSART1_is_tx_ready()) ;
    EUSART1_Write(0xaa);
    while(!EUSART1_is_tx_ready()) ;
    EUSART1_Write(firstNum);
                 
    while(!EUSART1_is_tx_ready()) ;
    EUSART1_Write(secondNum);
    while(!EUSART1_is_tx_ready());
    EUSART1_Write(thirdNum);
    while(!EUSART1_is_tx_ready());
    EUSART1_Write(fourthNum);
    while(!EUSART1_is_tx_ready()) ;
    EUSART1_Write(0xcc);
#endif
}

void setColor(uint8_t color)
{
    switch(color)
    {
        case GREEN :
            LATDbits.LATD5 = LOW;    //green
            LATCbits.LATC0 = HIGH;   //red
            break;
        case ORANGE :
            LATDbits.LATD5 = LOW;    //green
            LATCbits.LATC0 = LOW;        //red   
            break;
        case RED :
            LATDbits.LATD5 = HIGH;    //green
            LATCbits.LATC0 = LOW;        //red
            break;
        case COLOR_OFF :
            LATDbits.LATD5 = HIGH;    //green
            LATCbits.LATC0 = HIGH;        //red
            break;
        default :
            break;
    }
}

adc_result_t getTempFromSensor(uint8_t channel)
{
    //return(ADCC_GetSingleConversion(channel_ANE0));
//    uint16_t adc_value[10];
#if 1
     __delay_ms(50);
    if(channel == 0)    return(ADC_CalcAvg(channel_ANE0));  //t_obj
    else                return(ADC_CalcAvg(channel_ANE1));  //t_amb
#else
     __delay_ms(50);
    if(channel == 0)    return(ADC_CalcMax(channel_ANE0));
    else                return(ADC_CalcMax(channel_ANE1));
#endif
}

adc_result_t ADC_CalcAvg(adcc_channel_t channel) 
{
    
    adc_result_t AIP1_Total = 0, InitialAIP1_Avg = 0;
    uint8_t num = 20;
    for (int x = 0; x < num; x++) 
    {
        __delay_ms(10);
        AIP1_Total += ADCC_GetSingleConversion(channel);
    }
    InitialAIP1_Avg = ((AIP1_Total / num));
    return(InitialAIP1_Avg);
}

adc_result_t ADC_CalcMax(adcc_channel_t channel) 
{
    uint16_t getADC[20] = {0};
    for(uint8_t i=0; i<20 ; i++)
    {
        __delay_ms(10);
        getADC[i] = ADCC_GetSingleConversion(channel);
        
    }
    return(getMax(getADC, 20));
}
uint16_t getMax( uint16_t *arr, uint8_t len)
{
    uint16_t i;
    uint16_t max = *arr;

    for(i = 0; i < len; i++)
    {
        if(*(arr + i) > max) max = *(arr + i);
    }
    return max ;
}
uint16_t getMin( uint16_t *arr, uint8_t len)
{
    uint16_t i;
    uint16_t min = *arr;
 
    for(i = 0; i< len; i++)
    {
        if(*(arr + i) < min) min = *(arr + i);
    }
    return min;
}


uint16_t getMiddle( uint16_t list[], uint8_t len)
{
    uint16_t i, j, temp;
 
 
    for(i=0;i<len;i++)  //?? 10? ?? ???? !! (??? 10?????)
    {       
        for(j=0;j<(len-1);j++)
        {        
            if(list[j] < list[j+1]) //val[0] ?? val[10]?? 2?? ???? ???? ????? ???
            {   
                swap(list[j], list[j+1], temp);
            }
        }
    }
    return(list[len/2]);
}


/*********************************************************************
 * @fn      battMeasure
 *
 * @brief   Measure the battery level with the ADC and return
 *          it as a percentage 0-100%.
 *
 * @return  Battery level.
 */

// Min voltage (mV)
#define BATT_MIN_VOLTAGE            1320    //2000  // 1800

// Max voltage (mV)
#define BATT_MAX_VOLTAGE            1920    //3000    //3273

// Minimum battery level.
static uint16_t battMinLevel = BATT_MIN_VOLTAGE;

// Maximum battery level.
static uint16_t battMaxLevel = BATT_MAX_VOLTAGE;

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b)) 

uint8_t battMeasure(void)
{
    int32_t pre_percent;
    float batt;

    // Read the battery voltage (V), only the first 12 bits
    pre_percent = ADCC_GetSingleConversion(channel_ANE2);
    batt = (pre_percent*((float)2.495 /(float)4096)) ;  // 3.3v --> 2.495
    // Convert to from V to mV to avoid fractions.
    // Fractional part is in the lower 8 bits thus converting is done as follows:
    // (1/256)/(1/1000) = 1000/256 = 125/32
    // This is done most effectively by multiplying by 125 and then shifting
    // 5 bits to the right.
#if 1
    batt *= 1000;
#else
    pre_percent = ( ((pre_percent>>8)&0x7)*1000 + (pre_percent&0xFF)*1000/256 );
#endif
    // Convert to percentage of maximum voltage.
    if(batt > battMinLevel)
        percent = (((batt - battMinLevel) / (battMaxLevel - battMinLevel))) *100;
    else
        percent = 0;
    
    return percent;
}
void battDisplay(void)
{
#if 0
    int32_t pre_percent;
    float batt;

    // Read the battery voltage (V), only the first 12 bits
    pre_percent = ADCC_GetSingleConversion(channel_ANE2);
    batt = (pre_percent*((float)2.495 /(float)4096)) ;  // 3.3v --> 2.495
    // Convert to from V to mV to avoid fractions.
    // Fractional part is in the lower 8 bits thus converting is done as follows:
    // (1/256)/(1/1000) = 1000/256 = 125/32
    // This is done most effectively by multiplying by 125 and then shifting
    // 5 bits to the right.
#if 1
    batt *= 1000;
#else
    pre_percent = ( ((pre_percent>>8)&0x7)*1000 + (pre_percent&0xFF)*1000/256 );
#endif
    // Convert to percentage of maximum voltage.
    if(batt > battMinLevel)
        percent = (((batt - battMinLevel) / (battMaxLevel - battMinLevel))) *100;
    else
        percent = 0;
#endif    
    battMeasure();
    
    // Make sure the value is 1~100%
 //   percent = (uint8_t) MAX(1, MIN(pre_percent, 100));
    // if( DISALBE_UART)Log_info1("battery read value:%d", percent);
    if(percent >= 75) 
    {
        BATT_SYM01AON; BATT_SYM01BON; BATT_SYM01CON; BATT_SYM01DON; 
    }
    if(percent >= 40 && percent <75)
    {
        BATT_SYM01AON; BATT_SYM01BON; BATT_SYM01CON; BATT_SYM01DOFF; 
    }
    if(percent >= 10 && percent < 40)
    {
        BATT_SYM01AON; BATT_SYM01BON; BATT_SYM01COFF; BATT_SYM01DOFF; 
    }
    if(percent > 0 && percent < 10) 
    {
        BATT_SYM01AON; BATT_SYM01BOFF; BATT_SYM01COFF; BATT_SYM01DOFF; 
    }
    if( percent == 0) 
    {
        if(blinkOn == 1)
        {
            BATT_SYM01AON; BATT_SYM01BOFF; BATT_SYM01COFF; BATT_SYM01DOFF; 
        }
        else
        {
            BATT_SYM01AOFF; BATT_SYM01BOFF; BATT_SYM01COFF; BATT_SYM01DOFF; 
        }
        NO_1_SYM06GON; NO_2_SYM00GON;  NO_3_SYM07GON; NO_4_SYM08GON;
    }
}

void testDisplay(void)
{
    CEL_SYM05ON; FAR_SYM04ON; BODY_SYM03ON; DOT_SYM02ON; SURFACE_SYM11ON;
    SPK_SYM12ON; MEM_SYM14ON; SMILE_SYM15ON; ANGRY_SYM16ON;
    {
    BATT_SYM01AON; BATT_SYM01BON; BATT_SYM01CON; BATT_SYM01DON; 
    }
#if 0
    for( uint8_t j = 0 ; j<= 8 ;j++)
#else
    for( uint8_t j = 8 ; j<= 8 ;j++)
#endif    
    {
 
        LCD_NO_1_SYM06Num(j);
        LCD_NO_2_SYM00Num(j);
        LCD_NO_3_SYM07Num(j);
        LCD_NO_4_SYM08Num(j);
        LCD_NO_S1_SYM10Num(j);
        LCD_NO_S2_SYM09Num(j);
    }

}

void buzzer( uint8_t count)
{
    thermoTimer.chkBuzzerCnt = 0;
    thermoTimer.T_chkBuzzer = count;
 }
/**
 End of File
*/
