clear all;
close all;
clc;
clf;
R = 10000; % define variables and ranges
C = 133e-9;
F = linspace(0, 200, 1000);
W = 2*pi*F;
M = 0.8;
H1 = ((1+M).*((2*1j*W*R*C).^2+1))./((2*1j*W*R*C).^2+(4*(1-M).*(1j*W*R*C))+1); % h for m = 0.8
M = 0.9;
H2 = ((1+M).*((2*1j*W*R*C).^2+1))./((2*1j*W*R*C).^2+(4*(1-M).*(1j*W*R*C))+1);% h for m = 0.9
subplot(2, 1, 1)
plot(F, abs(H1), 'DisplayName', 'm=0.8') % plot magnitude for m = 0.8
hold on;
plot(F, abs(H2), 'DisplayName', 'm=0.9') % plot magnitude for m = 0.9
legend show;
ylabel('Magnitude')
xlabel('Frequency')
subplot(2, 1, 2)
plot(F, angle(H1), 'DisplayName', 'm=0.8') % plot phase for m = 0.8
hold on;
plot(F, angle(H2), 'DisplayName', 'm=0.9') % plot phase for m = 0.9
legend show;
ylabel('Phase')
xlabel('Frequency')
load ecg_signal.mat; % load for part B
F = linspace(-250, 250, length(ecg)); % define new variables
W=2*pi*F;
H = ((1+M).*((2*1j*W*R*C).^2+1))./((2*1j*W*R*C).^2+(4*(1-M).*(1j*W*R*C))+1);
figure;
subplot(4, 1, 1)
plot(ecg) % plot ecg signal
ylabel('ECG Signal')
xlabel('Time')
subplot(4, 1, 2)
plot(F, abs(fftshift(fft(ecg)))); % plot magnitude of X(f)
ylabel('|X(f)|')
xlabel('Frequency')
subplot(4, 1, 3)
plot(F, abs(fftshift(fftshift(fft(ecg)).*H))) % plot magnitude of Z(f)
ylabel('|Z(f)|')
xlabel('Time')
subplot(4, 1, 4)
plot(t, real(ifft(fftshift(fft(ecg)).*H))) % plot z(t)
ylabel('z(t)')
xlabel('Frequency')