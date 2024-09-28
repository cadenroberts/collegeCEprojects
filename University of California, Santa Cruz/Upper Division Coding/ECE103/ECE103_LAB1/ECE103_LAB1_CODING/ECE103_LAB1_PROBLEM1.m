x = 1:100; % creates a vector x with the numbers 1 - 100
y = x(mod(x,2)==0); % creates a t/f array where true corresponds to even numbers and false corresponds to odd numbers. x is then selected from according to the true values, putting only even numbers into vector y.
fprintf('Values in y:\n');
for i=1:1:50
    fprintf('%0.f ', y(i)); % print out vector y
    if i == 26
        fprintf('\n '); % newline for formatting
    end
end
fprintf('\n');