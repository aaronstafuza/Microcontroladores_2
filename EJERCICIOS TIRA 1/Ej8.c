
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

void rta_impulsiva(void);
void rta_escalon(void);
double Ts=0.005;
int muestras=50;


int main(void) {

    rta_impulsiva();
    //RespuestaEscalon();

    return 0;
}

void rta_impulsiva(void){
    printf("Valores rta impulsiva:\n");
    int i=0;
    fractional y=Q15(0);
    fractional val[3]={Q15(1),Q15(0),Q15(0)};
    fractional coef[3]={Q15(0),Q15(0.04877),Q15(0.9512294245)};
    for(i=0;i<muestras;i++){
        y=VectorDotProduct(3,val,coef);
        val[1]=val[0]; 
        val[0]=0;  
        val[2]=y;    
        printf("%f\n",(1/Ts)*Fract2Float(y)); 
    }
}

void rta_escalon(void){
    printf("Valores rta al escalon:\n");
    int i=0;
    fractional y=Q15(0);
    fractional val[3]={Q15(1),Q15(0),Q15(0)};
    fractional coef[3]={Q15(0),Q15(0.04877),Q15(0.9512294245)};
    for(i=0;i<muestras;i++){
        y=VectorDotProduct(3,val,coef);
        val[1]=val[0];  /
        val[0]=Q15(1);  
        val[2]=y;   
        printf("%f\n",Fract2Float(y)); 
    }
}

