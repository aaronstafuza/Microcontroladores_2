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
double V1f[3] = {0.21, -0.3, 1.2};
double V2f[3] = {0.2, 2.2, 1.3};
fractional V1[3] = {Q15(0.21/4), Q15(-0.3/4), Q15(1.2/4)}; 
fractional V2[3] = {Q15(0.2/4), Q15(2.2/4), Q15(1.3/4)};   
fractional Vsuma[3],Vpv[3],Vpe,Vresta[3],Vpot1,Vpot2;
double Vsumaf[3],Vpvf[3],Vpef,Vrestaf[3],Vpot1f,Vpot2f;
void suma();
void resta();
void prod_escalar_vect();
void multiplicacion();
void potencia();

int main (void){
    
    suma();
    resta();
    prod_escalar_vect();
    multiplicacion();
    potencia();

    return 0;
}

void prod_escalar_vect(){
    double prodEsc_float;
    static double count_dsp=0,count_float=0;
    start=clock();    
    Vpe=VectorDotProduct(3,V1,V2);
    stop=clock();       
    count_dsp=stop-start-19.0;  
    Vpef=Fract2Float(Vpe);    
    Vpef=Vpef*16;
    printf("\nProducto escalar:[%f]\n",Vpef);
    start=clock();
    prodEsc_float=V1f[0]*V2f[0]+V1f[1]*V2f[1]+V1f[2]*V2f[2];
    stop=clock();
    count_float=stop-start-19.0;  
    printf("Demora DSP:: %.1f TCY\nDemora pto flotante: %.1f TCY\n",count_dsp,count_float);
}

void suma(void){
    int i=0;
    double suma_float[3];
    static double count_dsp=0, count_float=0;
    start = clock();    
    VectorAdd(3,Vsuma,V1,V2);
    stop=clock();       
    count_dsp=stop-start-19.0;  
    for(i=0;i<=2;i++){
        Vsumaf[i]=Fract2Float(Vsuma[i]);    
        Vsumaf[i]=Vsumaf[i]*4;
    }
    printf("\nSuma de vectores:[%f, %f, %f]\n",Vsumaf[0],Vsumaf[1],Vsumaf[2]);
    start=clock();
    for(i=0;i<3;i++){
        suma_float[i]=V1f[i]+V2f[i];
    }
    stop=clock();
    count_float=stop-start-19.0;  //  el -19 tarda el clock
    printf("Demora DSP: %.1f TCY\nDemora punto flotante: %.1f TCY\n",count_dsp,count_float);
}

void resta(){
    int i=0;
    double resta_float[3];
    static double count_dsp=0,count_float=0;
    start = clock();   
    VectorAdd(3,Vresta,V1,V2);
    stop=clock();      
    count_dsp=stop-start-19.0;  
    for(i=0;i<=2;i++){
        Vrestaf[i]=Fract2Float(Vresta[i]);    
        Vrestaf[i]=Vrestaf[i]*4;
    }
    printf("\nResta de vectores:[%f, %f, %f]\n",Vsumaf[0],Vsumaf[1],Vsumaf[2]);
    start=clock();
    for(i=0;i<3;i++){
        resta_float[i]=V1f[i]-V2f[i];
    }
    stop=clock();
    count_float=stop-start-19.0;  //  el -19 tarda el clock
    printf("Demora DSP: %.1f TCY\nDemora punto flotante: %.1f TCY\n",count_dsp,count_float);
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
    printf("\nProducto vectorial:[%f, %f, %f]\n",Vpvf[0],Vpvf[1],Vpvf[2]); 
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
    printf("Demora DSP: %.1f TCY\nDemora punto flotante: %.1f TCY\n",count_dsp,count_float);    
}

void potencia(){
    double pot1_float=0,pot2_float=0;
    static double count_dsp1=0,count_dsp2=0 ,count_float1=0,count_float2=0;
    start = clock();   
    Vpot1=VectorPower(3,V1);
    stop=clock();       
    count_dsp1=stop-start-19.0;  
    Vpot1f=Fract2Float(Vpot1);    
    Vpot1f=Vpot1f*16;
    start = clock();    
    Vpot2=VectorPower(3,V2);
    stop=clock();      
    count_dsp2=stop-start-19.0;  
    Vpot2f=Fract2Float(Vpot2);    
    Vpot2f=Vpot2f*16;
    printf("\nPotencia V1 : %f, y la de V2 es: %f\n",Vpot1f,Vpot2f); 
    start=clock();
    pot1_float=V1f[0]*V1f[0]+V1f[1]*V1f[1]+V1f[2]*V1f[2];
    stop=clock();
    count_float1=stop-start-19.0;  
    start=clock();
    pot2_float=V2f[0]*V2f[0]+V2f[1]*V2f[1]+V2f[2]*V2f[2];
    stop=clock();
    count_float2=stop-start-19.0;  //  el -19 tarda el clock
    printf("Demora DSP: %.1f TCY y %.1f TCY\n",count_dsp1,count_dsp2);
    printf("Demora punto flotante: %.1f TCY y %.1f TCY\n",count_float1,count_float2);
}
