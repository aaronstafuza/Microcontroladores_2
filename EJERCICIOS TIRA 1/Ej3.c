
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
clock_t start, stop;
double count=0;
unsigned int i=0,j=0;
double c[3][3];
fractional srcM1[3][3]={{Q15(0.02/2),Q15(0.12/2),Q15(-0.12/2)},{Q15(0.13/2),Q15(-0.15/2),Q15(1.2/2)},{Q15(0.28/2),Q15(-0.28/2),Q15(0.1/2)}};
fractional srcM2[3][3]={{Q15(1.3/2),Q15(0.13/2),Q15(-0.15/2)},{Q15(0.21/2),Q15(0.18/2),Q15(-0.01/2)},{Q15(-0.03/2),Q15(0.17/2),Q15(0.13/2)}};
fractional resM[3][3];
double M1[3][3] = { {0.02,0.12,-0.12},{0.13,-0.15,1.2},{0.28,-0.28,0.1} };
double M2[3][3] = { {1.3,0.13,-0.15},{0.21,0.18,-0.01},{-0.03,0.17,0.13} };
double rM[3][3];
void AB(void);
void BA(void);
void inverA(void);
void inverB(void);

int main(void) {
    AB();
    BA();
    inverA();
    inverB();
    return (EXIT_SUCCESS);
}
void AB(void){
    printf("Producto A*B\r\n");
    start=clock();
    MatrixMultiply(3,3,3,resM,srcM1,srcM2);
    stop=clock();
    for(i=0;i<=2;i++){
        for(j=0;j<=2;j++){
            c[i][j]=Fract2Float(resM[i][j])*4;
            printf("%.3f ",c[i][j]);
        }
        printf("\r\n");
    }
    count=(stop-start)-19.0; //19 tcy starr clock
    printf("# TCY con DSP: %.1f\r\n",count);
}

void BA(void){
    printf("Producto B*A\r\n");
    start=clock();
    MatrixMultiply(3,3,3,resM,srcM2,srcM1);
    stop=clock();
    for(i=0;i<=2;i++){
        for(j=0;j<=2;j++){
            c[i][j]=Fract2Float(resM[i][j])*4;
            printf("%.3f ",c[i][j]);
        }
        printf("\r\n");
    }
    count=(stop-start)-19.0; 
    printf("# TCY con DSP: %.1f\r\n",count);
}
void inverA(void){
    printf("\r\nInversa A con DSP\r\n");
    start=clock();
    int swappedRows[3],swappedCols[3],i=0;  
    float pivotFlag[3];    
    MatrixInvert(3,&(rM[0][0]),&(M1[0][0]),pivotFlag,swappedRows,swappedCols);
    stop=clock();
    for(i=0;i<3;i++){
        printf("[%3f  %3f  %3f]\n",rM[i][0],rM[i][1],rM[i][2]);
    }
    count=(stop-start)-19.0; //19 tcy starr clock
    printf("# TCY de inversa con DSP: %.1f\r\n",count);
}
void inverB(void){
    printf("\r\nInversa B con DSP\r\n");
    start=clock();
    int swappedRows[3],swappedCols[3],i=0;  
    float pivotFlag[3];     
    MatrixInvert(3,&(rM[0][0]),&(M2[0][0]),pivotFlag,swappedRows,swappedCols);
    stop=clock();
    for(i=0;i<3;i++){
        printf("[%3f  %3f  %3f]\n",rM[i][0],rM[i][1],rM[i][2]);
    }
    count=(stop-start)-19.0; //19 tcy starr clock
    printf("# TCY de inversa con DSP: %.1f\r\n",count);
}