f=9; 
T=1/f;
muestreo=50 
Ts=T/muestreo; 
w=2*pi*f; 


num=[w];
den=[1 0 w^2];
sys=tf(num,den)

format long
[numd,dend] = c2dm(num,den,Ts,'tustin')
hold off

dimpulse ((1/Ts)*numd,dend,muestreo+1)