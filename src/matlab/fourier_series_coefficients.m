% EE348-002 Lab 06
% March 22, 2017
% Fourier Series Coefficients
% 4 : Fourier Series Approximation
syms k
t=sym('t','real');

N=3;

T=0.1;
w_0 = 2*pi/T;
j = 1i;
dt = 0.000001;

%x=sin(k*w_0*t)/(k*w_0*T);
x=sin(w_0*t);
cycles = 2; 
time = [0:dt:(15*T-dt)];
sqr_wave = -1*square(pi*20*time);  % -1 * square wave

% Fourier Coefficients
a_0 = (1/T)* int((x),t,(-T/2),(T/2))
a_k = (2/T)*  int(x*cos(k*w_0*t),t,-T/2,T/2)
b_k = (2/T)*  int(x*sin(k*w_0*t),t,-T/2,T/2)
FS = (a_0 + symsum(((a_k*cos(k*w_0*t))+(b_k*sin(k*w_0*t))), k,-N,-1) + symsum(((a_k*cos(k*w_0*t))+(b_k*sin(k*w_0*t))), k,1,N))

figure(1)

ezplot(erf( -1*FS), [0,2*T])
%set(Fourier,'Color','red', 'LineStyle', 's', 'LineWidth', 2);
axis([0 2*T -2 2])
xlabel('t')
ylabel('x^(t)')
%title('4.1 Fourier Series Approximation')
title('4.1 Fourier Series Approximation')
hold on
plot(time,sqr_wave,'b.-')
legend('Periodic Function','Square Wave')
grid on
