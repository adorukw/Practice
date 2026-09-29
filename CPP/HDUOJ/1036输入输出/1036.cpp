#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;
char str[20];
int main(void)
{
    double tim,dis;
    int pt,n,i,h,mm,ss,t1,t2;
    scanf("%d %lf",&n,&dis);
    while(~scanf("%d",&pt))
    {
        int fg=0;
        tim=0;
        for(i=0;i<n;i++)
        {
            scanf("%s",str);
            if(str[0]=='-') fg=1;
            else
            {
                h=str[0]-'0';
                mm=(str[2]-'0')*10+(str[3]-'0');
                ss=(str[5]-'0')*10+(str[6]-'0');
                tim+=h*3600+mm*60+ss;
            }
        }
        printf("%3d:",pt);
        if(fg==1)
        {
            printf(" -\n");
        }
        else
        {
            int sum=(int)(tim/dis+0.5);
            t1=sum/60,t2=sum%60;
            printf(" %d:%02d min/km\n",t1,t2);
        }
    }
    return 0;
}