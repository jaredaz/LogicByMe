 %{ 
Computer Programming Assignment #2
EE499-01 Antennas and Radiation
April 12, 2019
%}
%{
Use MATLAB to plot the radiation pattern of the following array types. Do
not have the user input information. The script will run and plot all of
the arrays in the order shown. 

For each array, plot the following:
a) 4 source broadside array for d=0.25*L, 0.5*L, L, and 1.5*L
b) 4 source endfire array for d =0.25*L, 0.5*L, L, and 1.5*L
c) N source broadside array with d=0.5*: and N=4,5,6,8,10
d) N source endfire array with d=0.5*: and N=4,5,6,8,10
e) 4 source endfire array with d=0.5*L and for the phase differences of
-pi/5 through -2pi in pi/5 increments

%}

L = 1; % lamda
theta1 = 0:0.01:2*pi;

% anonomous functions 
F_an = @ (N, d, L, theta) ((sin( ((N * pi * d)/(L)).*cos(theta) ) ).^2 )./ (N^2 .* (sin( ((pi * d)/(L)).*cos(theta))).^2); 
y = @ (d,L,theta) ((2*pi*d)/L) .* cos(theta);
F_a = @ (N,d,L,theta) ((sin(((N*pi*d)/L).*cos(theta))).^2)./((sin(((pi*d)/L).*cos(theta))).^2);
theta_broadside = [0:0.01:pi, zeros(1,314)];
theta_endfire = [[0:0.01:pi/2, zeros(1,313)],3*pi/2:0.01:2*pi];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% a) 4 source broadside array for d=0.25*L, 0.5*L, L, and 1.5*L
N_a = 4; % N
d_a = [0.25 0.5 1 1.5] * L; % d
theta_a = pi / 2; % broadside direction 90 deg

y_a1 = y(d_a(1), L, theta_a);
F_a1 = F_a(N_a,d_a(1),L,theta1);
%F_an_A = (sin( ((N * pi .* d)./(2.*d)).*cos(theta) ) ).^2 ./ (N^2 .* (sin((pi .* d)./(2.*d))).^2); 

%plots for problem a
figure('Name','a) 4 source broadside array','NumberTitle','off');
subplot(2,2,1) 
polar(theta1, F_an(N_a,d_a(1),L,theta_broadside))
title('N=4, d=0.25\lambda')
subplot(2,2,2) 
polar(theta1, (F_an(N_a,d_a(2),L,theta_broadside))) 
title('N=4, d=0.5 \lambda')
subplot(2,2,3) 
polar(theta1, F_an(N_a,d_a(3),L,theta_broadside))
title('N=4, d= \lambda')
subplot(2,2,4) 
polar(theta1, F_an(N_a,d_a(4),L,theta_broadside))
title('N=4, d=1.5 \lambda')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% b) 4 source endfire array for d =0.25*L, 0.5*L, L, and 1.5*L
N_b = 4;
d_b = [0.25 0.5 1 1.5] * L; 
theta_b = 0; % end-fire direction
%F_an_B = (sin( ((N * pi .* d)./(2.*d)).*cos(theta) ) ).^2 ./ (N^2 .* (sin((pi .* d)./(2.*d))).^2); 

%plots for problem b
figure('Name','b) 4 source end-fire array','NumberTitle','off');
subplot(2,2,1) 
polar(theta1, F_an(N_b,d_b(1),L,theta_endfire))
title('N=4, d=0.25\lambda')
subplot(2,2,2) 
polar(theta1, F_an(N_b,d_b(2),L,theta_endfire))
title('N=4, d=0.5 \lambda')
subplot(2,2,3) 
polar(theta1, F_an(N_b,d_b(3),L,theta_endfire))
title('N=4, d= \lambda')
subplot(2,2,4) 
polar(theta1, F_an(N_b,d_b(4),L,theta_endfire))
title('N=4, d=1.5 \lambda')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% c) N source broadside array with d=0.5*: and N=4,5,6,8,10
N_c = [4 5 6 8 10];
d_c = 0.5 * L;
theta_c = 90; % broadside direction
%F_an = (sin( ((N .* pi * d)./(2*d)).*cos(theta) ) ).^2 ./ (N.^2 .* (sin((pi * d)/(2*d)))^2); 

%plots for problem c
figure('Name','c) N source broadside array','NumberTitle','off');
subplot(2,3,1) 
polar(theta1, F_an(N_c(1),d_c,L,theta_broadside))
title('N=4, d=0.5\lambda')
subplot(2,3,2) 
polar(theta1, F_an(N_c(2),d_c,L,theta_broadside))
title('N=5, d=0.5 \lambda')
subplot(2,3,3) 
polar(theta1, F_an(N_c(3),d_c,L,theta_broadside))
title('N=6, d=0.5 \lambda')
subplot(2,3,4) 
polar(theta1, F_an(N_c(4),d_c,L,theta_broadside))
title('N=8, d=0.5 \lambda')
subplot(2,3,5) 
polar(theta1, F_an(N_c(5),d_c,L,theta_broadside))
title('N=10, d=0.5 \lambda')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% d) N source endfire array with d=0.5*: and N=4,5,6,8,10
N_d = [4 5 6 8 10];
d_d = 0.5 * L;
theta_d = 0; % end-fire direction
%F_an = (sin( ((N .* pi * d)./(2*d)).*cos(theta) ) ).^2 ./ (N.^2 .* (sin((pi * d)/(2*d)))^2); 

%plots for problem d
figure('Name','d) N source end-fire array','NumberTitle','off');
subplot(2,3,1) 
polar(theta1, F_an(N_d(1),d_d,L,theta_endfire))
title('N=4, d=0.5\lambda')
subplot(2,3,2) 
polar(theta1, F_an(N_d(2),d_d,L,theta_endfire))
title('N=5, d=0.5 \lambda')
subplot(2,3,3) 
polar(theta1, F_an(N_d(3),d_d,L,theta_endfire))
title('N=6, d=0.5 \lambda')
subplot(2,3,4) 
polar(theta1, F_an(N_d(4),d_d,L,theta_endfire))
title('N=8, d=0.5 \lambda')
subplot(2,3,5) 
polar(theta1, F_an(N_d(5),d_d,L,theta_endfire))
title('N=10, d=0.5 \lambda')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% e) 4 source endfire array with d=0.5*L and for the phase differences of
%    -pi/5 through -2pi in pi/5 increments
N_e = 4;
d_e = 0.5 * L;
theta_e = 0; % end-fire direction
phase_shift_e = -pi/5 : pi/5 : 2*pi;
%y = (pi.*(cos(theta)-cos(phase_shift)));
F_an_e = (sin( (N_e/2).* (2*pi*d_e/L).*(cos(theta_e)-cos(phase_shift_e)) )).^2 ./ (N^2 .* (sin(0.5.*(2*pi*d_e/L).*(cos(theta_e)-cos(phase_shift_e)))).^2); 

figure('Name','e) 4 source end-fire array with phase differences','NumberTitle','off');
subplot(1,2,1)
polar(theta1, F_an(N_b,d_e,L,theta1))
title('N=4, d=0.5\lambda')

%end-EE499-Project2%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%