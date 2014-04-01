movieObj = VideoWriter('Rotation.avi'); %% Create movie object
open(movieObj ); %% Open movie object
nFrames = 90;
Isize = 200;
tempFrame(nFrames) = struct('cdata',[],'colormap',[]); %% Create tempFrame
c = zeros(Isize,Isize,3);
for j=1:nFrames %% Looping to create video
    filename = sprintf('C_capture_%d_out.raw',j);
    fid=fopen(filename); 
    a=fread(fid,[Isize*3,Isize],'uchar'); 
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