// 解法一：前缀最大值（最优解）
// 时间 O(n)，空间 O(1)
// 核心思路：数组是 [0,n-1] 的排列，max(arr[0..i]) == i 时可以分块
// 因为前 i+1 个元素的最大值为 i，由鸽巢原理它们必然恰好是 {0,1,...,i}
class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int n = arr.size();
        int curMax = 0;
        int chunks = 0;

        for (int i = 0; i < n; ++i) {
            // 维护前缀最大值
            curMax = max(curMax, arr[i]);

            // 前缀最大值等于当前索引
            // 说明 arr[0..i] 恰好包含 {0, 1, ..., i}
            // 可以在这里切一刀形成一个独立块
            if (curMax == i) {
                ++chunks;
            }
        }

        return chunks;
    }
};

// 解法二：单调栈（通用方法，与 768 题一致）
// 时间 O(n)，空间 O(n)
// 核心思路：栈中存每个块的最大值，保持单调递增
// 遇到更小元素时合并前面的块
class Solution2 {
public:
    int maxChunksToSorted(vector<int>& arr) {
        stack<int> stk;

        for (int num : arr) {
            if (stk.empty() || num >= stk.top()) {
                // 当前元素 >= 栈顶，独立成新块
                stk.push(num);
            } else {
                // 当前元素 < 栈顶，需要合并块
                // 记住合并区域的最大值（第一个弹出的栈顶）
                int curMax = stk.top();
                stk.pop();

                // 继续弹出所有大于 num 的栈顶（合并更多块）
                while (!stk.empty() && stk.top() > num) {
                    stk.pop();
                }

                // 合并后的块以最大值为代表压回栈
                stk.push(curMax);
            }
        }

        // 栈大小就是最大分块数
        return stk.size();
    }
};

// 解法三：前缀 max + 后缀 min
// 时间 O(n)，空间 O(n)
// 核心思路：位置 i 是分割点 iff max(arr[0..i]) <= min(arr[i+1..n-1])
class Solution3 {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int n = arr.size();

        // 前缀最大值
        vector<int> prefixMax(n);
        prefixMax[0] = arr[0];
        for (int i = 1; i < n; ++i) {
            prefixMax[i] = max(prefixMax[i - 1], arr[i]);
        }

        // 后缀最小值
        vector<int> suffixMin(n);
        suffixMin[n - 1] = arr[n - 1];
        for (int i = n - 2; i >= 0; --i) {
            suffixMin[i] = min(suffixMin[i + 1], arr[i]);
        }

        // 统计分割点：prefixMax[i] <= suffixMin[i+1] 时可以切一刀
        int chunks = 1;  // 最后位置天然是分割点
        for (int i = 0; i < n - 1; ++i) {
            if (prefixMax[i] <= suffixMin[i + 1]) {
                ++chunks;
            }
        }

        return chunks;
    }
};

// 解法四：排序对比法（利用排列的前缀和特性）
// 时间 O(n)，空间 O(1)
// 核心思路：排列中前缀和相等 iff 前缀元素集合相等
// 目标数组 [0,1,...,n-1] 的前缀和为 0, 1, 3, 6, 10, ...
class Solution4 {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int n = arr.size();
        int sumArr = 0;
        int sumTarget = 0;
        int chunks = 0;

        for (int i = 0; i < n; ++i) {
            sumArr += arr[i];
            sumTarget += i;  // 目标数组的前缀和 = 0 + 1 + ... + i

            // 对于排列，前缀和相等 <=> 前缀元素集合相等
            // 因为没有重复元素，和相等意味着集合相等
            if (sumArr == sumTarget) {
                ++chunks;
            }
        }

        return chunks;
    }
};

// 解法五：位运算验证法（异或 + max 双重验证）
// 时间 O(n)，空间 O(1)
// 核心思路：利用异或的性质作为辅助验证
// arr[0]^...^arr[i] == 0^1^...^i 是必要条件（但不充分）
// 结合 max == i 使用可以增强正确性
class Solution5 {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int n = arr.size();
        int curMax = 0;
        int xorArr = 0;
        int xorTarget = 0;
        int chunks = 0;

        for (int i = 0; i < n; ++i) {
            curMax = max(curMax, arr[i]);
            xorArr ^= arr[i];
            xorTarget ^= i;

            // 双重验证：max 条件 + 异或条件
            // 对于排列，单独 max == i 已经充分
            // 加上异或验证只是额外保障
            if (curMax == i && xorArr == xorTarget) {
                ++chunks;
            }
        }

        return chunks;
    }
};
