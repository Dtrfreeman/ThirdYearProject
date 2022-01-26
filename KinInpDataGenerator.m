 robotRBT=loadrobot("abbIrb120",'Gravity',[0,0,-9.81]);
 %showdetails(robotRBT) 
 %for no gravity
 

 showdetails(robotRBT);
 
 %picks a small number of desired configuration waypoints
 %that the arm has to move between, 
 %between those it calculates transitory points 
 %that should be passed through to reach those waypoints points
 
 cla;
hold on;

 samplesPerRandomWaypoint=20;
 
 numWayPoints=10;
 testSampNum=samplesPerRandomWaypoint*numWayPoints;
 %just to make sure everything is a integer multiple
 
 waypointTransforms=zeros(4,4,numWayPoints);
 waypointConfigurations=repmat(struct('JointName','Default','JointPosition',0),6,numWayPoints);
%the first configuration and transform is the robots home position
waypointTransforms(:,:,1)=getTransform(robotRBT,homeConfiguration(robotRBT),'tool0');
waypointConfigurations(:,1)=homeConfiguration(robotRBT);

for wayPointInd=2:numWayPoints

    curConfiguration=randomConfiguration(robotRBT);
    waypointConfigurations(:,wayPointInd)=curConfiguration;
    waypointTransforms(:,:,wayPointInd)=getTransform(robotRBT,curConfiguration,'tool0');
end
 
%once we have those waypoints we linearly interpolate their transformation
%matrices in order to transition between them 
desiredTransforms=zeros(4,4,testSampNum);

for xIndex=1:4
    for yIndex = 1:4
        prevWrtEnd=0;
        nextWrtEnd=samplesPerRandomWaypoint;
        for wayPointInd=2:numWayPoints
            prevWaypoint=waypointTransforms(xIndex,yIndex,wayPointInd-1);
            nextWaypoint=waypointTransforms(xIndex,yIndex,wayPointInd);
            
            desiredTransforms(xIndex,yIndex,prevWrtEnd+1:nextWrtEnd)=linspace(prevWaypoint,nextWaypoint,samplesPerRandomWaypoint);
            prevWrtEnd=nextWrtEnd;
            nextWrtEnd=nextWrtEnd+samplesPerRandomWaypoint;
        end
    end
end
axis on;
 q0=homeConfiguration(robotRBT);
samplingRate=1/(4*samplesPerRandomWaypoint);
 weights = [1,1,1,1,1,1];
 initTargetPose=desiredTransforms(:,:,1);
 ik=inverseKinematics('RigidBodyTree',robotRBT);
 [configSln,solInfo]=ik('tool0',initTargetPose,weights,q0);
 hold off;


%give a demo rendering of what these actions should look like
show(robotRBT);
axis([-1,1,-1,1,-1,1])
prevWrtEnd=0;
nextWrtEnd=samplesPerRandomWaypoint;

for wayPointInd=2:numWayPoints
    [configSln,solInfo]=ik('tool0',initTargetPose,weights,q0);
        show(robotRBT, waypointConfigurations(:,wayPointInd));
    
   for pathPointInd=2:samplesPerRandomWaypoint
%        
%        nx=desiredTransforms(1,4,prevWrtEnd+pathPointInd);
%        ny=desiredTransforms(2,4,prevWrtEnd+pathPointInd);
%        nz=desiredTransforms(3,4,prevWrtEnd+pathPointInd);
%        px=desiredTransforms(1,4,prevWrtEnd+pathPointInd-1);
%        py=desiredTransforms(2,4,prevWrtEnd+pathPointInd-1);
%        pz=desiredTransforms(3,4,prevWrtEnd+pathPointInd-1);
%        
%        plot3(px,py,pz,nx,ny,nz,'-or');
        
        drawnow
        waitfor(samplingRate);
   end
   prevWrtEnd=nextWrtEnd;
   nextWrtEnd=nextWrtEnd+samplesPerRandomWaypoint;
   show(robotRBT, waypointConfigurations(:,wayPointInd));
   drawnow
   waitfor(samplingRate);
   axis([-1,1,-1,1,-1,1])
   
end