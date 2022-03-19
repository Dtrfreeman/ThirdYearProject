
syms t0 t1 t2 t3 t4 t5
thetas=[t0 t1 t2 t3 t4 t5];

initialGuess=zeros(1,6)
prevGuess=transpose(initialGuess);
tolOnOrient=10^-3;
tolOnPos=10^-2;

homeTF=subs(toolTransform,thetas,initialGuess);
  for i = 1:6
    Slist(1:6,i)=jointToScrewAxis(subs(jointTf(:,:,i),thetas,[0,0,0,0,0,0]),initialGuess,thetas)
  end
   
for sample = 1:1000
 
   
    [thetalist, success] = IKinSpace(Slist,homeTF,desiredTransforms(:,:,sample),prevGuess, tolOnOrient, tolOnPos)
    prevGuess(1:6,1)=thetalist(1:6,1);
    foundConfigurationList(1:6,sample)=thetalist(1:6,1);
    successList(sample)=success;
end

