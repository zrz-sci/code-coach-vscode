/*
 * LeetCode 1477: Find Two Non-overlapping Sub-arrays Each With Target Sum
 * 找两个和为目标值且不重叠的子数组
 *
 * 难度: Medium
 * 标签: Array, Hash Table, Binary Search, Dynamic Programming, Sliding Window
 *
 * 核心思路:
 *   正整数数组 → 滑动窗口找所有和为target的子数组
 *   best[i] 记录前缀中最短合法子数组 → 一遍扫描找最优组合
 *
 * 滑动窗口示意图:
 *
 *   arr = [3, 1, 1, 1, 5, 1, 2, 1],  target = 3
 *
 *   Step1: right=0, sum=3==target  → 子数组 [3], len=1
 *          [ 3 ] 1  1  1  5  1  2  1
 *           ^^^
 *          best[0]=1, 无左侧可组合
 *
 *   Step4: right=3, sum=3==target  → 子数组 [1,1,1], len=3
 *           3 [ 1  1  1 ] 5  1  2  1
 *               ^^^^^^^^
 *          best[3]=min(best[2],3)=1,  组合: 3+best[0]=1 → ans=4
 *
 *   Step7: right=6, sum=3==target  → 子数组 [1,2], len=2
 *           3  1  1  1  5 [ 1  2 ] 1
 *                          ^^^^^^
 *          best[6]=min(best[5],2)=1,  组合: 2+best[4]=1 → ans=3 ✓
 *
 *   最终: ans=3, 由 [3] + [1,2] 组成
 */

// =====================================================
// 解法1: 滑动窗口 + DP前缀最优 (推荐)
// 时间: O(n)   空间: O(n)
// =====================================================
class Solution {
public:
    int minSumOfLengths(vector<int>& arr, int target) {
        int n = arr.size();

        // best[i] = 在 arr[0..i] 范围内，和为 target 的最短子数组长度
        // 如果不存在则为 INT_MAX
        vector<int> best(n, INT_MAX);

        int ans = INT_MAX;  // 最终答案: 两个子数组长度之和的最小值
        int sum = 0;        // 当前窗口内元素之和
        int left = 0;       // 窗口左端点

        for (int right = 0; right < n; right++) {
            // 扩展窗口: 加入右端点元素
            sum += arr[right];

            // 收缩窗口: 正整数 → 和单调递增
            // 当窗口和超过target时，不断移除左端点元素
            while (sum > target) {
                sum -= arr[left];
                left++;
            }

            if (sum == target) {
                // 找到一个和为target的子数组 [left, right]
                int len = right - left + 1;

                // --- 核心: 尝试与左侧最优子数组组合 ---
                // 不重叠要求: 伙伴子数组必须在 left 之前结束
                // best[left-1] 就是 [0..left-1] 范围内的最短合法子数组
                if (left > 0 && best[left - 1] != INT_MAX) {
                    ans = min(ans, len + best[left - 1]);
                }

                // 更新 best[right]: 取"前缀已有最优"和"当前子数组"的较短者
                best[right] = (right > 0) ? min(best[right - 1], len) : len;
            } else {
                // sum < target, 当前 right 处没有合法子数组
                // 继承前缀最优值
                best[right] = (right > 0) ? best[right - 1] : INT_MAX;
            }
        }

        return ans == INT_MAX ? -1 : ans;
    }
};

/*
 * best[i] 传递机制的详细说明:
 *
 *   best 数组的核心设计: 从左到右传递"前缀中已知最短合法子数组"
 *
 *   情况A: 当前 right 处找到合法子数组 [left, right], 长度 len
 *     → best[right] = min(best[right-1], len)
 *     → 如果新子数组比之前找到的都短，更新; 否则保留旧值
 *
 *   情况B: 当前 right 处无合法子数组
 *     → best[right] = best[right-1]
 *     → 直接继承，确保信息不丢失
 *
 *   这保证了 best[i] 是单调不增的（值只会变小或不变）
 *
 *   示例: arr = [7, 3, 4, 7], target = 7
 *
 *   right=0: sum=7==target, [7] len=1
 *            best = [1, _, _, _]
 *            left=0, 无左侧 → 不组合
 *
 *   right=1: sum=3 < target
 *            best = [1, 1, _, _]   (继承)
 *
 *   right=2: sum=3+4=7==target, [3,4] len=2
 *            best = [1, 1, 1, _]   min(1, 2) = 1
 *            组合: 2 + best[0]=1 = 3 → ans=3
 *
 *   right=3: sum=7==target, [7] len=1
 *            best = [1, 1, 1, 1]   min(1, 1) = 1
 *            组合: 1 + best[2]=1 = 2 → ans=2 ✓
 */


