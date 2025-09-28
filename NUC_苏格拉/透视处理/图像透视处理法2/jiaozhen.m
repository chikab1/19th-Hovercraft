% NUC_yw2022.8.2
clear,clc
Array_forward_bucket=cell(120,188);%��������
Array_backward_bucket=cell(120,188);
Array_forward_bird=cell(120,188);
Array_backward_bird=cell(120,188);
st.row=0;st.col=0;
for m=1:120                      %����
      for n=1:188
         Array_forward_bucket{m,n}=st;
         Array_backward_bucket{m,n}=st;
         Array_forward_bird{m,n}=st;
         Array_backward_bird{m,n}=st;
      end
end
img_origin1 = imread('C:\Users\admin\Desktop\NUC_�ո���\͸�Ӵ���\ͼ��͸�Ӵ���2\4.BMP'); %��ȡͼ���ļ�;  
img_origin = img_origin1(1:1:120,1:1:188);
img_origin1 = img_origin;
 %%Ͱ�λ������
k1 = -0.00000;  % �α����������ʵ���������     û��Ͱ�λ�����Ϊ0
k2 = -0.00000;  % �α����������ʵ���������     û��Ͱ�λ�����Ϊ0
  
img_size = size( img_origin );  
img_undist = zeros( img_size );  
img_undist = uint8( img_undist );  
  
for l1 = 1:img_size(1)  % ��ֱ����        
    y = l1 - img_size(1)/2;    
    for l2 = 1:img_size(2)  % ˮƽ����  
        x = l2 - img_size(2)/2;  
        x1 = round( x * ( 1 + k1 * x * x + k2 * y * y ) );  
        y1 = round( y * ( 1 + k1 * x * x + k2 * y * y ) );  
        y1 = y1 + img_size(1)/2;   
        x1 = x1 + img_size(2)/2;  
        img_undist(l1,l2) = img_origin(y1, x1);  
        Array_forward_bucket{l1,l2}.row=y1;
        Array_forward_bucket{l1,l2}.col=x1;
        Array_backward_bucket{y1,x1}.row= l1;
        Array_backward_bucket{y1,x1}.col= l2;
    end  
end  

figure(1);  
img_origin_show=img_origin(1:1:120,1:1:188);  
img_undist_show=img_undist(1:1:120,1:1:188);  
subplot(121); imshow(img_origin_show);  
subplot(122); imshow(img_undist_show); 
  %%Ͱ�λ����������
  
  %%���λ������
imwrite(img_undist,'line1_tong.bmp');  
A=imread('line1_tong.bmp');
% ������ͼ�񷽷�����λ�� ����h ��alpha �� beta ������������������������Ϊֹ  
h=0.40;%��ֱ�߶�
alpha=155.0*pi/180;%��ֱ�ӽ� 153  128  155
beta=175*pi/180;%ˮƽ�ӽ� 123  126  175

theta=atan(90*pi/180);%��ֱ���� 81.2 85

 


 
[m, n]=size(A);
B=A;
after_row=120;
after_col=188;
C=zeros(after_row,after_col);
% ������͸��ͼ���Ч��
zoom2=1300;%27.9 90    ͼ�������չ    (�Ĵ��������) 800
zoom1=60;%48.8 130    ͼ��������չ    (�Ĵ���������)  70
 rowadd =15;%��͸��ͼ������(��������)
coladd=5;%��͸��ͼ��������
 
