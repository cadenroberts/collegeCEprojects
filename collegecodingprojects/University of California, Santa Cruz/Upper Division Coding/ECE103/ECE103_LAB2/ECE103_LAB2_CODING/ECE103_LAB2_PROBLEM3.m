function y = x(t)
    y = t.*exp(-0.15*t);
end
t = linspace(-20, 20, 40000);
clf; % clear any remaining figure
subplot(2, 2, 1);
plot(t, x(t), 'b'); % plot blue
title('x(t)'); % title subplot
xlabel('t'); % label x axis t
ylabel('x(t)'); % label y axis x(t)
grid on; % grid on
subplot(2, 2, 2)
plot(t, (x(t)+x(-t))/2, 'r'); % plot even red
title('x_e(t)'); % title subplot
xlabel('t'); % label x axis t
ylabel('x_e(t)'); % label y axis x(t-3)
grid on; % grid on
subplot(2, 2, 3)
plot(t, (x(t)-x(-t))/2, 'y'); % plot odd yellow
title('x_o(t)'); % title subplot
xlabel('t'); % label x axis t
ylabel('x_o(t)'); % label y axis x(-t)
grid on; % grid on
subplot(2, 2, 4)
plot(t, ((x(t)+x(-t))/2 + (x(t)-x(-t))/2), 'g'); % plot green
title('y(t)'); % title subplot
xlabel('t'); % label x axis t
ylabel('y(t)'); % label y axis -3x(-t+4)
grid on; % grid on