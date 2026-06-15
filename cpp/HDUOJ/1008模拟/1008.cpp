#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std;
struct node
{
    double f;
    double j;
    double s;
};
bool cmp(node a,node b)
{
    return a.s>b.s;
}
int main()
{
    double m;
    int n;
    double sum;
    node nodes[1005];
    while(~scanf("%lf%d",&m,&n))
    {
        if(m==-1&&n==-1)
        break;
        sum=0;
        for(int i=0;i<n;i++)
        {
            scanf("%lf%lf",&nodes[i].j,&nodes[i].f);
            nodes[i].s=(double)nodes[i].j/nodes[i].f;
        }
        sort(nodes,nodes+n,cmp);
        for(int k=0;k<n;k++)
        {
            if(m>=nodes[k].f)
            {
               sum=sum+nodes[k].j;
               m=m-nodes[k].f;
            }
            else
            {
                sum=sum+m*nodes[k].s;
                m=0;
                break;
            }
        }
        printf("%.3lf\n",sum);
    }
}