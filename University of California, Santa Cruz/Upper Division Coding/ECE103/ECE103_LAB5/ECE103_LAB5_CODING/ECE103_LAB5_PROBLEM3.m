function y = M(w) % define M(w)
    a = 5 * abs(w) .* (abs(w)<=20);
    b = 2 * (abs(w) >= 5 & abs(w) <= 10);
    y = a .* b;
end
function y = m(t) % define m(t)
    y = ifftshift(ifft(M(t))/(t(1)-t(2)));
end
w = -31.4:0.01:31.4; % domain w
t = -100:0.1:100; % domain t
E = sum(abs(m(t)).^2)*(t(2)-t(1)); % calculate energy m(t)
parsevals = sum(abs(M(w)).^2) * (w(2)-w(1))/ (2*pi); % calculate energy M(w)
fprintf("Energy of m(t) is %.2f joules.\n", E);
fprintf("Energy of M(w) is %.2f joules.\n", parsevals);