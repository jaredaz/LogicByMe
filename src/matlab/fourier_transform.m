% EE348 - L - 01
% April 12, 2017
% Lab 7: Fourier Transform

close all
clear all

% Given a function x(t), find its Fourier Transform (FT)
% Generates the function x(t) = e^((-alpha)*t)*u(t)

% Fourier Transform defined as:
% X(w) = integral((x(t)*exp((-1i)*w*t)), 0, t) 

% Lab Exercise: Part 1
% Case 1: alpha = 0.1
dt=0.1;
t = 0 : dt: 10-dt;

%F = @(w) R(w) .* exp(-1*a*t);
x_1 = exp((-0.1).*(t));
x_2 = exp((-(1).*(t)));
x_3 = exp((-(10).*(t)));

% Fourier Transform (FT)
% FT  is 1/(a+jw)
x_1_FT = fft(x_1)
x_2_FT = fft(x_2)
x_3_FT = fft(x_3)
% Magnitude of FT
x_mag1_FT = real(x_1_FT);
x_mag2_FT = real(x_2_FT);
x_mag3_FT = real(x_3_FT);
% Phase of FT
x_phase1_FT = angle(x_1_FT);
x_phase2_FT = angle(x_2_FT);
x_phase3_FT = angle(x_3_FT);

%Plotting
subplot(2,2,1)
plot (t, x_1, 'r'); hold on;
plot (t, x_2, 'b'); hold on;
plot (t, x_3, 'g');
title('2.1  x(t)=e^{\alphat}u(t)')
xlabel('time (t)')
ylabel('x(t)')
legend('show')
legend('a = 0.1', 'a = 1', 'a = 10')
hold off;

% Fourier Transform
subplot(2,2,2)
plot (t, x_1_FT, 'r'); hold on;
plot (t, x_2_FT, 'b-','LineWidth',2); hold on;
plot (t, x_3_FT, 'g');
title('2.2 Fourier Transform x(t)=e^{\alphat}u(t)')
xlabel('time (t)')
ylabel('X(\omega)')
legend('show')
legend('\alpha = 0.1', '\alpha = 1', '\alpha = 10')
hold off;

% Amplitude Fourier Transform
subplot(2,2,3)
plot (t, x_mag1_FT, 'r'); hold on;
plot (t, x_mag2_FT, 'b'); hold on;
plot (t, x_mag3_FT, 'g');
title('Amplitude of Fourier Transform')
xlabel('time (t)')
ylabel('|X(\omega)|')
legend('show')
legend('a = 0.1', 'a = 1', 'a = 10')
hold off;

% Phase Fourier Transform
subplot(2,2,4)
plot (t, x_phase1_FT, 'r'); hold on;
plot (t, x_phase2_FT, 'b'); hold on;
plot (t, x_phase3_FT, 'g');
title('Phase Plot of Fourier Transform')
xlabel('time (t)')
ylabel('\angleX(\omega)')
legend('show')
legend('a = 0.1', 'a = 1', 'a = 10')
hold off;

%end of lab6.m