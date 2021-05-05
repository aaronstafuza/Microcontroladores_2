
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
//Ir descomentando para usar diferentes cosas, generadorDSP es el generador senoidal
//punto_flotante es el generador senosoidal con punto florante. 
//esta en uso el generador cosenoidal

void ptoflotante(void); 
void generador_DSP(void);
void generador_cos(void);
int main (void){
    ptoflotante();
    generador_DSP();
    generador_cos();
    return 0;
}

void ptoflotante (void){
    printf("\r\nGenerador sen pto flotante\n\r");
    double numd[3]={0,0.077895973637639,0.077895973637639};  
    double dend[3]={1,-1.902113032590307,1};          
    double X[40]={0};
    double X1[40]={0};
    double X2[40]={0};
    double Y[40]={0};
    double Y1[40]={0};
    double Y2[40]={0};
    X[0]=2;                   
    X1[1]=2;   
    X2[2]=2;  
    int i=0;
    for(i=0;i<40;i++){
        X[i]=X[i]*numd[0];
        X1[i]=X1[i]*numd[1];
        X2[i]=X2[i]*numd[2];
    }   
    for(i=0;i<40;i++){
        Y[i]=X[i]+X1[i]+X2[i]-Y1[i]-Y2[i];
        if(i<39){
            Y1[i+1]=Y[i]*dend[1];
            Y2[i+2]=Y[i]*dend[2];
            }
        else if(i>38){
            Y1[i+1]=Y[i]*dend[1];
        }
    }
    for(i=0;i<40;i++){
        //printf("%f\n",2*Fract2Float(Y[i])); //imprimo el vector y para ver como da
        printf("[%d]=%f\n",i,Y[i]);
    }
}

void generador_DSP(void){
    printf("\r\nGenerador sen\n\r");
    fractional y=Q15(0);
    fractional srcV1[5]={Q15(0),Q15(0),Q15(1),Q15(0),Q15(0)};
    fractional srcV2[5]={Q15(0.003377/1.61803),Q15(0.003377/1.61803),Q15(0/1.61803),Q15(-1/1.61803),Q15(1.61803/1.61803)};
    int i=0;
    double a;
    while(i<60){
        y=VectorDotProduct(5,srcV1,srcV2); 
        a=Fract2Float(y)*1.61803*90; 
        printf("%f\n",a);
        srcV1[0]=srcV1[1]; 
        srcV1[1]=srcV1[2]; 
        srcV1[2]=0; 
        srcV1[3]=srcV1[4]; 
        srcV1[4]=y*1.61803; 
        i++;
    }
}
void generador_cos(void){
    printf("\r\nGenerador cos\n\r");
    fractional yc=Q15(1);
    fractional V1[5]={Q15(0),Q15(0),Q15(1),Q15(0),Q15(0)};
    fractional V2[5]={Q15(-0.0010/1.9566),Q15(0/1.9566),Q15(0.001099/1.9566),Q15(-1/1.9566),Q15(1.9566/1.9566)};
    int i=0;
    double a;
    while(i<60){
        yc=VectorDotProduct(5,V1,V2); 
        a=Fract2Float(yc)*1.9566; 
        printf("%f\n",a);
        V1[0]=V1[1]; 
        V1[1]=V1[2]; 
        V1[2]=0; 
        V1[3]=V1[4]; 
        V1[4]=yc*1.9566; 
        i++;
    }
}
