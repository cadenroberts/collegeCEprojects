function y = Sinc(x)
   y = sin(x) ./ x; % Define the sinc function using the given definition
   y(x == 0) = 1;   % Replace with 1 for x = 0
end
x = linspace(-2*pi, 2*pi, 1000); % Define the range of x values
mysinc = Sinc(x); % Calculate the values of my Sinc(x)
matlabsinc = sinc(x/pi); % MATLAB's function is defined sin(pi*x) / (pi*x)
plot(x, mysinc, 'b', 'LineWidth', 2); % mysinc blue width 2
hold on; % allow multiple plots on same graph
plot(x, matlabsinc, 'r--', 'LineWidth', 3); % matlabsinc dashed red width 3
xlabel('x'); % label x axis x
ylabel('sinc(x)'); % label y axis sinc(x)
title('Comparison of my Sinc and MATLAB sinc'); % create title
legend('MySinc', 'MATLAB sinc'); % create legend
grid on; % add grid
hold off; % don’t allow more plots on same graph