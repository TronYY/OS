JOBNUM=10;
JOBIF(:,1)=poissrnd(5,JOBNUM,1); %��ҵ��ʼʱ�� �˺������ɷ��Ӳ���(Poisson)�ֲ���JOBNUM��1�����ݡ����ɷֲ��Ĳ�����6
JOBIF(:,2)=ceil(exprnd(5,JOBNUM,1))%��ҵִ��ʱ��
JOBIF(:,3)=ceil(exprnd(10000,JOBNUM,1))%��ҵ�����ڴ棬���ɷ��Ӳ���Ϊ5��ָ���ֲ������ݾ���,ceil��������ȡ��