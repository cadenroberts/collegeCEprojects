function y = M(w) % define M(w) as H(w)*G(w)
    a = 5 * abs(w) .* (abs(w)<=20);
    b = 2 * (abs(w) >= 5 & abs(w) <= 10);
    y = a .* b;
end
w = -31.4:0.01:31.4; % domain w
subplot(2, 2, 1);
plot(w, abs(M(w))); % abs to get magnitude
title("Magnitude M(\omega)");
subplot(2, 2, 3);
plot(w, angle(M(w))); % angle to get phase
title("Phase M(\omega)");
m = ifftshift(ifft(M(t))*(t(2)-(1))); % define m(t)
subplot(2, 2, 2);
plot(t, real(m)); % plot real components of m(t)
title("Re(m(t))")
subplot(2, 2, 4);
plot(t, imag(m)); % plot imaginary components of m(t)
title("Im(m(t))");