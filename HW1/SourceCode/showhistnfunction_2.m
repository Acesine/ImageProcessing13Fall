% EE569 HW #1
% Author: Xiang Gao
% USCID:  9216348910
% Email:  xianggao@usc.edu
% Submission Date: Sep 19, 2013
% Require: drawhistfromfile.m
%% Problem 2: Image Enhancement -- Plotting tasks
% (a)
filenames = {'dark','mid','bright'};
for f=1:3
    % Plot histogram of input image
    datafilename = strcat('barbara_',filenames(f));
    datafilename = strcat(datafilename,'_histdata.dat');
    drawhistfromfile(datafilename);
    
    % Plot transfer function of 'full-range' scaling
    datafilename = strcat('barbara_',filenames(f));
    datafilename = strcat(datafilename,'_transferF_fr.dat');
    fid = fopen(char(datafilename),'r');
    transfer = fscanf(fid,'%d',256);
    fclose(fid);
    figure;
    plot(0:1:255,transfer);
    title(strcat('barbara\_',filenames(f),' transfer function of full-range scaling'));
       
    % Plot histogram of output image of 'full-range' scaling
    datafilename = strcat('barbara_',filenames(f));
    datafilename = strcat(datafilename,'_histdata_fr.dat');
    drawhistfromfile(datafilename);
    
    % Plot transfer function of 'histogram-equalization'
    datafilename = strcat('barbara_',filenames(f));
    datafilename = strcat(datafilename,'_transferF_he.dat');
    fid = fopen(char(datafilename),'r');
    transfer = fscanf(fid,'%d',256);
    fclose(fid);
    figure;
    plot(0:1:255,transfer);
    title(strcat('barbara\_',filenames(f),' transfer function of histogram equalization'));
 
    
    % Plot histogram of output image of 'histogram-equalization'
    datafilename = strcat('barbara_',filenames(f));
    datafilename = strcat(datafilename,'_histdata_he.dat');
    drawhistfromfile(datafilename);
    
end

%% Problem 2
% (b)
% Plot histogram of input image
datafilename = 'ocean_contrast_r_histdata.dat';
drawhistfromfile(datafilename);

datafilename = 'ocean_contrast_g_histdata.dat';
drawhistfromfile(datafilename);

datafilename = 'ocean_contrast_b_histdata.dat';
drawhistfromfile(datafilename);

% Plot histogram of output image of cumulative
datafilename = 'ocean_contrast_r_out_cumu_histdata.dat';
drawhistfromfile(datafilename);

datafilename = 'ocean_contrast_g_out_cumu_histdata.dat';
drawhistfromfile(datafilename);

datafilename = 'ocean_contrast_b_out_cumu_histdata.dat';
drawhistfromfile(datafilename);

% Plot histogram of output image of equalized
datafilename = 'ocean_contrast_r_out_equal_histdata.dat';
drawhistfromfile(datafilename);

datafilename = 'ocean_contrast_g_out_equal_histdata.dat';
drawhistfromfile(datafilename);

datafilename = 'ocean_contrast_b_out_equal_histdata.dat';
drawhistfromfile(datafilename);

% 
% (c)
% Plot histogram of input image
datafilename = 'barbara_contrast_histdata.dat';
drawhistfromfile(datafilename);

% Plot Transfer function
datafilename = 'barbara_contrast_transferF.dat';
fid = fopen(char(datafilename),'r');
transfer = fscanf(fid,'%d',256);
fclose(fid);
figure;
plot(0:1:255,transfer);
title('barbara\_contrast transfer function');

% Plot histogram of output image
datafilename = 'barbara_contrast_out_histdata.dat';
drawhistfromfile(datafilename);


