#include <cstdio>

int main() {
    int T;
    scanf("%d", &T);
    
    for (int caseNum = 1; caseNum <= T; caseNum++) {
        int N;
        scanf("%d", &N);
        
        int arr[N];
        for (int i = 0; i < N; i++) {
            scanf("%d", &arr[i]);
        }
        

        int currMax = arr[0];
        int maxSum = arr[0];
        int start = 0; 
        int end = 0; 
        int tempStart = 0; 
        

        for (int i = 1; i < N; i++) {
            if (arr[i] > currMax + arr[i]) {
                currMax = arr[i];
                tempStart = i;
            } else {
                currMax = currMax + arr[i];
            }
            
            if (currMax > maxSum) {
                maxSum = currMax;
                start = tempStart;
                end = i;
            }
        }
        
        if (caseNum > 1) printf("\n");
        printf("Case %d:\n", caseNum);
        printf("%d %d %d\n", maxSum, start + 1, end + 1);
    }
    
    return 0;
}