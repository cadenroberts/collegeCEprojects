function y = g(t) % define g(t) according to provided definition
    y = 3*pi*sin(8*pi.*t+1.3).*cos(4*pi.*t-0.8).*exp(sin(12*pi.*t));
end
t = -1:0.1:1; % initialize 0.1 step size t
clf; % clear any remaining figure
subplot(3, 1, 1); % subplot
plot(t, g(t), 'b'); % plot blue
title('t step = 0.1'); % title subplot
xlabel('t'); % label x axis t
ylabel('g(t)'); % label y axis g(t)
grid on; % grid on
t = -1:0.01:1; % initialize 0.01 step size t
subplot(3, 1, 2); % subplot
plot(t, g(t), 'r'); % plot red
title('t step = 0.01'); % title subplot
xlabel('t'); % label x axis t
ylabel('g(t)'); % label y axis g(t)
grid on; % grid on
t = -1:0.001:1; % initialize 0.001 step size t
subplot(3, 1, 3); % subplot
plot(t, g(t), 'g'); % plot green
title('t step = 0.001'); % title subplot
xlabel('t'); % label x axis t
ylabel('g(t)'); % label y axis g(t)
grid on; % grid on
fprintf('Period of function is %.2f.\n', lcm(lcm((2*pi)/(8*pi)*12, (2*pi)/(4*pi)*12), (2*pi)/(12*pi)*12)/12) % period = 2pi/(3 freq elements) (if all rational) take lcm (scaling because lcm takes int inputs)


