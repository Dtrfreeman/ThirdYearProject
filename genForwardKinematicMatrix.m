syms t0 t1 t2 t3 t4 t5
    d = [290 0 0 302 0 72];
    a = [0 -270 -70 0 0 0];
    theta = [t0 t1+(pi/2) t2 t3 t4 t5];
    alpha = [-pi/2 0 pi/2 -pi/2 pi/2 0];
  hold on
    workingMatrix=eye(4,4);
    for i=1:6
        jointTf(:,:,i)=dhmatrixGen(theta(i), d(i), a(i), alpha(i));
        workingMatrix=workingMatrix*jointTf(:,:,i);
        baseToJoint(:,:,i)=workingMatrix;
        homeConfigRenderMat=subs(workingMatrix,[t0 t1 t2 t3 t4 t5],[0,0,0,0,0,0]);
        plot3(homeConfigRenderMat(1,4),homeConfigRenderMat(2,4),homeConfigRenderMat(3,4),'-or');
    end
    toolTransform=workingMatrix;
    
    syms x y z R ax az ay desiredTf
    
    matrx=[ 1,  0,  0;
            0,  cos(ax),    -sin(ax);
            0,  sin(ax),    cos(ax)];
    matry=[cos(ay),0,sin(ay);0,1,0;-sin(ay),0,cos(ay)];
    matrz=[ cos(az),    -sin(az),0;
            sin(az),    cos(az),0;
            0,0,1];
    rotMat=matrz*matry*matrx;
    
    
    desiredTf(1:3,1:3)=rotMat;
    desiredTf(1:3,4)=[x;y;z];
    desiredTf(4,4)=1;
    desiredAndRealEquality=desiredTf==toolTransform;
    
    
    theta0=t0==atan2(jointTf(2,4),jointTf(1,4));
    %the y and x 
    
    
    %KinematicSolution=solve([desiredAndRealEquality theta0],[t0 t1 t2 t3 t4 t5])
    
    