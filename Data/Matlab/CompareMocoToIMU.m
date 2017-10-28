
% Example read/plot code
clear; clc; close all;


%% Start



IMUFiles = { ... 
            'F1_2.bin',... %2    1.8mph  15deg
            'F2.bin',...   %3    2.7mph  15deg
            'F3.bin',...   %4    1.8mph  0deg
            'F4.bin',...   %5    2.7mph  0deg
            'F5.bin',...   %6    1.8mph  -3deg
            'F6.bin',...   %7    2.7mph  -3deg
            'S1.bin',...   %8    1.8mph  15deg
            'S2.bin',...   %9    2.7mph  15deg
            'S3.bin',...   %10   1.8mph  0deg
            'S4.bin',...   %11   2.7mph  0deg
            'S5.bin',...   %12   1.8mph -3deg
            'S6.bin',...   %13   2.7mph -3deg
            'Z1.bin',...   %14   1.8mph  15deg
            'Z2.bin',...   %15   2.7mph  15deg
            'Z3.bin',...   %16   1.8mph  0deg 
            'Z4.bin',...   %17   2.7mph  0deg
            'Z5.bin',...   %18   1.8mph -3deg
            'Z6.bin' ...   %19   2.7mph -3deg
            };

% Note:
% MoCap Data only applies to Subject 4_2
MoCapFiles = {  'MoCap/F1.csv',... %2
                'MoCap/F2.csv',... %3 
                'MoCap/F3.csv',... %4
                'MoCap/F4.csv',... %5
                'MoCap/F5.csv',... %6
                'MoCap/F6.csv',... %7
                'MoCap/S1.csv',... %8
                'MoCap/S2.csv',... %9
                'MoCap/S3.csv',... %10
                'MoCap/S4.csv',... %11
                'MoCap/S5.csv',... %12
                'MoCap/S6.csv',... %13
                'MoCap/Z1.csv',... %14
                'MoCap/Z2.csv',... %15
                'MoCap/Z3.csv',... %16
                'MoCap/Z4.csv',... %17
                'MoCap/Z5.csv',... %18
                'MoCap/Z6.csv' ... %19
                };

% Z Truth
Incline_Truth = [ ...
                15
                15
                0
                0
                -3
                -3
                15
                15
                0
                0
                -3
                -3
                15
                15
                0
                0
                -3
                -3
                ];
                

f=13;

Subject4_dir = '..\BinaryData\Subject4_2';
Subject3_dir = '..\BinaryData\Subject3_2';

Incline = Incline_Truth(f);

IMU_file   = fullfile(Subject4_dir,IMUFiles{f});
MoCap_file = fullfile(Subject4_dir,MoCapFiles{f});

%% Read MoCap data
 

% O[xyz] Primary[xyz] Secondary[xyz] Center1 Center2 Center3
if( ~isempty(regexp( MoCap_file,'/Z\d+', 'once')) ) % if zappatos (foot data)
  
    
    data = csvread(MoCap_file,8,2);
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

%     fh1=figure(1); hold on; grid on;
%     plot3(lasi(1,1),lasi(1,2),lasi(1,3),'bo')
%     plot3(lpsi(1,1),lpsi(1,2),lpsi(1,3),'ro')
%     plot3(lthi(1,1),lthi(1,2),lthi(1,3),'go')
%     plot3(lkne(1,1),lkne(1,2),lkne(1,3),'bx')
%     plot3(ltib(1,1),ltib(1,2),ltib(1,3),'rx')
%     plot3(lank(1,1),lank(1,2),lank(1,3),'gx')
%     plot3(lhee(1,1),lhee(1,2),lhee(1,3),'bs')
%     plot3(ltoe(1,1),ltoe(1,2),ltoe(1,3),'rs')
%     hold off; xlabel('x'); ylabel('y'); zlabel('z');

    Foot = ltoe-lhee;
    Leg  = ltib-lank;

    X=[1 0 0]; Y=[0 1 0]; Z=[0 0 1];
    % FootVecot dot YAxis
    MoCapPitch  = acos( sum(Foot.*Y,2)./( sqrt(sum(Foot.^2,2)) .* sqrt(sum(Y.^2,2)) )).*180/pi;
    % LegVecot dot ZAxis
    MoCapRoll  = acos( sum(Leg.*Z,2)./( sqrt(sum(Leg.^2,2)) .* sqrt(sum(Z.^2,2)) )).*180/pi;

