function[DHmats] =generateDHMatrices(thetasDegrees,dist,radius,angleDegrees)
thetaRads=deg2rad(thetaDegrees);
angleRads=deg2rad(angleDegrees);
DHmats=eye(4,4,length(thetaDegrees));
for i = 1:length(thetaDegrees)
    DHmats(:,:,i)=dhMatrix(angleRads(i),radius(i),dist(i),thetaRads(i));
end

end