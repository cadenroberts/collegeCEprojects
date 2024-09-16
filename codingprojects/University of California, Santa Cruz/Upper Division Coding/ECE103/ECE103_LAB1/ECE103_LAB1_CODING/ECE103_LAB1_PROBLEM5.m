f1 = 0.2; % Define the vals f1, f2, t
f2 = 0.425;
t = 0:0.1:10;
s1 = sin(2*pi*f1*t); % Calculate s1, s2, and s3
s2 = sin(2*pi*f2*t + 0.4);
s3 = s1 + s2;
plot(t, s1, 'b'); % plot s1 in blue
hold on; % Allow multiple plots on the same graph
plot(t, s2, 'r'); % plot s2 in red
plot(t, s3, 'g'); % plot s3 in green
xlabel('Time (t)'); % label x axis
ylabel('Amplitude'); % label y axis
title('Plot of s1, s2, and s3 vs. t'); % create title
legend('s1', 's2', 's3'); % create legend
grid on; % add a grid
hold off; % stop allowing multiple plots on the same graph