
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
double V1f[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
double V2f[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
fractional V1[16] = {Q15(0.0/15),Q15(1.0/15),Q15(2.0/15),Q15(3.0/15),Q15(4.0/15),Q15(5.0/15),Q15(6.0/15),Q15(7.0/15),Q15(8.0/15),Q15(9.0/15),Q15(10.0/15),Q15(11.0/15),Q15(12.0/15),Q15(13.0/15),Q15(14.0/15),Q15(15.0/15)}; //Divido todo por 15 para que todos los numeros me queden menor a 1
fractional V2[16] = {Q15(0.0/15),Q15(1.0/15),Q15(2.0/15),Q15(3.0/15),Q15(4.0/15),Q15(5.0/15),Q15(6.0/15),Q15(7.0/15),Q15(8.0/15),Q15(9.0/15),Q15(10.0/15),Q15(11.0/15),Q15(12.0/15),Q15(13.0/15),Q15(14.0/15),Q15(15.0/15)};
fractional Vpv[16];
double Vpvf[16];
void multiplicacion();
int main(void) {
    multiplicacion();
    return (EXIT_SUCCESS);
}

void multiplicacion(){
    int i=0;
    double multip_float[16];
    static double count_dsp=0, count_float=0;
    start = clock();    
    VectorMultiply(16,Vpv,V1,V2);
    stop=clock();      
    count_dsp=stop-start-19.0;  
    for(i=0;i<=15;i++){
        Vpvf[i]=Fract2Float(Vpv[i]);    
        Vpvf[i]=Vpvf[i]*225;
    }
    printf("\nProducto vectorial:[%f, %f, %f %f, %f, %f %f, %f, %f %f, %f, %f %f, %f, %f %f,]\n",Vpvf[0],Vpvf[1],Vpvf[2],Vpvf[3],Vpvf[4],Vpvf[5],Vpvf[6],Vpvf[7],Vpvf[8],Vpvf[9],Vpvf[10],Vpvf[11],Vpvf[12],Vpvf[14],Vpvf[14],Vpvf[15]); 
    start=clock();
    for(i=0;i<16;i++){
        multip_float[i]=V1f[i]*V2f[i];
    }
    stop=clock();
    count_float=stop-start-19.0;  
    for(i=0;i<16;i++){
        multip_float[i]=V1f[i]*V2f[i];
    }
    stop=clock();
    count_float=stop-start-19.0;  
    printf("Demora DSP: %.1f TCY\nDemora pto flotante: %.1f TCY\n",count_dsp,count_float);    
}