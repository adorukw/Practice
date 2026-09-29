// ===================== 输入输出 =====================
#include <cstdio>    // 竞赛首选高效IO：scanf(格式化读), printf(格式化写), getchar(读单个字符), putchar(写单个字符)
#include <iostream>  // C++标准IO流：cin(读), cout(写), endl(换行，竞赛中慎用，效率低)
#include <iomanip>   // 输出格式控制：setprecision(设置精度), setw(设置宽度), fixed(固定小数位), left/right(对齐方式)

// ===================== 字符串处理 =====================
#include <string>    // C++字符串类：size()/length()(长度), substr(截取子串), find(查找字符/子串), 
                     // replace(替换), append(追加), c_str(转C风格字符串), empty(判空)
#include <cstring>   // C风格字符串：memset(内存初始化，如memset(a,0,sizeof(a))), memcpy(内存拷贝), 
                     // strlen(长度), strcmp(比较), strcpy(拷贝), strcat(拼接)

// ===================== 容器（STL核心） =====================
#include <vector>    // 动态数组（最常用）：push_back(尾插), pop_back(尾删), size(大小), empty(判空), 
                     // clear(清空), []随机访问, begin/end(迭代器), resize(调整大小)
#include <list>      // 双向链表：push_back/push_front(头尾插), pop_back/pop_front(头尾删), size/empty
#include <deque>     // 双端队列：兼顾vector和list特点，[]访问，push/pop front/back(头尾操作)
#include <queue>     // 队列/优先队列：
                     // queue：push(尾插), pop(头删), front(队首), back(队尾), empty/size
                     // priority_queue（优先队列/堆）：push(插入), pop(删堆顶), top(堆顶), empty/size
#include <stack>     // 栈：push(入栈), pop(出栈), top(栈顶), empty(判空), size(大小)
#include <map>       // 有序键值对（红黑树）：insert(插入), find(查找键), erase(删除), []访问值, 
                     // size/empty, begin/end（按键升序排列，键唯一）
#include <set>       // 有序集合（红黑树）：insert(插入), find(查找), erase(删除), size/empty, 
                     // begin/end（按值升序排列，元素唯一）
#include <unordered_map> // 无序哈希表：方法同map，查找/插入O(1)（比map快，无排序，键唯一）
#include <unordered_set>  // 无序哈希集合：方法同set，查找/插入O(1)（比set快，无排序，元素唯一）

// ===================== 算法 =====================
#include <algorithm> // 竞赛核心算法库：
                     // sort(排序), reverse(反转), max_element/min_element(找最大/小值), 
                     // lower_bound/upper_bound(二分查找), swap(交换), find(查找元素), count(计数),
                     // next_permutation(下一个排列), fill(填充)

// ===================== 数值与数学 =====================
#include <cmath>     // 数学函数：abs(整型绝对值), fabs(浮点绝对值), sqrt(开方), pow(幂运算), 
                     // ceil(向上取整), floor(向下取整), round(四舍五入), sin/cos/tan(三角函数)
#include <cstdlib>   // 通用工具：rand(生成随机数), srand(设置随机种子), atoi(字符串转int), 
                     // atof(字符串转double), exit(终止程序)
#include <numeric>   // 数值计算：accumulate(区间累加，求总和), partial_sum(前缀和)

// ===================== 时间与随机数 =====================
#include <ctime>     // 时间相关：time(获取时间戳，用于srand种子), clock(计算程序运行时间)
#include <random>    // 现代随机数（比rand更优）：mt19937(随机数引擎), 
                     // uniform_int_distribution(整数均匀分布), uniform_real_distribution(浮点均匀分布)

// ===================== 其他工具 =====================
#include <utility>   // 实用工具：pair(键值对，如pair<int,string>), make_pair(创建pair), swap(交换pair)
#include <functional>// 函数对象：greater<>(降序排序，如sort(v.begin(),v.end(),greater<int>())), less<>()
#include <limits>    // 数值极限：INT_MAX/INT_MIN(整型最值), LLONG_MAX/LLONG_MIN(长整型最值), 
                     // DBL_MAX(浮点最值)（避免手动写死最值）
#include <typeinfo>  // 类型信息：typeid(获取变量类型名称，调试用)
#include <fstream>   // 文件流：ifstream/ofstream(读写文件), fstream(读写字符串)
#include <sstream>   // 字符串流：stringstream(读写字符串)，如，stringstream ss; ss<<a<<b; int x=stoi(ss.str());