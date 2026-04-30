// ============================================================
// 624. Maximum Distance in Arrays (数组列表中的最大距离)
// 难度: Medium | 标签: Greedy, Array
// ============================================================
// 题意: m 个升序数组, 从不同数组各取一个数, 使绝对差最大
// ============================================================

// ============================================================
// 解法一: 贪心 + 一次遍历 (推荐)
// ============================================================
// 思路: 维护已遍历数组中的全局 min 和 max
//       对每个新数组, 先用其极值与全局极值计算距离, 再更新全局极值
//       "先计算后更新" 保证两数来自不同数组
// 时间: O(m) | 空间: O(1)
// ============================================================

class Solution {
public:
    int maxDistance(vector<vector<int>>& arrays) {
        int result = 0;
        // 用第一个数组初始化
        int globalMin = arrays[0].front();
        int globalMax = arrays[0].back();

        for (int i = 1; i < (int)arrays.size(); i++) {
            int curMin = arrays[i].front();  // 升序, 首元素最小
            int curMax = arrays[i].back();   // 升序, 末元素最大

            // 计算两个方向的最大距离
            // 方向1: 当前数组的最大值 - 之前数组中的最小值
            result = max(result, curMax - globalMin);
            // 方向2: 之前数组中的最大值 - 当前数组的最小值
            result = max(result, globalMax - curMin);

            // 先计算再更新, 保证不同数组约束
            globalMin = min(globalMin, curMin);
            globalMax = max(globalMax, curMax);
        }

        return result;
    }
};

// ============================================================
// 解法二: 暴力枚举所有数组对 (超时, 仅作理解用)
// ============================================================
// 思路: O(m^2) 枚举所有 (i, j) 数组对, 计算 |min_i - max_j|
// 时间: O(m^2) | 空间: O(1)
// m 可达 10^5, 会 TLE
// ============================================================

/*
class Solution {
public:
    int maxDistance(vector<vector<int>>& arrays) {
        int result = 0;
        int m = arrays.size();
        for (int i = 0; i < m; i++) {
            for (int j = i + 1; j < m; j++) {
                int d1 = abs(arrays[i].front() - arrays[j].back());
                int d2 = abs(arrays[i].back()  - arrays[j].front());
                result = max(result, max(d1, d2));
            }
        }
        return result;
    }
};
*/

// ============================================================
// 关键要点:
// 1. 排序数组: 首元素 = min, 末元素 = max, O(1) 获取极值
// 2. 不同数组约束: "先计算后更新" 的遍历顺序天然保证
// 3. 两个方向都要检查: curMax-globalMin 和 globalMax-curMin
// 4. 类似 LC 121 买卖股票 -- 维护历史极值求最大差
// ============================================================
