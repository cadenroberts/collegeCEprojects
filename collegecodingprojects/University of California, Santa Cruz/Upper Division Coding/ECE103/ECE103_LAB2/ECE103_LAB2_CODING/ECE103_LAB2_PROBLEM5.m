function x = f(s)
    x = zeros(1, 200001);
    for t = 1:1:200001
        for f = 159:s:161
            x(t) = x(t) + 10*cos(2*pi*f*((t-1)/1000));
        end
    end
end
t = 0:0.001:200; % initialize t range
clf; % clear any previous figure
plot(t, f(0.04), 'b', 'LineWidth', 2); % plot 0.04 ∆Hz blue width 2
hold on; % allow multiple plots on the same graph
plot(t, f(0.02), 'g', 'LineWidth', 2); % plot 0.02 ∆Hz green width 2
xlabel('Time (t)'); % label x axis
ylabel('Signal y(t)'); % label y axis
title('Plot of 0.02 ∆Hz and 0.04 ∆Hz for y(t)'); % create title
legend('0.04 ∆Hz', '0.02 ∆Hz'); % create legend
grid on; % grid on
hold off; % stop allowing multiple plots

