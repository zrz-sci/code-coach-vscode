/*
 * LeetCode 923: 三数之和的多种可能 (3Sum With Multiplicity)
 *
 * 【题目本质】
 * 统计满足 arr[i]+arr[j]+arr[k]==target (i<j<k) 的三元组数量。
 * 关键约束: arr[i] 范围仅 [0,100]，利用值域小的特点，
 * 用计数数组+组合数学替代暴力枚举下标。
 *
 * 【解法总览】
 * 解法1: 值域计数 + 组合数学 — O(V^2) / O(V) — 利用值域小
 * 解法2: 排序 + 双指针 — O(n^2) / O(1) — 通用解法
 */

// ============================================================
// 解法1: 值域计数 + 组合数学
// 时间: O(V^2) 其中 V=101  空间: O(V)
//
// 【思路】
// 1. 统计每个值出现次数 count[v]
// 2. 枚举 a <= b <= c 使得 a+b+c = target
// 3. 根据 a,b,c 是否相等，用不同组合公式:
//    - 三个都不同: count[a] * count[b] * count[c]
//    - a==b!=c:    C(count[a],2) * count[c]
//    - a!=b==c:    count[a] * C(count[b],2)
//    - a==b==c:    C(count[a],3)
//
// 枚举范围: a: [0,100], b: [a,100], c = target-a-b
// 最多 101*101/2 ≈ 5000 次循环，极快。
//
// 示意:
// arr = [1,1,2,2,3,3,4,4,5,5], target=8
// count = {1:2, 2:2, 3:2, 4:2, 5:2}
//
// (1,2,5): 不同 → 2*2*2 = 8
// (1,3,4): 不同 → 2*2*2 = 8
// (2,2,4): a==b → C(2,2)*2 = 1*2 = 2
// (2,3,3): b==c → 2*C(2,2) = 2*1 = 2
// 总计 = 20 ✓
// ============================================================
class Solution1 {
public:
    int threeSumMulti(vector<int>& arr, int target) {
        const int MOD = 1e9 + 7;
        long long count[101] = {};  // count[v] = 值 v 出现的次数

        // 统计频率
        for (int x : arr) {
            count[x]++;
        }

        long long result = 0;

        // 枚举值三元组 (a, b, c)，保证 a <= b <= c
        for (int a = 0; a <= 100; a++) {
            for (int b = a; b <= 100; b++) {
                int c = target - a - b;

                // 合法性检查: c 必须 >= b 且在值域内
                if (c < b || c > 100) continue;

                // 至少每个值都出现过才有贡献
                if (count[a] == 0 || count[b] == 0 || count[c] == 0) continue;

                if (a == b && b == c) {
                    // 三个值全相同: 从 count[a] 个里选 3 个
                    // C(n, 3) = n * (n-1) * (n-2) / 6
                    result += count[a] * (count[a] - 1) * (count[a] - 2) / 6;
                } else if (a == b) {
                    // a==b 但 !=c: 从 count[a] 里选 2 个，再乘 count[c]
                    // C(n, 2) = n * (n-1) / 2
                    result += count[a] * (count[a] - 1) / 2 * count[c];
                } else if (b == c) {
                    // b==c 但 a!=b: count[a] 乘以从 count[b] 里选 2 个
                    result += count[a] * count[b] * (count[b] - 1) / 2;
                } else {
                    // 三个值全不同: 直接相乘
                    result += count[a] * count[b] * count[c];
                }

                result %= MOD;
            }
        }

        return (int)result;
    }
};

