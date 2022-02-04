function [outMat]=zeroErrorChecker(inMat,variable1unchecked,variable2unchecked)
outMat=inMat;

variable1 = sym(variable1unchecked);
variable2 = sym(variable2unchecked);

dontCheckVar1=isSymType(variable1,'rational');
dontCheckVar2=isSymType(variable2,'rational');

if (dontCheckVar1+dontCheckVar2)~=2

for y = 1:(size(inMat,1)-1)
    for x = 1:(size(inMat,1))
    %for each element in the rotational element of the matrix
    %set each variable 
    if outMat(y,x)~=0
    isNonZero=0;
    sym curOutput;
    curOutput=0;
    for i1 = -1:1+(-2*dontCheckVar1)
        for i2 = -1:1+(-2*dontCheckVar2)
            
            if dontCheckVar1==0
                curOutput=subs(outMat(y,x),variable1,(-pi*i1)/2);
            end
            if dontCheckVar2==0
                curOutput=subs(curOutput,variable2,(-pi*i2)/2);
            end
            
            if abs(curOutput) > 7e-16
                isNonZero=1;
                break;
            end
        end
        if abs(curOutput) > 7e-16
                isNonZero=1;
                break;
            end
        
    end
    
    if isNonZero == 0
        outMat(y,x)=0;
    end
    end
    
    
    
    end
end
end

end

