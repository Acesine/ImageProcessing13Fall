name = 'naruto';
%% left2right
movieObj = VideoWriter([name,'_sliding_l2r.avi']); %% Create movie object
open(movieObj ); %% Open movie object
nFrames = 129;
tempFrame(nFrames) = struct('cdata',[],'colormap',[]); %% Create tempFrame
c = zeros(256,256,3);
for j=1:nFrames %% Looping to create video
    filename = sprintf([name,'_l2r_%d_out.raw'],j-1);
    fid=fopen(filename); 
    a=fread(fid,[256*3,256],'uchar'); 
    fclose(fid); 
    a=a';
    r = a(:,1:3:end);
    g = a(:,2:3:end);
    b = a(:,3:3:end);
    c(:,:,1) = r;
    c(:,:,2) = g;
    c(:,:,3) = b;
    c = uint8(c);
    
    imshow(c);%% image processing operation
    tempFrame(j) = getframe; %% reading created image into temporary frame
    writeVideo(movieObj, tempFrame(j)); %% writing the temporary frame into movie object
end
close(movieObj); %% Close movie object
movie(tempFrame,1); %% Play created video

%% lefttop2rightbottom
movieObj = VideoWriter([name,'_sliding_lt2rb.avi']); %% Create movie object
open(movieObj ); %% Open movie object
nFrames = 257;
tempFrame(nFrames) = struct('cdata',[],'colormap',[]); %% Create tempFrame
c = zeros(256,256,3);
for j=1:nFrames %% Looping to create video
    filename = sprintf([name,'_lt2rb_%d_out.raw'],j-1);
    fid=fopen(filename); 
    a=fread(fid,[256*3,256],'uchar'); 
    fclose(fid); 
    a=a';
    r = a(:,1:3:end);
    g = a(:,2:3:end);
    b = a(:,3:3:end);
    c(:,:,1) = r;
    c(:,:,2) = g;
    c(:,:,3) = b;
    c = uint8(c);
    
    imshow(c);%% image processing operation
    tempFrame(j) = getframe; %% reading created image into temporary frame
    writeVideo(movieObj, tempFrame(j)); %% writing the temporary frame into movie object
end
close(movieObj); %% Close movie object
movie(tempFrame,1); %% Play created video
%% Fade-in
movieObj = VideoWriter([name,'_fadein.avi']); %% Create movie object
open(movieObj ); %% Open movie object
nFrames = 101;
tempFrame(nFrames) = struct('cdata',[],'colormap',[]); %% Create tempFrame
c = zeros(256,256,3);
for j=1:nFrames %% Looping to create video
    filename = sprintf([name,'_fadein_%d_out.raw'],j-1);
    fid=fopen(filename); 
    a=fread(fid,[256*3,256],'uchar'); 
    fclose(fid); 
    a=a';
    r = a(:,1:3:end);
    g = a(:,2:3:end);
    b = a(:,3:3:end);
    c(:,:,1) = r;
    c(:,:,2) = g;
    c(:,:,3) = b;
    c = uint8(c);
    
    imshow(c);%% image processing operation
    tempFrame(j) = getframe; %% reading created image into temporary frame
    writeVideo(movieObj, tempFrame(j)); %% writing the temporary frame into movie object
end
close(movieObj); %% Close movie object
movie(tempFrame,1); %% Play created video

