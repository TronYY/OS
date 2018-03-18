JOBNUM=10;
JOBIF(:,1)=poissrnd(5,JOBNUM,1); %作业开始时间 此函数生成服从泊松(Poisson)分布的JOBNUM行1列数据。泊松分布的参数是6
JOBIF(:,2)=ceil(exprnd(5,JOBNUM,1))%作业执行时间
JOBIF(:,3)=ceil(exprnd(10000,JOBNUM,1))%作业所需内存，生成服从参数为5的指数分布的数据矩阵,ceil向正无穷取整