
% Example read/plot code
clear; clc; close all;

%% Start
Files = {   '../Kyle/IMU_MoCap/F1.csv',...
            '../Kyle/IMU_MoCap/F2.csv',...
            '../Kyle/IMU_MoCap/F3.csv',...
            '../Kyle/IMU_MoCap/F4.csv',...
            '../Kyle/IMU_MoCap/F5.csv',...
            '../Kyle/IMU_MoCap/F6.csv',...
            '../Kyle/IMU_MoCap/S1.csv',...
            '../Kyle/IMU_MoCap/S2.csv',...
            '../Kyle/IMU_MoCap/S3.csv',...
            '../Kyle/IMU_MoCap/S4.csv',...
            '../Kyle/IMU_MoCap/S5.csv',...
            '../Kyle/IMU_MoCap/S6.csv',...
            '../Kyle/IMU_MoCap/Z1.csv',...
            '../Kyle/IMU_MoCap/Z2.csv',...
            '../Kyle/IMU_MoCap/Z3.csv',...
            '../Kyle/IMU_MoCap/Z4.csv',...
            '../Kyle/IMU_MoCap/Z5.csv',...
            '../Kyle/IMU_MoCap/Z6.csv' };
%%

fh1=figure(1); fh2=figure(2); fh3=figure(3); fh4=figure(4); fh5=figure(5);
for f = 13 : numel(Files)
    
    % O[xyz] Primary[xyz] Secondary[xyz] Center1 Center2 Center3
    if( ~isempty(regexp( Files{f},'/Z\d+', 'once')) )
        data = csvread(Files{f},8,2);
        data = data(:,1:16*3);

        lank = data(:,(7*3+1):(3+3*7));
        lhee = data(:,(8*3+1):(3+3*8));
        ltoe = data(:,(9*3+1):(3+3*9));
        
        lasi = data(:,(0*3+1):(3+3*0));
        lpsi = data(:,(2*3+1):(3+3*2));
        lthi = data(:,(4*3+1):(3+3*4));
        lkne = data(:,(5*3+1):(3+3*5));
        ltib = data(:,(6*3+1):(3+3*6));
        %lank = lank-lank; % lank shall be our origin
        
        rasi = data(:,(1*3+1):(3+3*1));
        rpsi = data(:,(3*3+1):(3+3*3));
        rthi = data(:,(10*3+1):(3+3*10));
        rkne = data(:,(11*3+1):(3+3*11));
        rtib = data(:,(12*3+1):(3+3*12));
        rank = data(:,(13*3+1):(3+3*13));
        rhee = data(:,(14*3+1):(3+3*14));
        rtoe = data(:,(15*3+1):(3+3*15));

%         fh1=figure(1); hold on; grid on;
%         plot3(lasi(1,1),lasi(1,2),lasi(1,3),'bo')
%         plot3(lpsi(1,1),lpsi(1,2),lpsi(1,3),'ro')
%         plot3(lthi(1,1),lthi(1,2),lthi(1,3),'go')
%         plot3(lkne(1,1),lkne(1,2),lkne(1,3),'bx')
%         plot3(ltib(1,1),ltib(1,2),ltib(1,3),'rx')
%         plot3(lank(1,1),lank(1,2),lank(1,3),'gx')
%         plot3(lhee(1,1),lhee(1,2),lhee(1,3),'bs')
%         plot3(ltoe(1,1),ltoe(1,2),ltoe(1,3),'rs')
%         hold off; xlabel('x'); ylabel('y'); zlabel('z');
%         clf(fh1);
        
        Foot = ltoe-lhee;
        Leg  = ltib-lank;
        
        X=[1 0 0]; Y=[0 1 0]; Z=[0 0 1];
        % FootVecot dot YAxis
        Pitch  = acos( sum(Foot.*Y,2)./( sqrt(sum(Foot.^2,2)) .* sqrt(sum(Y.^2,2)) )).*180/pi;
        % LegVecot dot ZAxis
        Roll  = acos( sum(Leg.*Z,2)./( sqrt(sum(Leg.^2,2)) .* sqrt(sum(Z.^2,2)) )).*180/pi;
        
    else
        % O[xyz] Primary[xyz] Secondary[xyz] Center1 Center2 Center3 
        data = csvread(Files{f},8,2);
        data = data(:,1:3*6);

        O  = data(:,1:3);
        P  = data(:,4:6) - O;
        S  = data(:,7:9) - O;
        C1 = data(:,10:12) - O;
        C2 = data(:,13:15) - O;
        C3 = data(:,16:18) - O;
        O = O - O;

