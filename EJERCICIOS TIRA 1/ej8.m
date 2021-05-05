Ts=0.005;
muestreo=50
num=[1];
den=[1/10 1];
sys=tf(num,den)
figure(1)
step(sys,4)
figure(2)
impulse(sys,4)
format long
[numd,dend] = c2dm(num,den,Ts,'tustin')
hold off
figure(3)
dimpulse ((1/Ts)*numd,dend,muestreo+1) 
figure(4)
dstep ((1/Ts)*numd,dend,muestreo+1)