function [ScrewAxisOut]=jointToScrewAxis(jointTransform,homeConfig,thetas)
    homeJointTf=double(subs(jointTransform,thetas,homeConfig));
    ScrewAxisOut(4:6)=homeJointTf(1:3,4);
    axang=rotm2axang(homeJointTf(1:3,1:3));
    ScrewAxisOut(1:3)=axang(1:3).*axang(4)
end