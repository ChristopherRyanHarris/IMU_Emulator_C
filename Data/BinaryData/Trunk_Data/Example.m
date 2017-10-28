
% Example read/plot code
% clear; clc; close all;

%% Start
Files = {   '../Data_Binary/static_backwardTilt.bin',...
            '../Data_Binary/static_crawl.bin',...
            '../Data_Binary/static_forwardTilt.bin',...
            '../Data_Binary/static_leftHigh.bin',...
            '../Data_Binary/static_rightHigh.bin',...
            '../Data_Binary/static_standing.bin',...
            '../Data_Binary/static_test.bin',...
            '../Data_Binary/walking_2mph_0pct.bin' };

fh1=figure(1); fh2=figure(2); fh3=figure(3); 
fh4=figure(4); fh5=figure(5);
for f=1:numel(Files)
    clf(fh1); clf(fh2); clf(fh3); 
    clf(fh4); clf(fh5);
    
    % Time Stamp,Accel x,Accel y,Accel z,Gyro x,Gyro y,Gyro z,Yaw (Deg),Pitch (Deg),Roll (Deg)
    fid = fopen(Files{f},'r');
    data = fread(fid,inf,'float32');
    fclose(fid);
    data = reshape(data,10,numel(data)/10);

    
    Time  = data(1,:); Time=(Time-Time(1))/1E6;
    A1    = data(2,:);
    A2    = data(3,:);
    A3    = data(4,:);
    G1    = data(5,:);
    G2    = data(6,:);
    G3    = data(7,:);
    Yaw   = data(8,:);
    Pitch = data(9,:);
    Roll  = data(10,:);
    Fs = 1/mean(diff(Time));
    
    figure(fh1);
    hold on; 
    plot(Time,Pitch,'b-');
    plot(Time,Roll,'r-');
    hold off;
    legend({'Pitch','Roll'}, 'Interpreter', 'none');
    title(sprintf('%s : Euler Angles : %2.3f Secs',Files{f},max(Time)), 'Interpreter', 'none');
    xlabel('Time (s)'); ylabel('deg');
    saveas(fh1,strrep(Files{f},'.bin','_EulerAngles'),'png');
    
    figure(fh3)
    hold on;
    plot(Time,A1,'color',[0.9 0 0.1] );
    plot(Time,A2,'color',[0 0.4 0] );
    plot(Time,A3,'color',[0.1 0 1] );
    legend({'A1','A2','A3'}, 'Interpreter', 'none');
    title(sprintf('%s : Accelerations : %2.3f Secs',Files{f},max(Time)), 'Interpreter', 'none');
    xlabel('Time (s)'); ylabel('Accel');
    saveas(fh3,strrep(Files{f},'.bin','_Acceleration'),'png');
    
    figure(fh4)
    hold on;
    plot(Time,G1,'color',[0.9 0 0.1] );
    plot(Time,G2,'color',[0 0.4 0] );
    plot(Time,G3,'color',[0.1 0 1] );
    legend({'G1','G2','G3'}, 'Interpreter', 'none');
    title(sprintf('%s : Gyroscope : %2.3f Secs',Files{f},max(Time)), 'Interpreter', 'none');
    xlabel('Time (s)'); ylabel('Gyro');
    saveas(fh4,strrep(Files{f},'.bin','_Gyroscope'),'png');
    
    
    F = abs(fftshift(fft( Pitch-mean(Pitch),2^(1+(floor(log2(numel(Pitch))))) )));
    xax = -Fs/2:Fs/numel(F):(Fs/2 - Fs/numel(F));
    figure(fh2);
    plot(xax,F);
    title(sprintf('%s : Pitch Freq : Nyq %2.3f Hz',Files{f},Fs/2), 'Interpreter', 'none');
    xlabel('Freq (Hz)'); ylabel('Freq Count');
    saveas(fh2,strrep(Files{f},'.bin','_PitchFreq'),'png');
    
    F = abs(fftshift(fft( Roll-mean(Roll),2^(1+(floor(log2(numel(Roll))))) )));
    xax = -Fs/2:Fs/numel(F):(Fs/2 - Fs/numel(F));
    figure(fh5);
    plot(xax,F);
    title(sprintf('%s : Roll Freq : Nyq %2.3f Hz',Files{f},Fs/2), 'Interpreter', 'none');
    xlabel('Freq (Hz)'); ylabel('Freq Count');
    saveas(fh5,strrep(Files{f},'.bin','_RollFreq'),'png');
    
    pause(3);
end