// ============================================================
// 解法2: 排序 + 双指针
// 时间: O(n^2)  空间: O(1) (不计排序)
//
// 【思路】
// 排序后，固定 arr[i]，在 [i+1, n-1] 上用双指针找两数之和。
// 核心难点: 当 arr[left]+arr[right]==remain 时，
// 需要正确统计连续重复元素的组合数。
//
// 两种情况:
// 1. arr[left] == arr[right]: 区间内全相同，选 2 个 = C(len, 2)
// 2. arr[left] != arr[right]: 左连续 leftCount 个，右连续 rightCount 个
//    → 贡献 = leftCount * rightCount
//
// 示意 (arr sorted = [1,1,2,2,3,3,4,4,5,5], target=8):
//
// i=0, arr[i]=1, remain=7
//   left=1(1), right=9(5): 1+5=6 < 7 → left++
//   left=2(2), right=9(5): 2+5=7 ✓
//     leftCount=2(两个2), rightCount=2(两个5) → +4
//   left=4(3), right=7(4): 3+4=7 ✓
//     leftCount=2, rightCount=2 → +4
//   left=6, right=5: 结束
// i=1, arr[i]=1, remain=7 (同样贡献+4+4)
// ... 最终加起来 = 20 ✓
// ============================================================
class Solution2 {
public:
    int threeSumMulti(vector<int>& arr, int target) {
        const int MOD = 1e9 + 7;
        sort(arr.begin(), arr.end());
        long long result = 0;
        int n = arr.size();

        for (int i = 0; i < n - 2; i++) {
            int remain = target - arr[i];
            int left = i + 1, right = n - 1;

            while (left < right) {
                int sum = arr[left] + arr[right];

                if (sum < remain) {
                    left++;
                } else if (sum > remain) {
                    right--;
                } else {
                    // 找到匹配: arr[left] + arr[right] == remain
                    if (arr[left] == arr[right]) {
                        // 区间 [left, right] 内所有值相同
                        // 从 len 个相同元素中选 2 个 = C(len, 2)
                        long long len = right - left + 1;
                        result = (result + len * (len - 1) / 2) % MOD;
                        break;  // 这个 i 的所有配对都处理完了
                    } else {
                        // 统计左端连续相同值的个数
                        int leftCount = 1;
                        while (left + leftCount < right
                               && arr[left + leftCount] == arr[left]) {
                            leftCount++;
                        }

                        // 统计右端连续相同值的个数
                        int rightCount = 1;
                        while (right - rightCount > left
                               && arr[right - rightCount] == arr[right]) {
                            rightCount++;
                        }

                        // 左边选一个 * 右边选一个
                        result = (result + (long long)leftCount * rightCount) % MOD;

                        // 跳过所有重复值
                        left += leftCount;
                        right -= rightCount;
                    }
                }
            }
        }

        return (int)result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法              | 时间        | 空间    | 适用场景              |
// |-------------------|-------------|---------|----------------------|
// | 值域计数+组合数学 | O(V^2)≈10^4 | O(V)   | 值域小 (arr[i]<=100) |
// | 排序+双指针       | O(n^2)≈9*10^6| O(1)  | 通用                 |
//
// 当值域小时，解法1 远优于解法2。面试时应识别约束并选择最优。
//
// 【易错点】
// 1. 组合数公式写错:
//    ✗ C(n,2) = n*n/2 (应该是 n*(n-1)/2)
//    ✗ C(n,3) = n*n*n/6 (应该是 n*(n-1)*(n-2)/6)
//
// 2. int 溢出:
//    ✗ 用 int 做 count[a]*count[b]*count[c]，最大 3000^3 溢出
//    ✓ 用 long long 计算，每步取模
//
// 3. 值域边界漏检:
//    ✗ 不检查 c > 100 → 数组越界
//    ✓ c = target-a-b, 必须 b <= c <= 100
//
// 4. 双指针法中 arr[left]==arr[right] 特殊情况:
//    ✗ 当所有值相同时，leftCount/rightCount 统计出错
//    ✓ 单独处理: 区间长度 len，选 2 个 = C(len, 2)
//
// 【面试追问】
// Q1: 如果值域是 [0, 10^9]，值域计数还能用吗？
//    （不能直接用，可用 HashMap 但最坏退化到 O(n^2)，不如排序+双指针）
// Q2: 为什么要保证 a<=b<=c 枚举？
//    （避免重复计数 — 同一组值的 6 种排列只算一次）
// Q3: 和经典 3Sum (LC15) 的区别？
//    （LC15 去重返回值组合，本题计数下标三元组含重复值）
// ============================================================
