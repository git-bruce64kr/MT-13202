/**
  LCD Generated Driver API Header File
  
  @Company
    Microchip Technology Inc.

  @File Name
    lcd.h
	
  @Summary
    This is the generated header file for the LCD driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for LCD.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.4
        Device            :  PIC16LF19186
        Driver Version    :  2.01
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

#ifndef LCD_H
#define LCD_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdbool.h>
    
/**
    Section: Defines
*/ 
   
/**
    Defines: the segments pins
*/
#define SEG00      LCDSE0bits.SE00
#define SEG01      LCDSE0bits.SE01
#define SEG02      LCDSE0bits.SE02
#define SEG03      LCDSE0bits.SE03
#define SEG04      LCDSE0bits.SE04
#define SEG05      LCDSE0bits.SE05
#define SEG06      LCDSE0bits.SE06
#define SEG07      LCDSE0bits.SE07
#define SEG08      LCDSE1bits.SE08
#define SEG09      LCDSE1bits.SE09
#define SEG10      LCDSE1bits.SE10
#define SEG11      LCDSE1bits.SE11
#define SEG12      LCDSE1bits.SE12
#define SEG13      LCDSE1bits.SE13
#define SEG14      LCDSE1bits.SE14
#define SEG15      LCDSE1bits.SE15
#define SEG16      LCDSE2bits.SE16
#define SEG17      LCDSE2bits.SE17
#define SEG18      LCDSE2bits.SE18
#define SEG19      LCDSE2bits.SE19
#define SEG20      LCDSE2bits.SE20
#define SEG21      LCDSE2bits.SE21
#define SEG22      LCDSE2bits.SE22
#define SEG23      LCDSE2bits.SE23
#define SEG24      LCDSE3bits.SE24
#define SEG25      LCDSE3bits.SE25
#define SEG26      LCDSE3bits.SE26
#define SEG27      LCDSE3bits.SE27
#define SEG28      LCDSE3bits.SE28
#define SEG29      LCDSE3bits.SE29
#define SEG30      LCDSE3bits.SE30
#define SEG31      LCDSE3bits.SE31
#define SEG32      LCDSE4bits.SE32
#define SEG33      LCDSE4bits.SE33
#define SEG34      LCDSE4bits.SE34
#define SEG35      LCDSE4bits.SE35
#define SEG36      LCDSE4bits.SE36
#define SEG37      LCDSE4bits.SE37
#define SEG38      LCDSE4bits.SE38
#define SEG39      LCDSE4bits.SE39
#define SEG40      LCDSE5bits.SE40
#define SEG41      LCDSE5bits.SE41
#define SEG42      LCDSE5bits.SE42
#define SEG43      LCDSE5bits.SE43
#define SEG44      LCDSE5bits.SE44
#define SEG45      LCDSE5bits.SE45