else % Not foot data
    
    % O[xyz] Primary[xyz] Secondary[xyz] Center1 Center2 Center3 
    data = csvread(MoCap_file,8,2); 
    data = data(:,1:3*6);

    O  = data(:,1:3);
    P  = data(:,4:6) - O;
    S  = data(:,7:9) - O;
    C1 = data(:,10:12) - O;
    C2 = data(:,13:15) - O;
    C3 = data(:,16:18) - O;
    O = O - O;

%     fh1=figure(1); hold on; grid on;
%     plot3(O(1,1),O(1,2),O(1,3),'bo')
%     plot3(P(1,1),P(1,2),P(1,3),'ro')
%     plot3(S(1,1),S(1,2),S(1,3),'go')
%     plot3(C1(1,1),C1(1,2),C1(1,3),'bx')
%     plot3(C2(1,1),C2(1,2),C2(1,3),'rx')
%     plot3(C3(1,1),C3(1,2),C3(1,3),'gx')
%     hold off; 

    % a.b = |a||b|cos(th)
    X=[1 0 0]; Y=[0 1 0]; Z=[0 0 1];
    PStheta  = acos( sum(P.*S,2)./( sqrt(sum(P.^2,2)) .* sqrt(sum(S.^2,2)) )).*180/pi;
    Plane    = cross(P,S,2);
    Pitch1   = -1*asin( sum(P*(Z.'),2)./( sqrt(sum(P.^2,2)) .* sqrt(sum(Z.^2,2)) )).*180/pi;
    Pitch2   = -1*asin( sum(S*(Y.'),2)./( sqrt(sum(S.^2,2)) .* sqrt(sum(Y.^2,2)) )).*180/pi;
    MoCapPitch    = (Pitch1+Pitch2)/2;
    MoCapRoll     = -1*asin( sum(Plane*(Z.'),2)./( sqrt(sum(Plane.^2,2)) .* sqrt(sum(Z.^2,2)) )).*180/pi;
end
MoCapTime = (1:numel(MoCapPitch))/100;
MoCapTime = MoCapTime-MoCapTime(1);

%% Read IMU data 

fid = fopen(IMU_file,'r');
out = fread(fid,inf,'float32');
fclose(fid);
out = reshape(out,10,numel(out)/10).';
m=numel(out(:,1));

IMUTime = out(1:m,1)/1000000;
dt = diff(IMUTime);
dt = [dt(1);dt];
IMUTime=IMUTime-IMUTime(1);

IMUYaw   = out(1:m,8);
IMUPitch = out(1:m,9);
IMURoll  = out(1:m,10);

%% Compare

Threshold=0.5*max(MoCapPitch);
[MoCappeaks,MoCaploc]= findpeaks(MoCapPitch,'MinPeakDistance',100,'MinPeakHeight',Threshold);
MoCaploc=MoCaploc(MoCappeaks>0); MoCappeaks=MoCappeaks(MoCappeaks>0);
% figure(1); clf(1); hold on; grid on;
% plot(MoCapPitch,'b-'); 
% plot(MoCaploc,MoCapPitch(MoCaploc),'ro');
% fprintf('%s:\tMoCap:%d\t',IMUFiles{f}, numel(MoCappeaks))

Threshold=0.5*max(IMUPitch);
[IMUpeaks,IMUloc]= findpeaks(IMUPitch,'MinPeakDistance',200,'MinPeakHeight',Threshold);
IMUloc=IMUloc(IMUpeaks>0); IMUpeaks=IMUpeaks(IMUpeaks>0);
% figure(2); clf(2); hold on; grid on;
% plot(IMUPitch,'b-'); 
% plot(IMUloc,IMUPitch(IMUloc),'ro');
% fprintf('%s:\tIMU:%d\n',MoCapFiles{f}, numel(IMUpeaks))

MoCapFirst = find(MoCapPitch>(max(MoCapPitch(1:50))+abs(max(MoCapPitch(1:50)))*0.5),1,'first');
IMUFirst = find(IMUPitch>(max(IMUPitch(1:50))+abs(max(IMUPitch(1:50)))*0.5),1,'first');

MoCapTime = MoCapTime - mean(MoCapTime(MoCaploc)-IMUTime(IMUloc).');
%MoCapTime = MoCapTime +(IMUTime(IMUFirst) - MoCapTime(MoCapFirst));
% MoCapTime = MoCapTime+1;
% IMUTime=IMUTime*0.99;

figure(1); clf(1); hold on; grid on;
xlabel('Time (s)'); ylabel('Pitch (deg)');
plot(IMUTime,IMUPitch,'b-'); 
plot(MoCapTime,MoCapPitch,'r-');
legend({'IMU','Vicom'});

% end















