syms t0 t1 t2 t3 t4 t5
%of abb irb 120
    d = [290 0 0 302 0 72];
    a = [0 -270 -70 0 0 0];
    thetaDH = [t0 t1+(pi/2) t2 t3 t4 t5];
    alpha = [-pi/2 0 pi/2 -pi/2 pi/2 0];
  hold on
    workingMatrix=eye(4,4);
    for i=1:6
        jointTf(:,:,i)=dhmatrixGen(thetaDH(i), d(i), a(i), alpha(i));
        workingMatrix=workingMatrix*jointTf(:,:,i);
        baseToJoint(:,:,i)=workingMatrix;
        homeConfigRenderMat=subs(workingMatrix,[t0 t1 t2 t3 t4 t5],[0,0,0,0,0,0]);
        plot3(homeConfigRenderMat(1,4),homeConfigRenderMat(2,4),homeConfigRenderMat(3,4),'-or');
    end
    toolTransform=workingMatrix;
    

jointToBase=zeros(4,4,6);
workingMatrix=eye(4,4);
% jointTwist=zeros(6,6);
% for i=1:6
%     workingMatrix=workingMatrix*dhmatrixGen(theta(i), d(i), a(i), alpha(i))^-1;
%     jointToBase(:,:,(6-i)+1)=workingMatrix;
%     
% end
% bodyTf=workingMatrix;

%the transform from the tool to the base

initialGuess=[0 0 0 0 0 0];
curJointEst=initialGuess;
thetas=[t0 t1 t2 t3 t4 t5];
desiredPos=double(desiredPos);

% SYMcurToDesiredTF=(1/toolTransform)*desiredPos;

angleThresh=pi/100;
distThresh=10^-2;
iterations=0;
jacobianMatOfTF=double(zeros(4,4,6));
symJacobian=sym(zeros(4,4,6));
for mi=1:4
    for ni=1:4
        symJacobian(mi,ni,1:6)=jacobian(toolTransform(mi,ni),thetas);
    end
end
doneFlag=0;
while doneFlag==0
     curToDesiredTF=double((subs(toolTransform,thetas,curJointEst)^-1)*desiredPos);
        bodyTwist=log(curToDesiredTF);
    
        
    for mi=1:4
        for ni=1:4
            jacobianMatOfTF(mi,ni,1:6)=subs(symJacobian(mi,ni,1:6),thetas,curJointEst)
            curToDesiredTF(mi,ni)
        end
    end
    iterations=iterations+1;
    
end