#define NO_2_SYM00A      LCDDATA20bits.S19C3
#define NO_2_SYM00B      LCDDATA14bits.S19C2
#define NO_2_SYM00C      LCDDATA8bits.S19C1
#define NO_2_SYM00D      LCDDATA2bits.S19C0
#define NO_2_SYM00E      LCDDATA8bits.S20C1
#define NO_2_SYM00F      LCDDATA20bits.S20C3
#define NO_2_SYM00G      LCDDATA14bits.S20C2
#define BATT_SYM01A      LCDDATA0bits.S00C0
#define BATT_SYM01B      LCDDATA1bits.S11C0
#define BATT_SYM01C      LCDDATA15bits.S26C2
#define BATT_SYM01D      LCDDATA21bits.S26C3
#define DOT_SYM02      LCDDATA0bits.S02C0
#define BODY_SYM03      LCDDATA9bits.S26C1
#define FAR_SYM04      LCDDATA14bits.S20C2
#define CEL_SYM05      LCDDATA18bits.S00C3
#define NO_1_SYM06A      LCDDATA21bits.S24C3
#define NO_1_SYM06B      LCDDATA15bits.S24C2
#define NO_1_SYM06C      LCDDATA9bits.S24C1
#define NO_1_SYM06D      LCDDATA3bits.S24C0
#define NO_1_SYM06E      LCDDATA9bits.S25C1
#define NO_1_SYM06F      LCDDATA21bits.S25C3
#define NO_1_SYM06G      LCDDATA15bits.S25C2
#define NO_3_SYM07A      LCDDATA18bits.S06C3
#define NO_3_SYM07B      LCDDATA12bits.S06C2
#define NO_3_SYM07C      LCDDATA6bits.S06C1
#define NO_3_SYM07D      LCDDATA0bits.S06C0
#define NO_3_SYM07E      LCDDATA6bits.S07C1
#define NO_3_SYM07F      LCDDATA18bits.S07C3
#define NO_3_SYM07G      LCDDATA12bits.S07C2
#define NO_4_SYM08A      LCDDATA18bits.S01C3
#define NO_4_SYM08B      LCDDATA12bits.S01C2
#define NO_4_SYM08C      LCDDATA6bits.S01C1
#define NO_4_SYM08D      LCDDATA0bits.S01C0
#define NO_4_SYM08E      LCDDATA6bits.S02C1
#define NO_4_SYM08F      LCDDATA18bits.S02C3
#define NO_4_SYM08G      LCDDATA12bits.S02C2
#define NO_S2_SYM09A      LCDDATA19bits.S08C3
#define NO_S2_SYM09B      LCDDATA13bits.S08C2
#define NO_S2_SYM09C      LCDDATA7bits.S08C1
#define NO_S2_SYM09D      LCDDATA1bits.S08C0
#define NO_S2_SYM09E      LCDDATA7bits.S09C1
#define NO_S2_SYM09F      LCDDATA19bits.S09C3
#define NO_S2_SYM09G      LCDDATA13bits.S09C2
#define NO_S1_SYM10A      LCDDATA19bits.S10C3
#define NO_S1_SYM10B      LCDDATA13bits.S10C2
#define NO_S1_SYM10C      LCDDATA7bits.S10C1
#define NO_S1_SYM10D      LCDDATA1bits.S10C0
#define NO_S1_SYM10E      LCDDATA7bits.S11C1
#define NO_S1_SYM10F      LCDDATA19bits.S11C3
#define NO_S1_SYM10G      LCDDATA13bits.S11C2
#define SURFACE_SYM11      LCDDATA1bits.S11C0
#define SPK_SYM12      LCDDATA12bits.S00C2
#define BT_SYM13      LCDDATA3bits.S26C0
#define MEM_SYM14      LCDDATA1bits.S09C0
#define SMILE_SYM15      LCDDATA3bits.S25C0
#define ANGRY_SYM16      LCDDATA2bits.S20C0


