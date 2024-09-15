G = @(w) 2 * (abs(w) >= 5 & abs(w) <= 10); % define G(w)
w = -31.4:0.01:31.4; % w domain
t = -100:0.1:100; % t domain
g = @(t) ifftshift(ifft(G(t))*0.1); % define g(t)
y = @(t) ifftshift(ifft(G(t-5))*0.1); % define y(t)
subplot(3,2,1);
plot(w, G(w)); % plot G(w)
title("G(\omega)");
subplot(3,2,3);
plot(t, real(g(t))); % plot real components of g(t)
title("Re(g(t))");
subplot(3,2,5);
plot(t, imag(g(t))); % plot imaginary components of g(T)
title("Im(g(t))");
subplot(3,2,2);
plot(w, G(w-5)); % plot Y(w)
title("Y(\omega)=G(\omega -5)");
subplot(3, 2, 4);
plot(t, real(y(t))); % plot real components of y(t)
title("Re(y(t))");
subplot(3,2,6);
plot(t, imag(y(t))); % plot imaginary components of y(t)
title("Im(y(t))");
fprintf('g(t) is real? %d\ny(t) is real? %d\n', all(abs(G(w) - conj(G(-w))) < 1e-10), all(abs(G(w-5) - conj(G(-(w-5)))) < 1e-10)); % determine if g(t) and y(t) are real