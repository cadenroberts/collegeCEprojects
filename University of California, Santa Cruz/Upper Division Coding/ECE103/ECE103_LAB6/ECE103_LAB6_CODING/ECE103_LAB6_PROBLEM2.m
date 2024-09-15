clear all;
clc;
close all;
load ecg_signal.mat;
R = 10000; % define constants
C = 133e-9;
M = 0.9;
F = linspace(-250, 250, length(ecg)); % define range
W = 2 * pi * F;
H = ((1 + M).*((2*1j*W*R*C).^2+1))./((2*1j*W*R*C).^2+(4*(1-M).*(1j*W*R*C))+1); % calc freq response
X = fft(ecg); % time domain output signal
Z = fftshift(X).*H; % filtered signal Z(f)
z = ifft(Z);
energyx = sum(abs(ecg).^2); % calc energies
energyz = sum(abs(z).^2);
disp("Energy of x(t) in the time domain");
disp(energyx);
disp("Energy of z(t) in the time domain");
disp(energyz);
frequencyEnergyX = sum(abs(X).^2)/length(X);
disp("Energy of X(w) in the frequency domain")
disp(frequencyEnergyX);
frequencyEnergyZ = sum(abs(Z).^2)/length(Z);
disp("Energy of Z(w) in the frequency domain");
disp(frequencyEnergyZ);
frequencyEX = (abs(X).^2)/length(X); % array of the values computed for the energy
frequencyEZ = (abs(Z).^2)/length(Z);
figure(1); % plot energy spectrums
subplot(2,1,1);
plot(F,frequencyEX);
xlabel("Frequency (Hz)")
ylabel("Energy of X(\omega)");
grid on;
subplot(2,1,2);
plot(F, frequencyEZ);
xlabel("Frequency (Hz)")
ylabel("Energy of Z(\omega)")
grid on;
