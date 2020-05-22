% Creating movie of substrates
addpath('../output')
var=1;
timetotal =70;
A = 'output0000000';
A2 = 'output000000';
A3 = 'output00000'; 
A4 = 'output0000'; 
B = '.xml';


v = VideoWriter('sub111.avi')
v.FrameRate = 10;
open(v)
figure
hold on 

for tcount =1:timetotal
    clf
    if tcount<11
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<101
        K = [A2 num2str(tcount-1,'%d') B];
    elseif tcount<1001
        K = [A3 num2str(tcount-1,'%d') B];
    else
        K = [A4 num2str(tcount-1,'%d') B];
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    MCDS.continuum_variables(var).data(:,:,k) , 20 ) ;
    axis image;
    colorbar; 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(var).name , ...
     MCDS.continuum_variables(var).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ...
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
    T_E_Physi{tcount} = MCDS.continuum_variables(1).data(:,:,k);
 
    
    
    frame = getframe(gcf);
    writeVideo(v,frame);
    

end
close(v)

STOP

%%

v = VideoWriter('sub91.avi')
v.FrameRate = 10;
open(v)
figure
hold on 

for tcount = 1:timetotal
    clf
    if tcount<11
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<101
        K = [A2 num2str(tcount-1,'%d') B];
    else
        K = [A3 num2str(tcount-1,'%d') B];
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    MCDS.continuum_variables(2).data(:,:,k) , 20 ) ;
    axis image;
    colorbar;
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(2).name , ...
     MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
 
    frame = getframe(gcf);
    writeVideo(v,frame);
    

end

close(v)

v = VideoWriter('sub92.avi')
v.FrameRate = 10;
open(v)
figure
hold on 

for tcount = 1:timetotal
     clf
    if tcount<10
        K = [A num2str(tcount,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
   else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    MCDS.continuum_variables(5).data(:,:,k) , 20 ) ;
    axis image
    colorbar 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(5).name , ...
     MCDS.continuum_variables(5).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
 
    frame = getframe(gcf)
    writeVideo(v,frame);
   

end
close(v)

v = VideoWriter('sub93.avi')
v.FrameRate = 10;
open(v)
figure
hold on 

for tcount = 1:timetotal
    clf
    
    if tcount<10
        K = [A num2str(tcount,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
   else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    MCDS.continuum_variables(7).data(:,:,k) , 20 ) ;
    axis image;
    colorbar ;
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(7).name , ...
     MCDS.continuum_variables(7).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
 
    frame = getframe(gcf);
    writeVideo(v,frame);
    

end


v = VideoWriter('sub94.avi')
v.FrameRate = 10;
open(v)
figure
hold on 

for tcount = 1:timetotal
    clf
    
    if tcount<10
        K = [A num2str(tcount,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
   else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    MCDS.continuum_variables(9).data(:,:,k) , 20 ) ;
    axis image;
    colorbar ;
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(9).name , ...
     MCDS.continuum_variables(9).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
 
    frame = getframe(gcf);
    writeVideo(v,frame);
    

end



%%
v = VideoWriter('substrate3and4.avi')
v.FrameRate = 1;
open(v)
figure
hold on 
colormap jet

for tcount = 1:750
    clf
    
    if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(3).data(:,:,k)+MCDS.continuum_variables(4).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    sum(sum((MCDS.continuum_variables(3).data(:,:,k)+MCDS.continuum_variables(4).data(:,:,k)).*20*20))
    axis image;
    colorbar ;
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(4).name , ...
     MCDS.continuum_variables(4).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
 
    frame = getframe(gcf);
    writeVideo(v,frame);
    

end


%%
%%
v = VideoWriter('sub4.avi')
v.FrameRate = 10;
open(v)
figure
hold on 
colormap jet

for tcount = 1:205
    clf
    
    if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(7).data(:,:,k)+MCDS.continuum_variables(8).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    sum(sum((MCDS.continuum_variables(7).data(:,:,k)+MCDS.continuum_variables(8).data(:,:,k)).*20*20))
    axis image;
    colorbar ;
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(4).name , ...
     MCDS.continuum_variables(7).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
 
    frame = getframe(gcf);
    writeVideo(v,frame);
    

end




%%

% Creating movie of substrates
addpath('../output')

timetotal = 1;
A = 'output0000000';
A2 = 'output000000';
A3 = 'output00000';
B = '.xml';

colormap jet
figure
hold on 
tcount =1;
     if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(2).data(:,:,k)+MCDS.continuum_variables(2).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    axis image;
    colorbar; 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(2).name , ...
     MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 

figure
hold on 
tcount = 30;
colormap jet

     if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(2).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    axis image;
    colorbar; 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(2).name , ...
     MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
figure
hold on 
tcount = 90;
colormap jet

      if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(2).data(:,:,k)+MCDS.continuum_variables(2).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    axis image;
    colorbar; 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(2).name , ...
     MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
 

figure
hold on 
tcount = 60;
colormap jet
      if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(3).data(:,:,k)+MCDS.continuum_variables(4).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    axis image;
    colorbar; 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(2).name , ...
     MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
 
 
 colormap jet
figure
hold on 
tcount =100;
      if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(3).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    axis image;
    colorbar; 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(2).name , ...
     MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 

figure
hold on 
tcount = 300;
colormap jet

      if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(3).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    axis image;
    colorbar; 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(2).name , ...
     MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
figure
hold on 
tcount = 500;
colormap jet

     if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(3).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    axis image;
    colorbar; 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(2).name , ...
     MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
 

figure
hold on 
tcount = 700;
colormap jet
     if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(3).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    axis image;
    colorbar; 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(2).name , ...
     MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 

colormap jet
figure
hold on 
tcount =100;
     if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(4).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    axis image;
    colorbar; 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(2).name , ...
     MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 

figure
hold on 
tcount = 300;
colormap jet

     if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(4).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    axis image;
    colorbar; 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(2).name , ...
     MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
figure
hold on 
tcount = 500;
colormap jet

     if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(4).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    axis image;
    colorbar; 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(2).name , ...
     MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 
 

figure
hold on 
tcount = 700;
colormap jet
      if tcount<10
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<100 
        K = [A2 num2str(tcount,'%d') B]; 
    else 
        K = [A3 num2str(tcount,'%d') B]; 
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    [MCDS.continuum_variables(4).data(:,:,k)] , 150 ,'EdgeColor','none') ;
    axis image;
    colorbar; 
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s (%s) at t = %3.2f %s, z = %3.2f %s', MCDS.continuum_variables(2).name , ...
     MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units, ... 
     MCDS.mesh.Z_coordinates(k), ...
     MCDS.metadata.spatial_units ) ); 

 
 
 %%
 
