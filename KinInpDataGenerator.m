 robotRBT=loadrobot("abbIrb120",'Gravity',[0,0,-9.81]);
 %showdetails(robotRBT) 
 %for no gravity
%  
% syms t0 t1 t2 t3 t4 t5
%     d = [290 0 0 302 0 72];
%     a = [0 -270 -70 0 0 0];
%     theta = [t0 t1+(pi/2) t2 t3 t4 t5];
%     alpha = [-pi/2 0 pi/2 -pi/2 pi/2 0];
 showdetails(robotRBT);
 
 %picks a small number of desired configuration waypoints
 %that the arm has to move between, 
 %between those it calculates transitory points 
 %that should be passed through to reach those waypoints points
 
 cla;
hold on;

 samplesPerWaypoint=100;
 
 numWayPoints=10;
 testSampNum=samplesPerWaypoint*numWayPoints;
 %just to make sure everything is a integer multiple
 

 waypointConfigurations=repmat(struct('JointName','Default','JointPosition',0),6,numWayPoints);
%the first configuration and transform is the robots home position

waypointConfigurations(:,1)=homeConfiguration(robotRBT);
fprintf("generating random waypoints");
for wayPointInd=2:numWayPoints

    curConfiguration=randomConfiguration(robotRBT);
    waypointConfigurations(:,wayPointInd)=curConfiguration;
    
end
 
%once we have those waypoints we linearly interpolate their transformation
%matrices in order to transition between them 
desiredTransforms=zeros(4,4,testSampNum);
desiredConfigurations=repmat(struct('JointName','Default','JointPosition',0),6,testSampNum);
desiredConfigurations(:,1)=homeConfiguration(robotRBT);
desiredJointPositions=zeros(6,testSampNum);

jointConfigurationSegment=zeros(1,testSampNum);

fprintf("interpolating between waypoint joint configurations");
for jointN=1:6
  
    
   
     prevWrtEnd=0;
     nextWrtEnd=samplesPerWaypoint;
        
    for wayPointInd=2:numWayPoints
        prevWaypointJntVal=waypointConfigurations(jointN,wayPointInd-1).JointPosition;
        nextWaypointJntVal=waypointConfigurations(jointN,wayPointInd).JointPosition;
       
        prevWrtEnd=nextWrtEnd;
        nextWrtEnd=nextWrtEnd+samplesPerWaypoint;
        jointConfigurationSegment(prevWrtEnd+1:nextWrtEnd)=linspace(prevWaypointJntVal,nextWaypointJntVal,samplesPerWaypoint);
    end
    
    for i = 1:testSampNum
        desiredJointPositions(jointN,i)=jointConfigurationSegment(i);
        desiredConfigurations(jointN,i).JointName=waypointConfigurations(jointN,1).JointName;
        desiredConfigurations(jointN,i).JointPosition=jointConfigurationSegment(i);
    end
    
end

for i = 1:testSampNum
    desiredTransforms(:,:,wayPointInd)=getTransform(robotRBT,desiredConfigurations(:,i),'tool0');
   
end
axis on;
 q0=homeConfiguration(robotRBT);
samplingRate=1/(2*samplesPerWaypoint);
%  weights = [0.4,0.4,0.8,0.8,1,1];
%  initTargetPose=desiredTransforms(:,:,1);
%  ik=inverseKinematics('RigidBodyTree',robotRBT);

 hold off;


%give a demo rendering of what these actions should look like

writematrix(desiredJointPositions);
writematrix(desiredTransforms);

% curConfiguration=ik('tool0',initTargetPose,weights,q0);
axis([-1,1,-1,1,-1,1]);

fprintf("Calculated all transformations and configurations\n");
for sampInd=1:testSampNum
    fprintf("frame %u, waypoint %u \n",sampInd,floor(sampInd/samplesPerWaypoint));
    show(robotRBT,desiredConfigurations(:,sampInd));
   drawnow
   waitfor(samplingRate);
   
end
