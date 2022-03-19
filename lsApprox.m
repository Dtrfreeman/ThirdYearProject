function [coeffs,bestResidual]=lsApprox(func,inpVar,varsToFit,xVec,yVec)

coeffs=zeros(size(varsToFit));

syms residual;
residual=0;
for i=1:length(xVec)
    residual=residual+((subs(func,inpVar,xVec(i)))-yVec(i))^2;
end

for i=1:length(varsToFit)
   pdeForVar(i)=diff(residual,varsToFit(i));
   
end
[coeffs(1),coeffs(2)]=solve(pdeForVar);
hold on
scatter(xVec,yVec);
approxFunc=subs(func,varsToFit,coeffs);
plot(min(xVec):0.1:max(xVec),subs(approxFunc,inpVar,min(xVec):0.1:max(xVec)))

bestResidual=subs(residual,varsToFit,coeffs);

fprintf("minimum sum of squares of residuals: %g",bestResidual);
hold off


end