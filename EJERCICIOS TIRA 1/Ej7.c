
#define MIPS 30
#include <p30F4013.h>		// si se cambia de DSC cambiar tb p30FXXXX.gld linker
#include <stdio.h>
#include <string.h>
#include <dsp.h>
#include <time.h>           // tic toc
#include "terminal.h"		// hiperterminal colores
#include "delays.h"

_FOSC(CSW_FSCM_OFF & XT_PLL16);  //Run this project using an external crystal
                                //routed via the PLL in 16x multiplier mode
                                //For the 7.3728 MHz crystal we will derive a
                                //throughput of 7.3728e+6*16/4 = 29.4 MIPS(Fcy)
                                //,~33.9 nanoseconds instruction cycle time(Tcy).
_FWDT(WDT_OFF);                 //Turn off the Watch-Dog Timer.
_FBORPOR(MCLR_EN & PWRT_OFF);   //Enable MCLR reset pin and turn off the
                                //power-up timers.
_FGS(CODE_PROT_OFF);            //Disable Code Protection

////--------------------------------------------

clock_t start, stop;
double V1f[3] = {2.1, -0.8, 0.9};
double V2f[3] = {0.8, 1.7, -0.6};
fractional V1[3] = {Q15(2.1/4), Q15(-0.8/4), Q15(0.9/4)}; 
fractional V2[3] = {Q15(0.8/4), Q15(1.7/4), Q15(-0.6/4)};   
fractional Vpv[3],Vps[3];
fractional sclVal={Q15(0.5)};
double Vpvf[3];
void multiplicacion();
int main(void) {
    multiplicacion();
    return (EXIT_SUCCESS);
}

void multiplicacion(){
    int i=0;
    double multip_float[3];
    static double count_dsp=0, count_float=0;
    start = clock();    
    VectorMultiply(3,Vpv,V1,V2);
    stop=clock();     
    count_dsp=stop-start-19.0;  
    for(i=0;i<=2;i++){
        Vpvf[i]=Fract2Float(Vpv[i]);    
        Vpvf[i]=Vpvf[i]*16;
    }
    printf("\nEl producto vectorial:[%f, %f, %f]\n",Vpvf[0],Vpvf[1],Vpvf[2]);
    //VectorScale(3,Vps,V1,sclVal); //no es necesario usarlo porque escalo en Q15
    start=clock();
    for(i=0;i<3;i++){
        multip_float[i]=V1f[i]*V2f[i];
    }
    stop=clock();
    count_float=stop-start-19.0;  
    for(i=0;i<3;i++){
        multip_float[i]=V1f[i]*V2f[i];
    }
    stop=clock();
    count_float=stop-start-19.0;  
    printf("Dsp se tarda con VectorMultiply: %.1f TCY\nPunto fotante tarda: %.1f TCY\n",count_dsp,count_float);
}