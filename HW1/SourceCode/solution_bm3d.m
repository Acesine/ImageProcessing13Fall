% EE569 HW #1
% Author: Xiang Gao
% USCID:  9216348910
% Email:  xianggao@usc.edu
% Require: files in BM3D software which is provided on http://www.cs.tut.fi/∼foi/GCF-BM3D/
% Submission Date:

% Read raw image:
fid=fopen('lena.raw'); 
a=fread(fid,[256,256],'uchar'); 
fclose(fid); 
a=a'; 
y = double(a/255);
imshow(y);
% for i = 1:1:4
%     sigma = 15+i*5;
%     [NA, y_est] = BM3D(1, y, sigma); 
%     figure;
%     imshow(y_est);
% end
sigma = 30;
[NA, y_est] = BM3D(1, y, sigma); 
figure;
imshow(y_est);
A = uint8(y_est*255);
A=A';
fid=fopen('lena_BM3D.raw','w'); 
fwrite(fid,A,'uchar'); 
fclose(fid); 
