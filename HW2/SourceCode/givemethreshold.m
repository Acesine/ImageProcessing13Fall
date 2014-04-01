% EE569 HW #2
% Author: Xiang Gao
% USCID:  9216348910
% Email:  xianggao@usc.edu
% Submission Date:

%% Give threshold
function [ thres ] = givemethreshold( filename , edgeperc, type)

fid = fopen(char(filename),'r');
len = fscanf(fid,'%d',1);
data = fscanf(fid,'%d',len);
if type == 'l'
    data = abs(data);
end
if type == 'd'
    data = abs(data);
end
fclose(fid);

datamin = min(data);
datamax = max(data);
range = datamax - datamin + 1;
hist = zeros(1,range);
for i=1:range
    hist(i) = sum(data == datamin+i-1);
end
cum = cumsum(hist);
t = find(cum>(1-edgeperc)*len); % pick 10% as edge pixel

thres = datamin+t(1)-1;
end