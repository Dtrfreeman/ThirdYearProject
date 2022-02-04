for sampInd=1:200
    show(robotRBT,desiredConfigurations(:,sampInd));
   drawnow
   waitfor(1/60);
   axis([-1,1,-1,1,-1,1]);
end