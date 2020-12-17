/*
 * File:   temp.c
 * Author: shlee
 *
 * Created on 2020? 11? 30? (?), ?? 11:42
 */


#include "mylib.h"

double calObjTemp()
{
    double  Tcal, Tcom, Tadj;
 
    adc_result_t result = getTempFromSensor(T_OBJ_ADC_CH);   
    if(result >= REF_GET_ADC)
    {
        result = result - REF_GET_ADC; 
        Tcom = (result / (0.981 + (result * 0.000445)))/10;
        Tcal = REF_TEMP + Tcom;
    }
    else
    {
        result =  REF_GET_ADC - result; 
        Tcom = (result / (0.98758 + (result * 0.000697)))/10;
        Tcal = REF_TEMP - Tcom;

    }
    Tcal = Tcal + 3.3;
    return Tcal;
}

double calAmbTemp()
{
    double Tcalamb, Tamb;
    
    adc_result_t result = getTempFromSensor(T_AMB_ADC_CH);  
    if(result <= TAMB_REF_ADC )
    {
        Tcalamb = TAMB_REF_ADC - result;
        if( Tcalamb <= 224)                     Tamb = Tcalamb/44.7564;    //25~30
        if(Tcalamb > 224 && Tcalamb <= 435)     Tamb = Tcalamb/43.4560; 
        if(Tcalamb > 435 && Tcalamb <= 629)     Tamb = Tcalamb/41.9649;
        if(Tcalamb > 629 && Tcalamb <= 807)     Tamb = Tcalamb/40.3466; 
        if(Tcalamb > 807 && Tcalamb <= 966)     Tamb = Tcalamb/38.6570; //45~50
        if(Tcalamb > 966 && Tcalamb <= 1108)    Tamb = Tcalamb/36.9411;
        if(Tcalamb > 1108 && Tcalamb <= 1260)   Tamb = Tcalamb/35.9924; 
        if(Tcalamb > 1260 && Tcalamb <= 1343)   Tamb = Tcalamb/33.5661;
        if(Tcalamb > 1343 && Tcalamb <= 1438)   Tamb = Tcalamb/31.9539;
        if(Tcalamb > 1438 && Tcalamb <= 1521)   Tamb = Tcalamb/30.4113; 
        if(Tcalamb > 1521 && Tcalamb <= 1592)   Tamb = Tcalamb/28.9457; //75~ 80
        if(Tcalamb > 1592 && Tcalamb <= 1654)   Tamb = Tcalamb/27.5610; 
        if(Tcalamb > 1654 && Tcalamb <= 1707)   Tamb = Tcalamb/26.2582; 
        if(Tcalamb > 1707 && Tcalamb <= 1753)   Tamb = Tcalamb/25.0359; 
        if(Tcalamb > 1753 && Tcalamb <= 1792)   Tamb = Tcalamb/23.8917; //
        if(Tcalamb > 1792)                      Tamb = Tcalamb/23.8917;
        
        Tamb = TAMB_REF_TEMP + Tamb;
    }
    if(result > TAMB_REF_ADC )
    {
        Tcalamb = result - TAMB_REF_ADC ;
        if(Tcalamb <=234 )                      Tamb = Tcalamb/46.8974;
        if(Tcalamb > 234 && Tcalamb <= 460)     Tamb = Tcalamb/46.0280; //-10
        if(Tcalamb > 460 && Tcalamb <= 677)     Tamb = Tcalamb/45.1180;
        if(Tcalamb > 677 && Tcalamb <= 884)     Tamb = Tcalamb/44.2000;
        if(Tcalamb > 884 && Tcalamb <= 1082)    Tamb = Tcalamb/43.2876; //5~0
        if(Tcalamb > 1082 && Tcalamb <= 1257)   Tamb = Tcalamb/41.9084;
        if(Tcalamb > 1257 && Tcalamb <= 1410)   Tamb = Tcalamb/40.2827;
        if(Tcalamb > 1410 && Tcalamb <= 1541)   Tamb = Tcalamb/38.5138;
        if(Tcalamb > 1541 && Tcalamb <= 1651)   Tamb = Tcalamb/36.6783;
        if(Tcalamb > 1651)                      Tamb = Tcalamb/36.6783; 
 
        Tamb = TAMB_REF_TEMP - Tamb ;  // ???? 25?
    }
    return Tamb;
}

double adjustTemp(void)
{
    double Tcal, Tamb, Tdiff,Tcoeff, Tobj;
 
    Tcal = calObjTemp();
    Tamb = calAmbTemp();
 
    if(Tamb >= 25 /*TAMB_REF_TEMP*/)
    {
       
        Tcoeff = (Tamb - 25) * 0.04095;
        if(Tamb >= 27 && Tamb < 28 )    Tcoeff = (Tamb - 25) * 0.04595;
        if(Tamb >= 28 && Tamb < 29 )    Tcoeff = (Tamb - 25) * 0.04695;
        if(Tamb >= 29 && Tamb < 30 )    Tcoeff = (Tamb - 25) * 0.04795;
        if(Tamb >= 30 )                 Tcoeff = (Tamb - 25) * 0.04895;
        
        Tdiff = (0.851 - Tcoeff) * (Tamb - 25);
        Tdiff = Tdiff * 1.0;
        Tobj = Tcal + Tdiff;      
       
    }
    else
    {
        Tcoeff = (25 - Tamb) * 0.04595;
        Tdiff = (0.842 - Tcoeff) * (25 - Tamb);
        Tdiff = Tdiff * 1.0;
        Tobj = Tcal - Tdiff;   
    }
    Tobject = Tobj;
    return Tobj;
}
