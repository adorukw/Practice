#include <cstdio>
#include <cstring>

int main() {
    int n;
    scanf("%d", &n);
    char a[1000], b[1000], c[1001];
    for (int i = 0; i < n; i++) {
        scanf("%s %s", a, b);
        int carry = 0;
        int j = strlen(a) - 1, k = strlen(b) - 1, len = 0;
        while (j >= 0 || k >= 0 || carry > 0) {
            int sum = carry;
            if (j >= 0) sum += a[j] - '0';
            if (k >= 0) sum += b[k] - '0';
            carry = sum / 10;
            c[len] = sum % 10 + '0';
            j--;
            k--;
            len++;
        }
        c[len] = '\0';
        for (int l = 0; l < len / 2; l++) {
            char temp = c[l];
            c[l] = c[len - l - 1];
            c[len - l - 1] = temp;
        }
        printf("Case %d:\n %s + %s = %s\n", i + 1, a, b, c);
    }
}
