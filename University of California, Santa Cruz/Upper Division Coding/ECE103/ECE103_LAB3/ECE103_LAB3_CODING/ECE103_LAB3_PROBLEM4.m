M=100;
k=1;
b1=20;
b2=0.2;
clf; % clear current figure
syms y(t) % Define the symbolic function
ode1 = M*diff(y, t, 2) + b1*diff(y, t) + k*y == dirac(t);
ode2 = M*diff(y, t, 2) + b2*diff(y, t) + k*y == dirac(t);
Dy = diff(y, t);
t = -10:0.01:300;
disp(dsolve(ode1, [y(0) == 0, Dy(0) == 0]));
disp(dsolve(ode2, [y(0) == 0, Dy(0) == 0]));
sol1 = eval(dsolve(ode1, [y(0) == 0, Dy(0) == 0])).*(t>=0);
sol2 = eval(dsolve(ode2, [y(0) == 0, Dy(0) == 0])).*(t>=0);
subplot(2, 1, 1)
disp(dsolve(ode1, [y(0) == 0, Dy(0) == 0]));
plot(t, sol1);
xlabel('t')
ylabel('impulse response');
title('b=20kgs^{-1}')
grid on; % grid on
subplot(2, 1, 2)
disp(dsolve(ode2, [y(0) == 0, Dy(0) == 0]));
plot(t, sol2);
xlabel('t')
ylabel('impulse response');
title('b=0.2kgs^{-1}')
grid on; % grid on
