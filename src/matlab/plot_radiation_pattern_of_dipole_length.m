 %{ 
Computer Programming Assignment
EE499-01 Antennas and Radiation
March 8, 2019

Use MATLAB to calculate the values and plot the radiation pattern of the
following dipole lengths. Submit your plots and code to BBlearn. 

For arbitary length dipoles: 0.25?, 0.75?, 1.25?, 1.33?

For each dipole length, find the following:
    a) The power density radiated by dipole antenna.
    b) S_max
    c) Direction on main lobe
    d) Half power beamwidth
    e) Beam solid angle
    f) Directivity 
    g) Radiation resistance
    h) Plot the radiation pattern
%}

%initalizing variables
lamda = 1; % wavelength is constant
l = [0.25, 0.75, 1.25, 1.33]*lamda; %length
n_0 = 120*pi;  % n_0 := intrinsic impedamce of free space
I_0 = 1; % current is constant 
k = (2 * pi) / lamda; 
R=1; %also a constant
t = 0:0.01:2*pi;% range theta

% annonomous functions
S_0 = @ (I_0, R, dlen, lamda)((15 * pi * I_0^2)/(R^2)) * (dlen/ lamda).^2; %So
F = @(theta) sin(theta).^2; %F := normalized radiation intensity
S = @(theta, I_0, R, dlen, lamda) S_0(I_0, R, dlen, lamda)*F(theta); %power density
rad2deg = @(rad) rad * (180 / pi); % radians to degrees

% a) The power density radiated by dipole antenna.
S_0_val = S_0(I_0, R, l, lamda) ;
F_val = F(theta);
S_av = S_0_val .* F_val; % power density

% b) S_max
S_max = S_0_val; % S_max = S_0

% c) Direction on main lobe
syms rho theta
pattern_solid_angle = int(int(F(theta), theta, 0, pi), rho, 0, 2*pi);

% d) Half power beamwidth
half_power_beamwidth = asin(sqrt(0.5));
half_power_beamwidth_deg = rad2deg(half_power_beamwidth_thetaRad);

% e) Beam solid angle
beam_solid_angle = half_power_beamwidth * 2;
% f) Directivity 
P_rad = R^2 * S_max * int(int(F(theta)*sin(theta),theta,0,pi),rho,0,2*pi);
D = (4 * pi * R^2 .* S_max) ./ P_rad;

% g) Radiation resistance
R_rad = P_rad / (0.5 * I_0^2);
R_rad2 = (80 * pi^2 .*(l./lamda).^2); %different equation, same ans as R_rad

% h) Plot the radiation pattern
% get radiation patterns from far field equation
E1 = abs((cos(B*l(1)/2*cos(t))-cos(B*l(1)/2))./sin(t)).^2; % 0.25*lambda 
E2 = abs((cos(B*l(2)/2*cos(t))-cos(B*l(2)/2))./sin(t)).^2; % 0.75*lambda 
E3 = abs((cos(B*l(3)/2*cos(t))-cos(B*l(3)/2))./sin(t)).^2; % 1.25*lambda 
E4 = abs((cos(B*l(4)/2*cos(t))-cos(B*l(4)/2))./sin(t)).^2; % 1.33*lambda

% plotting each radiation pattern separately 
subplot(2,2,1)
polar(t,E1)
title('Radiation Pattern l = 0.25\lambda')
subplot(2,2,2)
polar(t, E2)
title('Radiation Pattern l = 0.75\lambda')
subplot(2,2,3)
polar(t, E3)
title('Radiation Pattern l = 1.25\lambda')
subplot(2,2,4)
polar(t, E4)
title('Radiation Pattern l = 1.33\lambda')

% plotting all radiation patterns on same plot
figure(2)
polar(t,E3)
hold on
polar(t,E2)
polar(t,E1)
polar(t,E4)
title('Radiation Pattern of [0.25\lambda, 0.75\lambda, 1.25\lambda, 1.33\lambda]')
legend('1.25\lambda','0.75\lambda','0.25\lambda','1.33\lambda')
hold off

%print out the results:
fprintf('\nEE499-01 Project 1: Computer Programming Assignment\n')
fprintf('a) The power density radiated by dipole antenna: [ %s, %s, %s, %s]',S_av(1), S_av(2), S_av(3), S_av(4))
fprintf('\nb) S_max: [ %.2f, %.2f, %.2f, %.2f]', S_max(1), S_max(2), S_max(3), S_max(4))
fprintf('\nc) Direction on main lobe: %s' , pattern_solid_angle )
fprintf('\nd) Half power beamwidth: %.4f radians == %.2f degrees', half_power_beamwidth, half_power_beamwidth_deg )
fprintf('\ne) Beam solid angle: %.2f rad = %.2f deg', beam_solid_angle, rad2deg(beam_solid_angle))
fprintf('\nf) Directivity: [ %.2f, %.2f, %.2f, %.2f]', D(1), D(2), D(3), D(4))
fprintf('\ng) Radiation resistance: [ %s, %s, %s, %s]', R_rad(1),R_rad(2),R_rad(3),R_rad(4))
fprintf('\nh) Plot the radiation pattern: see figure 1 & 2\n\n')

