% EE569 HW #2
% Author: Xiang Gao
% USCID:  9216348910
% Email:  xianggao@usc.edu
% Submission Date:

%% Draw data from file
function [ data ] = drawdatafromfile( filename )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

fid = fopen(char(filename),'r');
len = fscanf(fid,'%d',1);
data = fscanf(fid,'%d',len);
fclose(fid);
figure;
stem(0:len-1,data,'fill','MarkerSize',4);

end
