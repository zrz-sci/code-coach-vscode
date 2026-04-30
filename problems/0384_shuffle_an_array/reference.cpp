/*
 * 384. Shuffle an Array
 * 难度: Medium
 * 标签: Design, Array, Math, Randomized
 *
 * 【题目本质】
 * 实现数组的等概率随机打乱（洗牌），并支持重置到原始状态。
 *
 * 【解法总览】
 * 解法1: Fisher-Yates 洗牌（从后往前）
 * 解法2: Fisher-Yates 洗牌（从前往后）
 */

// ===================== Solution 1: Fisher-Yates（从后往前）=====================
// 思路：从最后一个位置开始，每次与 [0, i] 范围内随机位置交换
// 时间：reset O(n), shuffle O(n)
// 空间：O(n)（保存原始数组）
class Solution1 {
    vector<int> original;
    vector<int> arr;
public:
    Solution1(vector<int>& nums) : original(nums), arr(nums) {}

    vector<int> reset() {
        arr = original;
        return arr;
    }

    vector<int> shuffle() {
        for (int i = arr.size() - 1; i > 0; i--) {
            int j = rand() % (i + 1);  // 随机选 [0, i]
            swap(arr[i], arr[j]);
        }
        return arr;
    }
};

// ===================== Solution 2: Fisher-Yates（从前往后）=====================
// 思路：从第一个位置开始，每次与 [i, n-1] 范围内随机位置交换
// 时间：reset O(n), shuffle O(n)
// 空间：O(n)
class Solution2 {
    vector<int> original;
    vector<int> arr;
public:
    Solution2(vector<int>& nums) : original(nums), arr(nums) {}

    vector<int> reset() {
        arr = original;
        return arr;
    }

    vector<int> shuffle() {
        int n = arr.size();
        for (int i = 0; i < n; i++) {
            int j = i + rand() % (n - i);  // 随机选 [i, n-1]
            swap(arr[i], arr[j]);
        }
        return arr;
    }
};

/*
 * 【解法对比】
 * | 解法 | 方向 | 随机范围 | 效果 |
 * |------|------|---------|------|
 * | 从后往前 | i: n-1→1 | [0, i] | 标准 Fisher-Yates |
 * | 从前往后 | i: 0→n-1 | [i, n-1] | 等价变体 |
 * 两者数学上等价，都保证 n! 种排列等概率。
 *
 * 【易错点】
 * 1. 随机范围不能是 [0, n-1]，必须逐步缩小
 * 2. 必须深拷贝保存原始数组用于 reset
 * 3. shuffle 在 arr 上操作，不能破坏 original
 * 4. rand() % (i+1) 中别忘了 +1
 *
 * 【面试追问】
 * Q1: 为什么 rand()%n 不等概率 → n^n 不整除 n!
 * Q2: 部分洗牌（只取前k个）→ 只做k步 O(k)
 * Q3: 如何测试等概率性 → 卡方检验 / 大量采样统计频率
 */
