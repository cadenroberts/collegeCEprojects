function y = g(t) % define g(t) according to provided definiton
    y = 3*pi*sin(8*pi.*t+1.3).*cos(4*pi.*t-0.8).*exp(sin(12*pi.*t));
end
E = integral(@(t) abs(g(t)).^2, 0.25, 0.75); % calc energy as int of |g(t)|^2
fprintf('Energy of the function g(t) is %.4f joules.\nPower of the function g(t) is %.4f watts.\n', E, E/(0.75-0.25)); % calc power and display results