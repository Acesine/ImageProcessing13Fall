movieObj = VideoWriter('cross-dissolving-3-1.avi'); %% Create movie object
movieObj.FrameRate = 8;
open(movieObj ); %% Open movie object
nFrames = 101;
IsizeI = 400;
IsizeJ = 400;
tempFrame(nFrames) = struct('cdata',[],'colormap',[]); %% Create tempFrame
c = zeros(IsizeI,IsizeJ,3);
for j=1:nFrames %% Looping to create video
    filename = sprintf('%d.raw',j-1);
    fid=fopen(filename); 
    a=fread(fid,[IsizeJ*3,IsizeI],'uchar'); 
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