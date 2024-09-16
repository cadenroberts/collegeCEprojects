wvals = [35, 40, 45]; % Define the values of w
funcvals = zeros(size(wvals)); % Initialize array to store the results
maxtvals = zeros(size(wvals)); % initialize array to store max t vals
for i = 1:length(wvals) % Iterate over each value of w
   w = wvals(i); % select value of w
   t = round((10^(1/3)) * 100) / 100; % Initialize t
   while t^3 >= 0 % loop until t is negative
       if (exp(1.2)*cos(w*t)) < 10 % if func is less than 10
           maxtvals(i)=t; % update max t val
           funcvals(i)=exp(1.2)*cos(w*t); % update func val
           break % exit while loop
       end
       t = t - 0.01; % decrement t if func is > than 10, loop again
   end
end
fprintf('W vals:\n   %.0f   %.0f   %.0f\n\nMax t vals:\n  %.2f  %.2f  %.2f\n\nCorresponding func vals:\n  %.2f  %.2f  %.2f\n', wvals(1), wvals(2), wvals(3), maxtvals(1), maxtvals(2), maxtvals(3), funcvals(1), funcvals(2), funcvals(3)); % print results formatted

