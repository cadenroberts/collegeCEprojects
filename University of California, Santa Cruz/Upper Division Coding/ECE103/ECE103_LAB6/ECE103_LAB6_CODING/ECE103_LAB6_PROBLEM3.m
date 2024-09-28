clear all;
clc;
close all;
mFrequency = 5e4; % define the constants and the signals
cFrequency = 5e5;
cTime = 1/10/cFrequency; % Sampling period with 10 samples per
time = 0:cTime:6/mFrequency; % Time vector
message = [6, 0, 4, -6, 2];
mArray = zeros(size(time)); % array for the message signal values
intervals = (0:length(message))/mFrequency;% generate the message signal
for i = 1:length(message) % loop each interval assigning corresponding char
    mArray((time >= intervals(i)) & (time < intervals(i+1))) = message(i);
end
s = mArray.*cos(2*pi*cFrequency*time); % define the DSB-SC signal
v = s.*cos(2*pi*cFrequency*time+pi/3); % define the Demodulated signal
N = length(time);
freq = (-(N-1)/2:N/2)*1/cTime/N; % frequency axis for the fast fourier transform function
fftv = zeros(size(v)); % Performing the low-pass filtering
fftw = fftshift(fft(v));
for i = 1:length(freq)
    if(abs(freq(i))<5e5)
        fftv(i) = 2*fftw(i); % passing low pass frequencies
    end
end
v_out = ifft(ifftshift(fftv));
subplot(4, 1, 1); % plot signals and spectrums
plot(time, s);
xlabel('time (s)');
ylabel('s(t)');
title('Time domain signal');
subplot(4, 1, 2);
plot(freq, fftshift(abs(fft(s)))/N);
xlabel('frequency (Hz)');
ylabel('|S(f)|');
title('Magnitude spectrum');
subplot(4, 1, 3);
plot(time, v_out);
xlabel('time (s)');
ylabel('v_o(t)');
title('Time domain signal');
subplot(4, 1, 4);
plot(freq, fftshift(abs(fft(v_out)))/N);
xlabel('frequency (Hz)');
ylabel('|V_o(f)|');
title('Magnitude spectrum');
