t = 0:(1/14):1; % initialize 15 even spaced t vals 0<=t<=1
x = 4*cos(2*pi*t + 0.2) + 3*sin(pi^2*t); % Compute vals of x(t)
xmax = -1000; % initialize vars for min max and ave
xmin = 1000;
xave = 0;
for i=1:length(x) % Calculate results
   if xmax < x(i) % if xmax is less than x(i), update
       xmax = x(i);
   end
   if xmin > x(i) % if xmin is greater than x(i), update
       xmin = x(i);
   end
   xave = xave + x(i); % add each x(i) to xave
end
xave = xave / 15; % divide sum of x(i)'s by 15 to get average
fprintf('Max element value: %.4f\nMin element value: %.4f\nAve of element values: %.4f\nIndices with mag > 4:', xmax, xmin, xave); % display requested results formatted
indices=find(abs(x)>4); % find indices of elements with mag > 4
for i=1:1:(length(indices)-1) % print each index
    fprintf(' %.0f,', indices(i))
end
fprintf(' %.0f\n', indices(length(indices))); % last index and newline