for j=1:after_row
    for i=1:after_col
        x=(-(j-1)+after_row+rowadd)/zoom1;
        y=((i-1)-after_col/2+coladd)/zoom2;
             
        u_inv=(x*m-m*h*tan(theta))/(2*x*tan(theta)*tan(alpha)+2*h*tan(alpha));
        tan_puOg=tan(alpha)*2*u_inv/m;
        tan_puOp=y/sqrt(h^2+x^2);
        tan_pvOg=tan_puOp*sqrt(1+tan_puOg^2);
        v_inv=tan_pvOg*n/tan(beta)/2;
        
        Xt=u_inv+m/2;
        Yt=-v_inv+n/2;            
        
         sx=floor(Xt);
         sy=floor(Yt);
         a=Xt-sx;b=Yt-sy;       %С������     %�����ǲ�ֵ�㷨����������ڲ�ֵ��
         if((b<0.5))
             Yt=sy;
         else
             Yt=sy+1;
         end
         if((a<0.5))
             Xt=sx;
         else
             Xt=sx+1;
         end
        if Xt<0.5 || Xt>m  || Yt<0.5 || Yt>n
             continue;
        end
        C(j,i)=img_origin1(round(Xt),round(Yt));
       
        Array_forward_bird{j,i}.row= round(Xt);
        Array_forward_bird{j,i}.col= round(Yt);
     end
end
figure(3);%����һ������
C=uint8(C);
subplot(121);
imshow(C); 
subplot(122);
imshow(img_origin1); 
for u=1:after_row
    for v=1:after_col
        u_inv=u-m/2;
        v_inv=n/2-v;
        x=h*(m*tan(theta)+2*u_inv*tan(alpha))/(m-2*u_inv*tan(theta)*tan(alpha));
        
        mmm= 3.55;%3.2��
        y=(mmm*v_inv*tan(beta)*(x*tan(theta)+h))/(n*(tan(theta)^2+1));
 
        Xt=after_row-x*zoom1+rowadd;
 
        Yt=y*zoom2+after_col/2-coladd + 0.5;
               sx=floor(Xt);
         sy=floor(Yt);
         a=Xt-sx;b=Yt-sy;       %С������     %�����ǲ�ֵ�㷨����������ڲ�ֵ��
         if((b<0.5))
             Yt=sy;
         else
             Yt=sy+1;
         end
         if((a<0.5))
             Xt=sx;
         else
             Xt=sx+1;
         end
   if Xt<0.5 || Xt>m  || Yt<0.5 || Yt>n
             continue;
        end
       D(u,v)=C(round(Xt),round(Yt));
             
        Array_backward_bird{u,v}.row= round(Xt);
        Array_backward_bird{u,v}.col= round(Yt);
end 
end
D=uint8(D);figure(4);
subplot(121);imshow(img_undist);subplot(122);imshow(D);
 
fidout=fopen('Array_backward_bird_col.txt','w');
for s=1:120
    for t=1:188
      if  Array_backward_bird{s,t}.col<1
   fprintf(fidout,'%d,',Array_backward_bird{s,t}.col);
    else  
 fprintf(fidout,'%d,',Array_backward_bird{s,t}.col-1);
      end
   end
fprintf(fidout,'\n');
end
 fclose(fidout);
 
 
fidout=fopen('Array_backward_bird_row.txt','w');
for s=1:120
    for t=1:188
      if  Array_backward_bird{s,t}.row<1
   fprintf(fidout,'%d,',Array_backward_bird{s,t}.row);
    else  
 fprintf(fidout,'%d,',Array_backward_bird{s,t}.row-1);
end
end
fprintf(fidout,'\n');
  end
 fclose(fidout);
 
 
 
fidout=fopen('Array_forward_bird_row.txt','w');
for s=1:120
    for t=1:188
      if  Array_forward_bird{s,t}.row<1
      fprintf(fidout,'%d,',Array_forward_bird{s,t}.row);
      else  
      fprintf(fidout,'%d,',Array_forward_bird{s,t}.row-1);
      end
    end
     fprintf(fidout,'\n');
 end
 fclose(fidout);
 
 
 fidout=fopen('Array_forward_bird_col.txt','w');
for s=1:120
    for t=1:188
      if  Array_forward_bird{s,t}.col<1
      fprintf(fidout,'%d,',Array_forward_bird{s,t}.col);
      else  
      fprintf(fidout,'%d,',Array_forward_bird{s,t}.col-1);
      end
    end
     fprintf(fidout,'\n');
 end
 fclose(fidout);
 
 
%�򻯳���  �ο�������� ������ţ�ƣ�������   �򲩸��¾���������