%         fh1=figure(1); hold on; grid on;
%         plot3(O(1,1),O(1,2),O(1,3),'bo')
%         plot3(P(1,1),P(1,2),P(1,3),'ro')
%         plot3(S(1,1),S(1,2),S(1,3),'go')
%         plot3(C1(1,1),C1(1,2),C1(1,3),'bx')
%         plot3(C2(1,1),C2(1,2),C2(1,3),'rx')
%         plot3(C3(1,1),C3(1,2),C3(1,3),'gx')
%         hold off; clf(fh1);
    
        % a.b = |a||b|cos(th)
        X=[1 0 0]; Y=[0 1 0]; Z=[0 0 1];
        PStheta  = acos( sum(P.*S,2)./( sqrt(sum(P.^2,2)) .* sqrt(sum(S.^2,2)) )).*180/pi;
        Plane    = cross(P,S,2);
        Pitch1   = -1*asin( sum(P*(Z.'),2)./( sqrt(sum(P.^2,2)) .* sqrt(sum(Z.^2,2)) )).*180/pi;
        Pitch2   = -1*asin( sum(S*(Y.'),2)./( sqrt(sum(S.^2,2)) .* sqrt(sum(Y.^2,2)) )).*180/pi;
        Pitch    = (Pitch1+Pitch2)/2;
        Roll     = -1*asin( sum(Plane*(Z.'),2)./( sqrt(sum(Plane.^2,2)) .* sqrt(sum(Z.^2,2)) )).*180/pi;
    end
    
    Time = (1:numel(Pitch))/100;
    X1 = [floor(numel(Time)/2):floor(numel(Time)/2)+floor(numel(Time)/30)];
    
    % plot pitch roll - full
    clf(fh1); figure(fh1);
    hold on; 
    plot(Time,Pitch,'b-');
    plot(Time,Roll,'r-');
    hold off;
    legend({'Pitch','Roll'}, 'Interpreter', 'none');
    title(sprintf('%s : Euler Angles : %2.3f Secs',Files{f},max(Time)), 'Interpreter', 'none');
    xlabel('Time (s)');
    ylabel('deg');
    %saveas(fh1,strrep(Files{f},'.csv','_EulerAngles'),'png');
    
    % plot 
    clf(fh2); figure(fh2)
    hold on; grid on;
    plot(Time(X1),Pitch(X1),'b-');
    plot(Time(X1),Roll(X1),'r-');
    hold off;
    legend({'Pitch','Roll'}, 'Interpreter', 'none');
    title(sprintf('%s : Euler Angles : %2.3f Secs',Files{f},max(Time)), 'Interpreter', 'none');
    xlabel('Time (s)'); 
    ylabel('deg');
    %saveas(fh2,strrep(Files{f},'.csv','_EulerAngles_z'),'png');
    
    % get freq data
    Fs = 100;
    F = abs(fftshift(fft( Pitch-mean(Pitch),2^(1+(floor(log2(numel(Pitch))))) )));
    xax = -Fs/2:Fs/numel(F):(Fs/2 - Fs/numel(F));
    X2 = [floor(numel(xax)/2):floor(numel(xax)/2)+floor(numel(xax)/15)];
    
    %plot pich freq data - full
    figure(fh3); clf(fh3);
    plot(xax,F);
    title(sprintf('%s : Pitch Freq : Nyq %2.3f Hz',Files{f},Fs/2), 'Interpreter', 'none');
    xlabel('Freq (Hz)'); ylabel('Freq Count');
    %saveas(fh3,strrep(Files{f},'.csv','_PitchFreq'),'png');

    %plot pitch freq data - zoomed
    figure(fh4); clf(fh4); 
    plot(xax(X2),F(X2));  grid on;
    title(sprintf('%s : Pitch Freq : Nyq %2.3f Hz',Files{f},Fs/2), 'Interpreter', 'none');
    xlabel('Freq (Hz)'); ylabel('Freq Count');
    %saveas(fh4,strrep(Files{f},'.csv','_PitchFreq_z'),'png');

%     % get roll freq data
%     Fs = 100;
%     F = abs(fftshift(fft( Roll-mean(Roll),2^(1+(floor(log2(numel(Roll))))) )));
%     xax = -Fs/2:Fs/numel(F):(Fs/2 - Fs/numel(F));
%     X2 = [floor(numel(xax)/2):floor(numel(xax)/2)+floor(numel(xax)/15)];
%     
%     % plot roll freq data -full
%     figure(fh2); clf(fh2);
%     plot(xax,F);
%     title(sprintf('%s : Roll Freq : Nyq %2.3f Hz',Files{f},Fs/2), 'Interpreter', 'none');
%     xlabel('Freq (Hz)'); ylabel('Freq Count');
%     %saveas(fh2,strrep(Files{f},'.csv','_RollFreq'),'png');
% 
%     % plot freq data - zoomed
%     figure(fh2); clf(fh2); 
%     plot(xax(X2),F(X2));  grid on;
%     title(sprintf('%s : Roll Freq : Nyq %2.3f Hz',Files{f},Fs/2), 'Interpreter', 'none');
%     xlabel('Freq (Hz)'); ylabel('Freq Count');
%     %saveas(fh2,strrep(Files{f},'.csv','_RollFreq_z'),'png');
   
    pause(5);
end














        
        