function [jacobianCur]=genJacobian(posTF,thetas,homeConfiguration,bodyJacobian)
    jacobianCur=sym (zeros(6,length(thetas)));
    homeConfiguration=double(homeConfiguration);
    homeTF=subs(posTF,thetas,homeConfiguration);
    syms rotMat anglex angley anglez
    
    rotMat=[]
    
    
    
    for i =1:length(thetas)
%         thetaMask=sym (homeConfiguration);
%         %makes all other thetas to home config
%         thetaMask(i)=homeConfiguration(i)+1;
%         %and sets i to be at the home configuration position + 1
%         if bodyJacobian
%             %if its a body jacobian all higher thetas are accounted for
%             for ti=i:length(thetas)
%                 thetaMask(ti)=thetas(ti);
%             end
%         else
%             %if its a space jacobian all lower thetas are accounter for
%             for ti=1:i
%                 thetaMask(ti)=thetas(ti);
%             end
%         end
%         
%         
        
        %use that to derive the end effector position for a given theta
        for axisIndex=1:3
            jacobianCur(axisIndex+3,i)=jacobian(posTF(axisIndex,4),thetas(i));
        end
%         jacobian(1:3,i)=
        
        
    end



end