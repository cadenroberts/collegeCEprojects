function y = x(t) % define function according to provided definition
    y = 10-2*abs(t);
    y(t>=5) = 10;
    y(t>=10) = 0;
    y(t<-5) = 0;
end
t = linspace(-10, 15, 2500);
clf; % clear any remaining figure
subplot(2, 2, 1)
plot(t, x(t+2), 'b'); % plot blue
title('x(t+2)'); % title subplot
xlabel('t'); % label x axis t
ylabel('x(t)'); % label y axis x(t)
grid on; % grid on
subplot(2, 2, 2)
plot(t, x(t-3), 'r'); % plot red
title('x(t-3)'); % title subplot
xlabel('t'); % label x axis t
ylabel('x(t-3)'); % label y axis x(t-3)
grid on; % grid on
subplot(2, 2, 3)
plot(t, x(-t), 'y'); % plot yellow
title('x(-t)'); % title subplot
xlabel('t'); % label x axis t
ylabel('x(-t)'); % label y axis x(-t)
grid on; % grid on
subplot(2, 2, 4)
plot(t, -3*x(-t+4), 'g'); % plot green
title('-3x(-t+4)'); % title subplot
xlabel('t'); % label x axis t
ylabel('-3x(-t+4)'); % label y axis -3x(-t+4)
grid on; % grid on
