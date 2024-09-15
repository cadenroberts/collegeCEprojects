clf; % clear current figure
syms y(t) % Define the symbolic function
R = 2; % intialize variables
C = 1;
L = 0.5;
ode = R*C*diff(y, t, 2) + diff(y, t) + (R/L)*y; % define differential
Dy = diff(y, t); % define Dy
t = -10:0.001:30; % define time domain
sol = eval(dsolve(ode, [y(0) == 0, Dy(0) == 10])).*(t>=0); % eval values
plot(t, sol); % plot results
grid on; % grid on