/**
    Defines: To Turn on a pixel (a segment from a digit or a special character)
*/
#define NO_2_SYM00AON     NO_2_SYM00A = 1
#define NO_2_SYM00BON     NO_2_SYM00B = 1
#define NO_2_SYM00CON     NO_2_SYM00C = 1
#define NO_2_SYM00DON     NO_2_SYM00D = 1
#define NO_2_SYM00EON     NO_2_SYM00E = 1
#define NO_2_SYM00FON     NO_2_SYM00F = 1
#define NO_2_SYM00GON     NO_2_SYM00G = 1
#define BATT_SYM01AON     BATT_SYM01A = 1
#define BATT_SYM01BON     BATT_SYM01B = 1
#define BATT_SYM01CON     BATT_SYM01C = 1
#define BATT_SYM01DON     BATT_SYM01D = 1
#define DOT_SYM02ON     DOT_SYM02 = 1
#define BODY_SYM03ON     BODY_SYM03 = 1
#define FAR_SYM04ON     FAR_SYM04 = 1
#define CEL_SYM05ON     CEL_SYM05 = 1
#define NO_1_SYM06AON     NO_1_SYM06A = 1
#define NO_1_SYM06BON     NO_1_SYM06B = 1
#define NO_1_SYM06CON     NO_1_SYM06C = 1
#define NO_1_SYM06DON     NO_1_SYM06D = 1
#define NO_1_SYM06EON     NO_1_SYM06E = 1
#define NO_1_SYM06FON     NO_1_SYM06F = 1
#define NO_1_SYM06GON     NO_1_SYM06G = 1
#define NO_3_SYM07AON     NO_3_SYM07A = 1
#define NO_3_SYM07BON     NO_3_SYM07B = 1
#define NO_3_SYM07CON     NO_3_SYM07C = 1
#define NO_3_SYM07DON     NO_3_SYM07D = 1
#define NO_3_SYM07EON     NO_3_SYM07E = 1
#define NO_3_SYM07FON     NO_3_SYM07F = 1
#define NO_3_SYM07GON     NO_3_SYM07G = 1
#define NO_4_SYM08AON     NO_4_SYM08A = 1
#define NO_4_SYM08BON     NO_4_SYM08B = 1
#define NO_4_SYM08CON     NO_4_SYM08C = 1
#define NO_4_SYM08DON     NO_4_SYM08D = 1
#define NO_4_SYM08EON     NO_4_SYM08E = 1
#define NO_4_SYM08FON     NO_4_SYM08F = 1
#define NO_4_SYM08GON     NO_4_SYM08G = 1
#define NO_S2_SYM09AON     NO_S2_SYM09A = 1
#define NO_S2_SYM09BON     NO_S2_SYM09B = 1
#define NO_S2_SYM09CON     NO_S2_SYM09C = 1
#define NO_S2_SYM09DON     NO_S2_SYM09D = 1
#define NO_S2_SYM09EON     NO_S2_SYM09E = 1
#define NO_S2_SYM09FON     NO_S2_SYM09F = 1
#define NO_S2_SYM09GON     NO_S2_SYM09G = 1
#define NO_S1_SYM10AON     NO_S1_SYM10A = 1
#define NO_S1_SYM10BON     NO_S1_SYM10B = 1
#define NO_S1_SYM10CON     NO_S1_SYM10C = 1
#define NO_S1_SYM10DON     NO_S1_SYM10D = 1
#define NO_S1_SYM10EON     NO_S1_SYM10E = 1
#define NO_S1_SYM10FON     NO_S1_SYM10F = 1
#define NO_S1_SYM10GON     NO_S1_SYM10G = 1
#define SURFACE_SYM11ON     SURFACE_SYM11 = 1
#define SPK_SYM12ON     SPK_SYM12 = 1
#define BT_SYM13ON     BT_SYM13 = 1
#define MEM_SYM14ON     MEM_SYM14 = 1
#define SMILE_SYM15ON     SMILE_SYM15 = 1
#define ANGRY_SYM16ON     ANGRY_SYM16 = 1

