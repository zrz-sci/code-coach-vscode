// 786. K-th Smallest Prime Fraction
// 在有序质数数组中找第K小的 arr[i]/arr[j] (i < j)

// ============================================================
// 解法一: 二分答案 (推荐, 最优)
// 时间复杂度: O(n * log(max_val^2)), 空间复杂度: O(1)
// 二分分数值, 用双指针计数
// ============================================================
class Solution {
public:
    vector<int> kthSmallestPrimeFraction(vector<int>& arr, int k) {
        int n = arr.size();
        double lo = 0.0, hi = 1.0;

        // 记录答案的分子和分母
        int bestI = 0, bestJ = 1;

        // 二分迭代固定次数, 避免浮点精度问题
        for (int iter = 0; iter < 100; ++iter) {
            double mid = (lo + hi) / 2.0;

            // 统计有多少分数 <= mid
            // 同时追踪 <= mid 的最大分数
            int count = 0;
            int maxI = 0; // 最大分数的分子下标
            int maxJ = 1; // 最大分数的分母下标

            // 双指针: j 为分母下标, p 为分子下标
            int p = 0;
            for (int j = 1; j < n; ++j) {
                // 找到最大的 p 使得 arr[p] / arr[j] <= mid
                // 即 arr[p] <= mid * arr[j]
                while (p < j && (double)arr[p] <= mid * (double)arr[j]) {
                    p++;
                }
                // p 个分数 (0..p-1) 的分子满足条件
                count += p;

                // 追踪最大分数: arr[p-1] / arr[j]
                if (p > 0) {
                    // 比较 arr[p-1]/arr[j] vs arr[maxI]/arr[maxJ]
                    // 用交叉乘法避免浮点: arr[p-1]*arr[maxJ] > arr[maxI]*arr[j]
                    if ((long long)arr[p - 1] * arr[maxJ] > (long long)arr[maxI] * arr[j]) {
                        maxI = p - 1;
                        maxJ = j;
                    }
                }
            }

            if (count == k) {
                bestI = maxI;
                bestJ = maxJ;
                break;
            } else if (count < k) {
                lo = mid;
            } else {
                hi = mid;
                // count > k 时也记录一下, 因为恰好 == k 的情况
                // 可能在浮点二分中不精确命中
                bestI = maxI;
                bestJ = maxJ;
            }
        }

        return {arr[bestI], arr[bestJ]};
    }
};

// ============================================================
// 解法二: 小根堆多路归并
// 时间复杂度: O(K * log(n)), 空间复杂度: O(n)
//
// 每个分母 arr[j] 对应一条有序链:
//   arr[0]/arr[j] < arr[1]/arr[j] < ... < arr[j-1]/arr[j]
// 初始化: 把每条链的头(最小值)放入堆
// 弹出 K-1 次后, 堆顶即为第 K 小
// ============================================================
class Solution_Heap {
public:
    vector<int> kthSmallestPrimeFraction(vector<int>& arr, int k) {
        int n = arr.size();

        // 小根堆: {fraction_value, numerator_index, denominator_index}
        // 用 pair<int,int> 配合自定义比较, 避免浮点
        // 比较 arr[i1]/arr[j1] vs arr[i2]/arr[j2]
        // => arr[i1]*arr[j2] vs arr[i2]*arr[j1]
        auto cmp = [&arr](pair<int, int>& a, pair<int, int>& b) {
            return (long long)arr[a.first] * arr[b.second] >
                   (long long)arr[b.first] * arr[a.second];
        };

        priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> pq(cmp);

        // 初始化: 每条链的头部 arr[0]/arr[j]
        for (int j = 1; j < n; ++j) {
            pq.push({0, j}); // numerator_index=0, denominator_index=j
        }

        // 弹出 K-1 次
        for (int t = 0; t < k - 1; ++t) {
            auto [i, j] = pq.top();
            pq.pop();

            // 如果同一条链还有下一个元素
            if (i + 1 < j) {
                pq.push({i + 1, j});
            }
        }

        auto [i, j] = pq.top();
        return {arr[i], arr[j]};
    }
};

// ============================================================
// 解法三: 暴力排序
// 时间复杂度: O(n^2 * log(n^2)), 空间复杂度: O(n^2)
// 枚举所有分数, 排序后取第K个
// ============================================================
class Solution_Brute {
public:
    vector<int> kthSmallestPrimeFraction(vector<int>& arr, int k) {
        int n = arr.size();
        // 存储所有分数: {numerator, denominator}
        vector<pair<int, int>> fractions;

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                fractions.push_back({arr[i], arr[j]});
            }
        }

        // 按分数值排序: a/b < c/d <=> a*d < c*b
        sort(fractions.begin(), fractions.end(),
             [](const pair<int, int>& a, const pair<int, int>& b) {
                 return (long long)a.first * b.second < (long long)b.first * a.second;
             });

        return {fractions[k - 1].first, fractions[k - 1].second};
    }
};

// ============================================================
// 解法四: 二分答案 (更紧凑的写法)
// 双指针不回退, 整体 O(n) 计数
// ============================================================
class Solution_BinarySearch_V2 {
public:
    vector<int> kthSmallestPrimeFraction(vector<int>& arr, int k) {
        int n = arr.size();
        double lo = 0, hi = 1;

        while (lo < hi) {
            double mid = (lo + hi) / 2;
            int count = 0;
            int pi = 0, pj = 1; // 记录最大分数的下标

            // 对每个分母 j, 用指针 i 找满足条件的分子
            for (int j = 1, i = 0; j < n; ++j) {
                while (i < j && arr[i] <= mid * arr[j]) i++;
                count += i;
                // 更新最大分数
                if (i > 0 && (long long)arr[i - 1] * arr[pj] > (long long)arr[pi] * arr[j]) {
                    pi = i - 1;
                    pj = j;
                }
            }

            if (count == k) return {arr[pi], arr[pj]};
            if (count < k) lo = mid;
            else hi = mid;
        }

        return {}; // should not reach here
    }
};