// =====================================================
// 解法2: 前缀和 + HashMap (支持含0/负数的通用方法)
// 时间: O(n)   空间: O(n)
// =====================================================
class Solution2 {
public:
    int minSumOfLengths(vector<int>& arr, int target) {
        int n = arr.size();

        // 前缀和 → 最后出现的位置
        // prefix[j] - prefix[i] == target  意味着 arr[i+1..j] 的和为 target
        unordered_map<int, int> prefixMap;
        prefixMap[0] = -1; // 虚拟起点: 前缀和为0出现在位置-1

        // best[i] = arr[0..i] 范围内和为target的最短子数组长度
        vector<int> best(n, INT_MAX);
        int ans = INT_MAX;
        int prefix = 0;

        for (int i = 0; i < n; i++) {
            prefix += arr[i];

            // 查找: 是否存在 prefix - target 的前缀和
            // 如果存在, 子数组为 [prefixMap[prefix-target]+1, i]
            int need = prefix - target;
            if (prefixMap.count(need)) {
                int start = prefixMap[need] + 1; // 子数组起始位置
                int len = i - start + 1;         // 子数组长度

                // 尝试与左侧最优组合(不重叠)
                if (start > 0 && best[start - 1] != INT_MAX) {
                    ans = min(ans, len + best[start - 1]);
                }

                // 更新前缀最优
                best[i] = (i > 0) ? min(best[i - 1], len) : len;
            } else {
                // 当前位置无合法子数组, 继承
                best[i] = (i > 0) ? best[i - 1] : INT_MAX;
            }

            // 更新HashMap: 保留最新位置(确保找到的子数组最短)
            prefixMap[prefix] = i;
        }

        return ans == INT_MAX ? -1 : ans;
    }
};

/*
 * 两种解法对比:
 *
 * | 特性           | 滑动窗口         | 前缀和+HashMap    |
 * |----------------|------------------|--------------------|
 * | 适用场景       | 正整数数组       | 任意整数数组       |
 * | 时间复杂度     | O(n)             | O(n)               |
 * | 空间复杂度     | O(n)             | O(n)               |
 * | 实现难度       | 较简单           | 稍复杂             |
 * | HashMap开销    | 无               | 有常数因子         |
 * | 推荐场景       | 本题首选         | 通用备选           |
 *
 * 易错点:
 * 1. INT_MAX溢出: 在 len + best[left-1] 之前, 必须检查 best[left-1] != INT_MAX
 * 2. 边界 left=0: 此时没有 best[-1], 不能进行组合
 * 3. best传递: sum != target 时也要继承 best[right-1], 否则信息链断裂
 * 4. 前缀和方法中 prefixMap[prefix] = i 必须在检查之后更新, 否则可能找到长度为0的子数组
 */

// =====================================================
// 总结
// =====================================================
/*
 * 【解法对比】
 *
 * | 解法                   | 时间  | 空间  | 适用场景                   |
 * |------------------------|-------|-------|----------------------------|
 * | 滑动窗口+DP(推荐)     | O(n)  | O(n)  | 正整数数组，本题首选       |
 * | 前缀和+HashMap         | O(n)  | O(n)  | 通用，支持含0/负数         |
 *
 * 【易错点】
 * 1. INT_MAX 溢出 — len + best[left-1] 前必须检查 best[left-1] != INT_MAX，
 *    否则整数溢出导致错误结果
 * 2. best 数组的传递不能断 — 即使当前位置没有合法子数组，也要
 *    best[right] = best[right-1]，否则后续组合时丢失历史最优
 * 3. 不重叠的判定 — 组合时用 best[left-1] 而不是 best[left]，
 *    因为当前子数组从 left 开始，伙伴必须在 left 之前结束
 * 4. 前缀和 HashMap 更新时机 — 必须先查 prefixMap[need]，再更新
 *    prefixMap[prefix]=i，否则可能匹配到长度为0的"子数组"
 * 5. 返回 -1 的条件 — 不是"找不到任何合法子数组"，
 *    而是"找不到两个不重叠的合法子数组"
 *
 * 【面试追问】
 * Q1: 为什么滑动窗口只适用于正整数数组？
 *     → 滑动窗口依赖"窗口和随右扩单调递增"的性质。若含 0 或负数，
 *       扩大窗口时和可能不增，收缩窗口时和可能不减，双指针的
 *       单调性假设被破坏。此时必须用前缀和+HashMap。
 *
 * Q2: 如果要找三个不重叠子数组怎么办？
 *     → 扩展为两个 DP 数组: bestLeft[i] = [0..i] 中最短合法子数组，
 *       bestRight[i] = [i..n-1] 中最短合法子数组。
 *       枚举中间子数组 [l,r]，答案 = min(len + bestLeft[l-1] + bestRight[r+1])。
 *       时间仍为 O(n)。
 *
 * Q3: 如果数组元素可以为负数且要求子数组和恰好为 target，还能 O(n) 吗？
 *     → 可以。用前缀和+HashMap(解法2)即可处理负数。关键是
 *       prefixMap 存储最后出现位置，保证找到的子数组尽可能短。
 *       但如果 target=0 且数组全为0，需注意每个位置都是合法起点，
 *       best 数组会频繁更新为长度 1。
 */
