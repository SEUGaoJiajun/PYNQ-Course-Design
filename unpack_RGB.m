clear,clc;
im=imread('D:\sys\FPGAprojects\CourseDesign\1HLS_C\bdaxiTry6_5\bdaxiTry4\bdaxiTry4\4pic.png') ;  %文件路径
length=300*400;
R=im(:,:,1);       % R通道
R=reshape(R,1,length);
G=im(:,:,2);       % G通道
G=reshape(G,1,length);
B=im(:,:,3);      % B通道 
B=reshape(B,1,length);
fid = fopen('save_data.txt','w');

for i=1:length
    fprintf(fid,'%d',B(i));
    if i~=length
        fprintf(fid,',');
    end
end
fprintf(fid,'\r\n');
for i=1:length
    fprintf(fid,'%d',G(i));
    if i~=length
        fprintf(fid,',');
    end
end
fprintf(fid,'\r\n');
for i=1:length
    fprintf(fid,'%d',R(i));
    if i~=length
        fprintf(fid,',');
    end
end
fprintf(fid,'\r\n');
fclose(fid); 

