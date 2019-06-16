%1.uloha Zaostrenie obrazu
I=imread('xondre08.bmp');
H=[-0.5 -0.5 -0.5;-0.5 5 -0.5;-0.5 -0.5 -0.5];
u1= imfilter(I,H);
imwrite(u1,'step1.bmp');
%2.uloha Otocenie obrazu
u2=fliplr(u1);
imwrite(u2,'step2.bmp');
%3.uloha Medianovy filter
u3=medfilt2(u2,[5 5]);
imwrite(u3,'step3.bmp');
%4.uloha Rozmazanie obrazu
H2=[1 1 1 1 1;1 3 3 3 1;1 3 9 3 1;1 3 3 3 1;1 1 1 1 1]/49;
u4=imfilter(u3,H2);
imwrite(u4,'step4.bmp');
%5.uloha Chyba v obraze
u4_1=fliplr(u4);

I_double=im2double(I);
u4_double=im2double(u4_1);

sizeX=min(size(I_double));
sizeY=max(size(I_double));

chyba=0;
for i=1:sizeX 
    for j=1:sizeY
        chyba=chyba+double(abs(I_double(i,j)-u4_double(i,j)));
    end;
end;

chyba=(chyba/(sizeY*sizeX))*255;

file=fopen('reseni.txt','w');
fprintf(file,'chyba=%2.3f\n',chyba);
%6.uloha Roztazeni histogramu
minimum=min(min(im2double(u4)));
maximum=max(max(im2double(u4)));

u5 = imadjust(u4,[minimum maximum],[0 1]);

imwrite(u5,'step5.bmp');
%7.uloha Spocitanie strednej hodnoty a smerodatnej odchylky
s4=im2uint8(u4);
s5=im2uint8(u5);

fprintf(file,'mean_no_hist=%2.3f\n',mean2(s4));
fprintf(file,'std_no_hist=%2.3f\n',std2(s4));
fprintf(file,'mean_hist=%2.3f\n',mean2(s5));
fprintf(file,'std_hist=%2.3f',std2(s5));

%8.uloha Kvantizace obrazu
N=2;
a=0;
b=255;
u8=zeros(sizeX,sizeY);
for i=1:sizeX
    for j=1:sizeY
        u8(i,j)=round(((2^N)-1)*(double(u5(i,j))-a)/(b-a))*(b-a)/((2^N)-1)+a;
    end;
end;


imwrite(uint8(u8),'step6.bmp');


fclose(file);
