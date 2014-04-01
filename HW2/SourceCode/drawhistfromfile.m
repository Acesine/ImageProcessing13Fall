% EE569 HW #1
% Author: Xiang Gao
% USCID:  9216348910
% Email:  xianggao@usc.edu
% Submission Date:

%% Draw data from file
function [ ] = drawhistfromfile( filename )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

fid = fopen(char(filename),'r');
histogram = fscanf(fid,'%d',256);
fclose(fid);
figure;
stem(0:255,histogram,'fill','MarkerSize',4);
title(['histogram of ',filename]);

end