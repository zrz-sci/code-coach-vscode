// 解法一：单调栈（最优解）
// 时间 O(n)，空间 O(n)
// 核心思路：栈中每个元素代表一个块的最大值，保持单调递增
// 遇到更小的元素时，合并前面的块（弹栈），保留合并后的最大值
class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
        // 单调递增栈，存储每个块的最大值
        stack<int> stk;

        for (int num : arr) {
            if (stk.empty() || num >= stk.top()) {
                // 当前元素 >= 栈顶，可以独立成新块
                stk.push(num);
            } else {
                // 当前元素 < 栈顶，需要合并块
                // 保留当前合并区域的最大值（即原始栈顶）
                int curMax = stk.top();
                stk.pop();

                // 继续弹出所有大于 num 的栈顶
                // 这些块都需要与当前块合并
                while (!stk.empty() && stk.top() > num) {
                    stk.pop();
                }

                // 将合并后的块（以其最大值为代表）压回栈
                stk.push(curMax);
            }
        }

        // 栈的大小就是最大分块数
        return stk.size();
    }
};

// 解法二：前缀最大值 + 后缀最小值
// 时间 O(n)，空间 O(n)
// 核心思路：位置 i 是合法分割点 iff max(arr[0..i]) <= min(arr[i+1..n-1])
class Solution2 {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int n = arr.size();

        // 前缀最大值：prefixMax[i] = max(arr[0], arr[1], ..., arr[i])
        vector<int> prefixMax(n);
        prefixMax[0] = arr[0];
        for (int i = 1; i < n; ++i) {
            prefixMax[i] = max(prefixMax[i - 1], arr[i]);
        }

        // 后缀最小值：suffixMin[i] = min(arr[i], arr[i+1], ..., arr[n-1])
        vector<int> suffixMin(n);
        suffixMin[n - 1] = arr[n - 1];
        for (int i = n - 2; i >= 0; --i) {
            suffixMin[i] = min(suffixMin[i + 1], arr[i]);
        }

        // 统计分割点数量
        // 位置 i（0-indexed）是分割点 iff prefixMax[i] <= suffixMin[i+1]
        // 最后一个位置 n-1 天然是分割点
        int chunks = 1;  // 最后一个位置始终是分割点
        for (int i = 0; i < n - 1; ++i) {
            if (prefixMax[i] <= suffixMin[i + 1]) {
                ++chunks;
            }
        }

        return chunks;
    }
};

// 解法三：排序对比法（计数映射）
// 时间 O(n log n)，空间 O(n)
// 核心思路：原数组前 i 个元素的多重集合 == 排序数组前 i 个元素时可以分块
// 用差异映射跟踪两个前缀的元素差异
class Solution3 {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int n = arr.size();

        // 排序得到目标数组
        vector<int> sorted_arr = arr;
        sort(sorted_arr.begin(), sorted_arr.end());

        // 用 map 跟踪差异：+1 表示原数组多出的元素，-1 表示排序数组多出的
        // nonZero 记录 map 中非零条目的数量
        // 当 nonZero == 0 时，前缀的元素集合完全匹配，可以分块
        unordered_map<int, int> diff;
        int nonZero = 0;
        int chunks = 0;

        for (int i = 0; i < n; ++i) {
            // 原数组第 i 个元素贡献 +1
            ++diff[arr[i]];
            if (diff[arr[i]] == 1) ++nonZero;       // 从 0 变为非零
            else if (diff[arr[i]] == 0) --nonZero;   // 从非零变为 0

            // 排序数组第 i 个元素贡献 -1
            --diff[sorted_arr[i]];
            if (diff[sorted_arr[i]] == -1) ++nonZero; // 从 0 变为非零
            else if (diff[sorted_arr[i]] == 0) --nonZero; // 从非零变为 0

            // 差异为零说明前缀元素的多重集合匹配
            if (nonZero == 0) {
                ++chunks;
            }
        }

        return chunks;
    }
};

// 解法四：排序对比法（有序 map 变体）
// 时间 O(n log n)，空间 O(n)
// 核心思路：与解法三相同，使用 ordered map 便于调试
class Solution4 {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int n = arr.size();
        vector<int> sorted_arr = arr;
        sort(sorted_arr.begin(), sorted_arr.end());

        // 使用 map 动态维护原数组和排序数组的前缀元素差异
        map<int, int> count;
        int chunks = 0;
        int balance = 0;  // 非零计数的条目数

        for (int i = 0; i < n; ++i) {
            // 原数组元素加入（+1）
            if (++count[arr[i]] == 0) --balance;
            else if (count[arr[i]] == 1) ++balance;

            // 排序数组元素移出（-1）
            if (--count[sorted_arr[i]] == 0) --balance;
            else if (count[sorted_arr[i]] == -1) ++balance;

            // balance == 0 表示前缀元素集合完全匹配
            if (balance == 0) ++chunks;
        }

        return chunks;
    }
};
