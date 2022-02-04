function [dh]=dhmatrixGen(o, d, r, a)
  
    dh=[cos(o),-sin(o)*cos(a),sin(o)*sin(a),r*cos(o);
        sin(o),cos(o)*cos(a),-cos(o)*sin(a),r*sin(o);
        0     ,sin(a)       ,  cos(a)           ,d;
        0     ,0            ,0             ,1;];
    
    dh=zeroErrorChecker(dh,a,o);


end

