for j = 1:1:4 % iterate over each plot
    clearvars -except j % clear everything except j
    clf % clear figure
    x1 = @(t) 5.*(t>=0 & t<10); % define x1
    x2 = @(t) 2*x1(t-10); % define x2
    xlinearcomb = @(t) x1(t)+x2(t); % define xlincomb
    funcs = {x1, x2, xlinearcomb, x1}; % use to call proper function to x
    h = @(time) (time>=0 & time<1); % define h
    dtau = 0.1; % define dtau (tau step size)
    tau = -10:dtau:40; % define tau range
    dT=0.5; % define dT (t step size)
    t = -10:dT:40;  % time variable for the time delay and output signal
    y = NaN(1, length (t)); % allocate memory
    x = funcs{j}; % set x to proper function
    for ii=1:length(t)
        if j == 4 % if we are doing y3
            xh = h(t(ii)-tau).*x(tau) + h(t(ii)-tau).*x2(tau); % set xh to sum of convolutions
        else
            xh = h(t(ii)-tau).*x(tau); % calc convolution
        end
        y(ii)=trapz(tau,xh);   % evaluating the overlap integral of x(tau) and h(t-tau)
    %% plotting in fancy way, good to know what the code does, but not a  
    %  necessary requirment
        if j == 4
            plot (t, y, 'k', t (ii), y(ii), 'ok');
            xlabel ('t'); ylabel ('y(t) = \int x1(\tau)h(t-\tau) d\tau + \int x2(\tau)h(t-\tau) d\tau ');
            axis ([tau(1) tau(end) -5 20]); grid;
            drawnow;
        else
            subplot (211)
            plot(tau, x(tau), 'k-', tau, h(t(ii)-tau), 'b--', t(ii), 0, 'ok');
            xlabel('\tau'); axis ([tau(1) tau(end) -1 20]); grid;
            legend('x(\tau)','h(t-\tau)');
            subplot (212)
            plot (t, y, 'k', t (ii), y(ii), 'ok');
            xlabel ('t'); ylabel ('y(t) = \int x(\tau)h(t-\tau) d\tau');
            axis ([tau(1) tau(end) -5 20]); grid;
            drawnow;
        end
    end
end
