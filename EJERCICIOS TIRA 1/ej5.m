f=0.5; 
T=1/f; 
muestreo=50 
Ts=T/muestreo; 
w=2*pi*f; 
num=[2];
den=[1 0 0 0];
sys=tf(num,den)
impulse(sys,T) 
format long
[numd,dend] = c2dm(num,den,Ts,'tustin')
hold off
dimpulse ((1/Ts)*numd,dend,muestreo+1)