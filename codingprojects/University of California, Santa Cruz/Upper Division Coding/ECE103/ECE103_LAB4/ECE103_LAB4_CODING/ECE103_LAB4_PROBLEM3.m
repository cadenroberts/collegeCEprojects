clf; % clear figure
function [Ck]=fourier_series_exp(x,t,Nk,p)
    dT=t(2)-t(1); % calc dT
    T= dT*length(t); % calc T
    w0=2*pi/T;   % calc w0
    if nargin <2 % handle input / default values
        error('Not enough input argument!')
    elseif nargin == 2
        Nk=21;   
        p=0;      
    elseif nargin ==3
        p=0;      
    end
    k=-floor(Nk/2):floor(Nk/2); % calc k
    Ck = zeros(1, length(k)); % Preallocate Ck
    for i = 1:length(k)
        Ck(i) = trapz(t, x .* exp(-1j * k(i) * w0 * t)) * (1/T); % calc fourier coefficients
    end 
    for i = 1:length(k)
            fprintf("%d: %.6f+%fj", i, real(Ck(i)), imag(Ck(i))); % print formatted
            if mod(i, 5) == 0
                fprintf("\n");
            elseif i == length(k)
                fprintf("\n");
            else
                fprintf(", ");
            end
    end
    if p==1
        subplot(2,1,1);
        stem(k,abs(Ck));         % plot abs(Ck) vs k and angle(Ck) vs k
        xlabel('\omega');
        title('Magnitude of Fourier Series Coefficients');
        subplot(2,1,2);
        w0k = w0*k; % harmonic angular frequencies
        stem(w0k,angle(Ck)*180/pi); % plot phase
        xlabel('\omega');
        ylabel('degrees');
        title('Phase spectrum k=-25:25');
        figure; % plot 3 cycles of the signal 'x' and the reconstructed signal
        t_extended = linspace(min(t),2*max(t),2*length(t)); % 2 periods
        x_reconstructed = zeros(1,length(t_extended)); % initialize reconstruction
        for i = 1:length(k)
            x_reconstructed = x_reconstructed + Ck(i)*exp(1j*k(i)*w0*t_extended); % reconstruct signal
        end
        plot(t_extended,real(x_reconstructed)); % plot reconstructed
        hold on;
        plot([t, t+5], [real(x), real(x+5)],'r', 'LineWidth', 2); % plot real signal
        xlabel('t');
        ylabel('real part of z');
        title('approximation with 51 terms');
        legend('Reconstructed Signal', 'Original Signal');
        grid on;
        figure;
        plot(t_extended,imag(x_reconstructed)); % plot reconstructed
        hold on;
        plot([t, t+5], [imag(x), imag(x+5)],'r', 'LineWidth', 2); % plot imag signal
        xlabel('t');
        ylabel('imaginary part of z');
        title('approximation with 51 terms');
        legend('Reconstructed Signal', 'Original Signal');
        grid on;
    end
end
t = linspace(0, 5, 1000); % define t
z = t.^3-1j*2*pi*t.^2; % define z
fourier_series_exp(z, t, 51, 1); % call func