/**
    Defines: To Turn off a pixel (a segment from a digit or a special character)
*/
#define NO_2_SYM00AOFF    NO_2_SYM00A = 0
#define NO_2_SYM00BOFF    NO_2_SYM00B = 0
#define NO_2_SYM00COFF    NO_2_SYM00C = 0
#define NO_2_SYM00DOFF    NO_2_SYM00D = 0
#define NO_2_SYM00EOFF    NO_2_SYM00E = 0
#define NO_2_SYM00FOFF    NO_2_SYM00F = 0
#define NO_2_SYM00GOFF    NO_2_SYM00G = 0
#define BATT_SYM01AOFF    BATT_SYM01A = 0
#define BATT_SYM01BOFF    BATT_SYM01B = 0
#define BATT_SYM01COFF    BATT_SYM01C = 0
#define BATT_SYM01DOFF    BATT_SYM01D = 0
#define DOT_SYM02OFF    DOT_SYM02 = 0
#define BODY_SYM03OFF    BODY_SYM03 = 0
#define FAR_SYM04OFF    FAR_SYM04 = 0
#define CEL_SYM05OFF    CEL_SYM05 = 0
#define NO_1_SYM06AOFF    NO_1_SYM06A = 0
#define NO_1_SYM06BOFF    NO_1_SYM06B = 0
#define NO_1_SYM06COFF    NO_1_SYM06C = 0
#define NO_1_SYM06DOFF    NO_1_SYM06D = 0
#define NO_1_SYM06EOFF    NO_1_SYM06E = 0
#define NO_1_SYM06FOFF    NO_1_SYM06F = 0
#define NO_1_SYM06GOFF    NO_1_SYM06G = 0
#define NO_3_SYM07AOFF    NO_3_SYM07A = 0
#define NO_3_SYM07BOFF    NO_3_SYM07B = 0
#define NO_3_SYM07COFF    NO_3_SYM07C = 0
#define NO_3_SYM07DOFF    NO_3_SYM07D = 0
#define NO_3_SYM07EOFF    NO_3_SYM07E = 0
#define NO_3_SYM07FOFF    NO_3_SYM07F = 0
#define NO_3_SYM07GOFF    NO_3_SYM07G = 0
#define NO_4_SYM08AOFF    NO_4_SYM08A = 0
#define NO_4_SYM08BOFF    NO_4_SYM08B = 0
#define NO_4_SYM08COFF    NO_4_SYM08C = 0
#define NO_4_SYM08DOFF    NO_4_SYM08D = 0
#define NO_4_SYM08EOFF    NO_4_SYM08E = 0
#define NO_4_SYM08FOFF    NO_4_SYM08F = 0
#define NO_4_SYM08GOFF    NO_4_SYM08G = 0
#define NO_S2_SYM09AOFF    NO_S2_SYM09A = 0
#define NO_S2_SYM09BOFF    NO_S2_SYM09B = 0
#define NO_S2_SYM09COFF    NO_S2_SYM09C = 0
#define NO_S2_SYM09DOFF    NO_S2_SYM09D = 0
#define NO_S2_SYM09EOFF    NO_S2_SYM09E = 0
#define NO_S2_SYM09FOFF    NO_S2_SYM09F = 0
#define NO_S2_SYM09GOFF    NO_S2_SYM09G = 0
#define NO_S1_SYM10AOFF    NO_S1_SYM10A = 0
#define NO_S1_SYM10BOFF    NO_S1_SYM10B = 0
#define NO_S1_SYM10COFF    NO_S1_SYM10C = 0
#define NO_S1_SYM10DOFF    NO_S1_SYM10D = 0
#define NO_S1_SYM10EOFF    NO_S1_SYM10E = 0
#define NO_S1_SYM10FOFF    NO_S1_SYM10F = 0
#define NO_S1_SYM10GOFF    NO_S1_SYM10G = 0
#define SURFACE_SYM11OFF    SURFACE_SYM11 = 0
#define SPK_SYM12OFF    SPK_SYM12 = 0
#define BT_SYM13OFF    BT_SYM13 = 0
#define MEM_SYM14OFF    MEM_SYM14 = 0
#define SMILE_SYM15OFF    SMILE_SYM15 = 0
#define ANGRY_SYM16OFF    ANGRY_SYM16 = 0
/**
  Section: Interface Routines
*/

void LCD_Initialize(void);
void LCD_Enable (void);
void LCD_Disable (void);
void LCD_EnableSleepMode (void);
void LCD_DisableSleepMode (void);
void LCD_SetContrast (unsigned int value);
void LCD_SetIntervalAPowerMode (unsigned int value);
void LCD_SetIntervalBPowerMode (unsigned int value);
void LCD_SetPowerDistribution (unsigned int value);
bool LCD_IsActive (void);
void LCD_DisplayOn_NO_2_SYM00Num();
void LCD_DisplayOff_NO_2_SYM00Num(void);    
void LCD_NO_2_SYM00Num (unsigned char num);
void LCD_DisplayOn_NO_1_SYM06Num();
void LCD_DisplayOff_NO_1_SYM06Num(void);    
void LCD_NO_1_SYM06Num (unsigned char num);
void LCD_DisplayOn_NO_3_SYM07Num();
void LCD_DisplayOff_NO_3_SYM07Num();    
void LCD_NO_3_SYM07Num (unsigned char num);
void LCD_DisplayOn_NO_4_SYM08Num();
void LCD_DisplayOff_NO_4_SYM08Num();    
void LCD_NO_4_SYM08Num (unsigned char num);
void LCD_DisplayOn_NO_S2_SYM09Num();
void LCD_DisplayOff_NO_S2_SYM09Num();    
void LCD_NO_S2_SYM09Num (unsigned char num);
void LCD_DisplayOn_NO_S1_SYM10Num();
void LCD_DisplayOff_NO_S1_SYM10Num();    
void LCD_NO_S1_SYM10Num (unsigned char num);
void LCD_Clear(void);
#endif /*_LCD_H*/