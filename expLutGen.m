
max=50000;
k=0.0307211
vals=zeros(256);
for i=1:256
    vals(i)=exp((105+i)*k);
    fprintf("%lu,",uint16(round(vals(i))));
end

plot(1:256,vals)