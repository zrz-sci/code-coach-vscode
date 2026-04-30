// 659. Split Array into Consecutive Subsequences
// 难度: Medium
// 标签: Greedy, Array, Hash Table, Heap (Priority Queue)

// =====================================================
// 解法一：贪心 + 双哈希表（推荐）
// 时间 O(n)  空间 O(n)
// =====================================================
// 核心：对每个数字 x，优先追加到已有子序列（need[x]>0），
// 否则尝试新建 [x, x+1, x+2]，都不行则 false。
//
// freq: 每个数字的剩余可用次数
// need: "以 x-1 结尾的子序列期望 x 来续接" 的个数

class Solution {
public:
    bool isPossible(vector<int>& nums) {
        unordered_map<int, int> freq, need;

        // 第一遍：统计每个数字的出现频次
        for (int x : nums) {
            freq[x]++;
        }

        // 第二遍：贪心分配
        for (int x : nums) {
            if (freq[x] == 0) {
                // 已经被之前的开新序列操作消耗了，跳过
                continue;
            }

            if (need[x] > 0) {
                // 优先追加到以 x-1 结尾的已有子序列
                need[x]--;
                need[x + 1]++;
            } else if (freq[x + 1] > 0 && freq[x + 2] > 0) {
                // 开新序列 [x, x+1, x+2]
                freq[x + 1]--;
                freq[x + 2]--;
                need[x + 3]++;   // 新序列末尾是 x+2，期望 x+3 续接
            } else {
                // 既不能追加，也不能开新序列
                return false;
            }

            freq[x]--;  // 无论哪种方式，x 都被消耗了一个
        }

        return true;
    }
};

// =====================================================
// 解法二：贪心 + 最小堆
// 时间 O(n log n)  空间 O(n)
// =====================================================
// 对每个数字 x，优先延长末尾为 x-1 的最短子序列；
// 如果没有这样的子序列，就新开一个长度为 1 的子序列。
// 最后检查所有子序列长度是否 >= 3。

class Solution2 {
public:
    bool isPossible(vector<int>& nums) {
        // key: 子序列末尾值
        // value: 最小堆，存放所有以该 key 结尾的子序列的长度
        unordered_map<int, priority_queue<int, vector<int>, greater<int>>> chains;

        for (int x : nums) {
            int prevLen = 0;
            if (chains.count(x - 1) && !chains[x - 1].empty()) {
                // 取出以 x-1 结尾的最短子序列来延长
                prevLen = chains[x - 1].top();
                chains[x - 1].pop();
                // 清理空堆（可选，但有助于减少内存）
                if (chains[x - 1].empty()) {
                    chains.erase(x - 1);
                }
            }
            // 追加 x 后放入 chains[x]
            chains[x].push(prevLen + 1);
        }

        // 检查所有子序列长度是否 >= 3
        for (auto& [endVal, pq] : chains) {
            while (!pq.empty()) {
                if (pq.top() < 3) return false;
                pq.pop();
            }
        }

        return true;
    }
};

// =====================================================
// 运行示例与解释
// =====================================================
// 示例 1: nums = [1,2,3,3,4,5]
//   x=1: need[1]=0, 开新序列 [1,2,3], freq: {1:0,2:0,3:0,4:1,5:1}, need={4:1}
//   x=2: freq[2]=0, 跳过
//   x=3: freq[3]=0, 跳过  (注意：第一个3已用于新序列)
//   x=3: need[3]=0, 开新序列 [3,4,5], freq: {4:0,5:0}, need={4:0,6:1}
//   x=4: freq[4]=0, 跳过
//   x=5: freq[5]=0, 跳过
//   返回 true（两个子序列 [1,2,3] 和 [3,4,5]）
//
// 示例 3: nums = [1,2,3,4,4,5]
//   x=1: 开新序列 [1,2,3], freq: {1:0,2:0,3:0,4:2,5:1}, need={4:1}
//   x=2: 跳过
//   x=3: 跳过
//   x=4: need[4]=1, 追加到 [1,2,3,4], need={4:0,5:1}
//   x=4: need[4]=0, 尝试新序列需要 freq[5]>0 && freq[6]>0, freq[6]=0
//        失败 -> 返回 false