% tcount = 2;
 
% tcount = 25;
 

 tcount = 49;
 
 figure
    if tcount<11
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<101
        K = [A2 num2str(tcount-1,'%d') B];
    else
        K = [A3 num2str(tcount-1,'%d') B];
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    MCDS.continuum_variables(2).data(:,:,k) , 20 ) ;
    axis image;
    colorbar;
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
    title( sprintf('%s at t = %3.2f %s', MCDS.continuum_variables(2).name , ...%MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units) );%, ... 
     %MCDS.mesh.Z_coordinates(k), ...
    % MCDS.metadata.spatial_units 
     set(gca,'FontSize',13)
    %frame = getframe(gcf);
    %writeVideo(v,frame);
    %%
    
% tcount = 2;
 
% tcount = 25;
 

 tcount = 49;
 
 figure
    if tcount<11
        K = [A num2str(tcount-1,'%d') B];
    elseif tcount<101
        K = [A2 num2str(tcount-1,'%d') B];
    else
        K = [A3 num2str(tcount-1,'%d') B];
    end
    MCDS = read_MultiCellDS_xml(K);
    k = find( MCDS.mesh.Z_coordinates == 0 ); 
    contourf( MCDS.mesh.X(:,:,k), MCDS.mesh.Y(:,:,k), ...
    MCDS.continuum_variables(3).data(:,:,k) , 20 ) ;
    axis image;
    colorbar;
    xlabel( sprintf( 'x (%s)' , MCDS.metadata.spatial_units) ); 
    ylabel( sprintf( 'y (%s)' , MCDS.metadata.spatial_units) ); 
    
  title( sprintf('%s at t = %3.2f %s', MCDS.continuum_variables(3).name , ...%MCDS.continuum_variables(2).units , ...
     MCDS.metadata.current_time , ...
     MCDS.metadata.time_units) );%, ... 
     %MCDS.mesh.Z_coordinates(k), ...
    % MCDS.metadata.spatial_units 
     set(gca,'FontSize',13)
    %frame = getframe(gcf);
    %writeVideo(v,frame);