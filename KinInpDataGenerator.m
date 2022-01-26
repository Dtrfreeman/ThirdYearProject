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
desiredConfigurations=repmat(struct('JointName','Default','JointPosition',0),6,testSampNum);

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

 hold off;


%give a demo rendering of what these actions should look like

tic;
predictedEndTime=0;

curConfiguration=ik('tool0',initTargetPose,weights,q0);

for sampInd=1:testSampNum
    prevConfiguration=curConfiguration;
   curConfiguration=ik('tool0',desiredTransforms(:,:,sampInd),weights,prevConfiguration);
   desiredConfigurations(:,sampInd)=curConfiguration;
   
   predictedEndTime=(toc)*(testSampNum-sampInd)/sampInd;
   fprintf("%u / %u, %f percent, %f seconds in, should take %f seconds",sampInd,testSampNum,100*sampInd/testSampNum,toc,predictedEndTime);

end
fprintf("Calculated ideal configurations for each desired transformation");
for sampInd=1:testSampNum
    show(robotRBT,desiredConfigurations(:,sampInd));
   drawnow
   waitfor(samplingRate);
   axis([-1,1,-1,1,-1,1]);
end

writematrix(desiredConfigurations);
writematrix(desiredTransforms);