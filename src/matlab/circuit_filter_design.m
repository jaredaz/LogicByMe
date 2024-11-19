% EE348 Lab8
% Filter Design Project

f_s = 500*10^3; % Sampling rate
t = (0:250*10^3-1)/f_s;

freq = 60*10^3;     

norm_freq = freq/(f_s/2);
co_freq = 10*log10(0.5);    % cutoff freq
H_max = 1;                  % Filter magnitude   |Hmax|
magH_w = H_max/sqrt(2);     % 

p_h = magH_w ^ 2;     % Filter Power


N=5;        % number of zeros
M=5;        % number of zeros
CM = (N+M)/2;       % Complexity Measure

z=[0;125;180;220;250]
p=[40;50;60;70;80]

%Total_points = Total_points + (5*(6-CM));   %
zplane(z,p)
title('Pole-Zero Diagram')

