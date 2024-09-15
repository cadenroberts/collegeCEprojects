tval = 0:0.1:0.4; % Define time vals
fval = [10, 15, 20]; % Define freq vals
results = zeros(length(fval), length(tval)); % 2D array to store the results
for i = 1:length(fval)            % Compute for each freq, put in results
   f = fval(i);                   % load f value
   for j = 1:length(tval)
       t = tval(j);               % load t value
       x = 3*cos(2*pi*f*t + 0.1); % Compute x(t) for given f and t
       results(i, j) = x;         % Store the result in the results array
   end
end
fprintf('Results f vs t:\n  t:'); % use for loops/if’s to print formatted
for i = 1:5
   fprintf('      %.1f ', tval(i));
end
fprintf('\nf:\n');
n = 1;
while n < 4
    fprintf('%.0f   ', fval(n));
    for i = n:3:(n+12)
       if results(i) > 0
           fprintf(' ');
       end
       fprintf(' %.4f  ', results(i));
    end
    n=n+1;
    fprintf('\n');
end
