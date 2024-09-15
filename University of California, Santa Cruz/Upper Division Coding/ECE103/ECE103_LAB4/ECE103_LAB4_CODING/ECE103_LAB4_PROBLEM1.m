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
        xlabel('k');
        ylabel('|C_k|');
        title('Magnitude of Fourier Series Coefficients');
        subplot(2,1,2);
        stem(k,angle(Ck)); % plot phase
        xlabel('k');
        ylabel('angle(C_k)');
        title('Phase of Fourier Series Coefficients');
        figure; % plot 3 cycles of the signal 'x' and the reconstructed signal
        t_extended = linspace(min(t),3*max(t),3*length(t)); % 3 periods
        x_reconstructed = zeros(1,length(t_extended)); % initialize reconstruction
        for i = 1:length(k)
            x_reconstructed = x_reconstructed + Ck(i)*exp(1j*k(i)*w0*t_extended); % reconstruct signal
        end
        plot(t_extended,real(x_reconstructed)); % plot reconstructed
        hold on;
        plot(t_extended, [x, x, x],'r', 'LineWidth', 2); % plot signal for e^-x in this case
        xlabel('t');
        ylabel('x(t)');
        title('Reconstructed Signal vs Original Signal');
        legend('Reconstructed Signal', 'Original Signal');
        grid on;
    end
end
t = linspace(0, 2*pi, 1000);  % Generate a sample signal
x = sin(t);                   % Signal
Ck_default = fourier_series_exp(x, t); % Call the function with default arguments
Ck_custom_Nk = fourier_series_exp(x, t, 10); % specify the number of Fourier coefficients
Ck_custom_Nk_plot = fourier_series_exp(x, t, 11, 1); % specify the number of Fourier coefficients and plotting option
