
#include "adcc.h"

double calObjTemp(void);
double calAmbTemp(void);
double adjustTemp(void);
adc_result_t getTempFromSensor(uint8_t channel);
void changeMode(void);
// for object temp
#if 1 
#define REF_TEMP    37.0
#define REF_GET_ADC 1381 // 37.0 (27k:2.495V) 1590  //36.5 //1728 //1596(36.0)) 1622(37.0))
#else  //BODY
#define REF_TEMP    30.0
#define REF_GET_ADC 1466/REF_STEP_ADC  //30.0
#endif

//#define REF_ADC     1.381    // 1.238
#define STEP_ADC    2.495/4096  //0.0008

#define REF_STEP_ADC 1.0269  //R1:200, R2:39k  //3.665

// for ambient temp
/*
    Tamb = 25 ==> 15.54684 + 1.45774 = 17.00458 mv
    1 step : 17.00458 / 4096 = 0.00415 mv
    1 step : 270 / 4096 = 0.066
    
 */
#define TAMB_REF_ADC  2048  //2982 //3292 //3132 + 3.6(140)
#define TAMB_REF_TEMP 25    // 0.0


#define T_OBJ_ADC_CH    0
#define T_AMB_ADC_CH    1

#define GREEN   0
#define ORANGE  1
#define RED     2
#define COLOR_OFF   3

#define CHK_EEPROM_DATA 0x5A
#define CHK_EEPROM_ADDR 0
#define F0_ADDR 2
#define F1_ADDR 4
#define F2_ADDR 6
#define F8_ADDR 8
#define F3_ADDR 10

#define BODY    0
#define SURFACE 1
#define CELSIUS     0
#define FAHRENHEIT  1
#define SOUND     0
#define NO_SOUND  1

 double  Tobject = 0;

 typedef struct {
   uint8_t num;
   uint8_t real;
   double  disp;
} mem_store_t;
mem_store_t storeTemp;
 
typedef struct {
   uint8_t mode0;
   uint8_t mode1;
   uint8_t mode2;
   uint8_t mode3;
   uint8_t toggle;
} display_t;

display_t setDisplay;

#define swap(x,y,t) ((t)=(x),(x)=(y),(y)=(t))


#define PWR_ON  1
#define PWR_OFF 0
#define SCAN_BUTTON PORTAbits.RA5
#define MODE_BUTTON PORTCbits.RC1
#define SELECT_BUTTON PORTDbits.RD7

#define MAIN_POWER  LATDbits.LATD4
