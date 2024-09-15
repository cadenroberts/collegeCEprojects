clc
close all
clear all
t = -0.1:0.001:0.1;
w = sin(400*pi*t);
h = 400*exp(-200*t).*cos(400*pi*t).*(t>=0);
x = (cos(100*pi*t)+sin(400*pi*t)-cos(800*pi*t)).*(t>=0);
figure(1)
subplot(311)
plot(t, w)
ylabel('w(t)')
grid on
subplot(312)
plot(t, x)
ylabel('Input - x(t)')
grid on
subplot(313)
Conv = 0.001*conv(x, h);
t = -0.1:0.0005:0.1;
disp(length(t))
disp(length(Conv))
plot(t, Conv)
ylabel('Output - Convolution')