myaxes = axes('xlim', [-4,4], 'ylim', [-4,4]); 
view(2);
grid on;
hold on
xlabel('x');
ylabel('y');

R=1;

x1 = [-4 -3 -3 -4];
y1 = [0 0 1 1];
p(1) = patch(x1,y1,'red');

x2 = [-3 -2 -2 -3];
y2 = [0 0 1 1];
p(2) = patch(x2,y2,'red');

x3 = [-2 -1 -1 -2];
y3 = [0 0 1 1];
p(3) = patch(x3,y3,'red');

x4 = [-1 0 0 -1];
y4 = [0 0 1 1];
p(4) = patch(x4,y4,'red');

x5 = [0 1 1 0];
y5 = [0 0 1 1];
p(5) = patch(x5,y5,'red');

x6 = [1 2 2 1];
y6 = [0 0 1 1];
p(6) = patch(x6,y6,'red');

x7 = [2 3 3 2];
y7 = [0 0 1 1];
p(7) = patch(x7,y7,'red');

x8 = [3 4 4 3];
y8 = [0 0 1 1];
p(8) = patch(x8,y8,'red');

%serial port
s = serial('COM7');
set(s, 'BaudRate', 9600);
set(s, 'DataBits', 8);
set(s, 'StopBits', 1);

%open serial port and read in values continuosly
fopen(s);
s.ReadAsyncMode = 'continuous';

check = 1; 

while (true)
    
    %collect data from sensor recognizing from which serial port it comes
        
        
        if (check==1)
        string = fscanf(s, '%s');
        while(strcmp(string, 'A') == 0)
        string = fscanf(s, '%s');
        end
        check = 0;
        end
        
         pixel1 = fscanf(s, '%d');
         pixel2 = fscanf(s, '%d');
         pixel3 = fscanf(s, '%d');
         pixel4 = fscanf(s, '%d');
         pixel5 = fscanf(s, '%d');
         pixel6 = fscanf(s, '%d');
         pixel7 = fscanf(s, '%d');
         pixel8 = fscanf(s, '%d');
         
         n = fscanf(s, '%s');
         
         R1 = -(pixel1-700)/700;
        
        if (R1>1)
            R1 = 1;
        end
        
        if(R1<0)
           R1 = 0;
        end
        
        set(p(1),'FaceColor', [R1 R1 R1]);
        
        R2 = -(pixel2-700)/700;
        
        if (R2>1)
            R2 = 1;
        end
        
        if(R2<0)
           R2 = 0;
        end
            set(p(2),'FaceColor', [R2 R2 R2]);
        
        R3 = -(pixel3-700)/700;
        
        if (R3>1)
            R3 = 1;
        end
        
        if(R3<0)
           R3 = 0;
        end
        
            set(p(3),'FaceColor', [R3 R3 R3]);
        
        R4 = -(pixel4-700)/700;
        
        if (R4>1)
            R4 = 1;
        end
        
        if(R4<0)
           R4 = 0;
        end
        
            set(p(4),'FaceColor', [R4 R4 R4]);
            
        R5 = -(pixel5-700)/700;
        
        if (R5>1)
            R5 = 1;
        end
        
        if(R5<0)
           R5 = 0;
        end
        
            set(p(5),'FaceColor', [R5 R5 R5]);
        
        R6 = -(pixel6-700)/700;
        
        if (R6>1)
            R6 = 1;
        end
        
        if(R6<0)
           R6 = 0;
        end
        
            set(p(6),'FaceColor', [R6 R6 R6]);
            
         R7 = -(pixel7-700)/700;
        
        if (R7>1)
            R7 = 1;
        end
        
        if(R7<0)
           R7 = 0;
        end
        
            set(p(7),'FaceColor', [R7 R7 R7]);
            
        R8 = -(pixel8-700)/700;
        
        if (R8>1)
            R8 = 1;
        end
        
        if(R8<0)
           R8 = 0;
        end
        
            set(p(8),'FaceColor', [R8 R8 R8]);
        
        drawnow;
        
        %wait for short period of time
        pause(0.001);
        
    
end