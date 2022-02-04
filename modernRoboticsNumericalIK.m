

initialGuess=zeros(1,6)
prevGuess=initialGuess;
  for i = 1:6
    Slist(1:6,i)=jointToScrewAxis(subs(jointTf(:,:,i),thetas,[0,0,0,0,0,0]),initialGuess,thetas)
  end
   
for sample = 1:1000
 
   
    [thetalist, success] = IKinSpace(Slist, desiredTransforms(:,:,sample), T, initialGuess, eomg, ev)
    prevGuess=thetalist(1,1:6);
    foundConfigurationList(sample,1:6)=thetalist(1,1:6);
    successList(sample)=success;
end

