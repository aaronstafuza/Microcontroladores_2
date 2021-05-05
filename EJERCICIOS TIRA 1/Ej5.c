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

////-------------------------------------------
#define N_Muestras 15
#define CICLOS 2
clock_t start, stop;
static double count=0;
unsigned int i,j;
void calculo_Y(void);
void Y_DSP(void);

int main (void){
    printf("Y con pto flotante:\n\r");
    ptoflotante();
    printf("Y con DSP:\n\r");
    Y_DSP();
    while(1);
}

void ptoflotante(void){
    float ec[7]={0.0006,0.0018,0.0018,0.0006,3,-3,1}; 
    float impulso[7]={7.5,0,0,0,0,0,0};
    float Y[N_Muestras*CICLOS]={0}; //Un ciclo van a ser 15 muestras
    start=clock();
    for(i=0;i<=((N_Muestras*CICLOS)-1);i++){
        if ((i%N_Muestras)==0){
            impulso[0]=7.5;
            for (j=1;j<7;j++){
                impulso[j]=0;
            }
        }
        for(j=0;j<7;j++){
            Y[i]=Y[i]+impulso[j]*ec[j];
        }

        impulso[3]=impulso[2];
        impulso[2]=impulso[1];
        impulso[1]=impulso[0];
        impulso[0]=0;

        impulso[6]=impulso[5];
        impulso[5]=impulso[4];
        impulso[4]=Y[i];
    }
    stop=clock();
    count=(stop-start)-19.0;
	printf("# TCY: %.1f\r\n",count);
    for(i=0;i<=((N_Muestras*CICLOS)-1);i++){
        printf("%.4f\r\n",Y[i]);
    }
}

void Y_DSP(void){
    fractional ecDsp[7]={Q15(0.0006/3),Q15(0.0018/3),Q15(0.0018/3),Q15(0.0006/3),Q15(3/3),Q15(-3/3),Q15(1/3)};
    fractional impulsoDsp[7]={Q15(1),Q15(0),Q15(0),Q15(0),Q15(0),Q15(0),Q15(0)};
    fractional YDsp[N_Muestras*CICLOS]={Q15(0)};
    start=clock();
    for(i=0;i<=((N_Muestras*CICLOS)-1);i++){
        if ((i%N_Muestras)==0){ 
            impulsoDsp[0]=Q15(1);
            for (j=1;j<7;j++){
                impulsoDsp[j]=Q15(0);
            }
        }
        YDsp[i]=VectorDotProduct(7,ecDsp,impulsoDsp);

        impulsoDsp[3]=impulsoDsp[2];
        impulsoDsp[2]=impulsoDsp[1];
        impulsoDsp[1]=impulsoDsp[0];
        impulsoDsp[0]=0;

        impulsoDsp[6]=impulsoDsp[5];
        impulsoDsp[5]=impulsoDsp[4];
        impulsoDsp[4]=YDsp[i]*3;
    }
    stop=clock();
    count=(stop-start)-19.0;  
	printf("# TCY: %.1f\r\n",count);
    for(i=0;i<=((N_Muestras*CICLOS)-1);i++){
        printf("%.4f\r\n",(Fract2Float(YDsp[i]))*3*7.5);
    }
}