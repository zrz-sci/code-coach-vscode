// 702. Search in a Sorted Array of Unknown Size
// 难度: Medium
// 标签: Array, Binary Search, Interactive
//
// 思路: 指数扩展(Exponential Search) + 标准二分查找
// 阶段一: 从 right=1 开始，不断翻倍直到 reader.get(right) >= target
// 阶段二: 在 [right/2, right] 范围内进行标准二分查找
//
// 时间复杂度: O(log n) -- 扩展 O(log n) + 二分 O(log n)
// 空间复杂度: O(1)

// ============================================================
// 解法一: 指数扩展 + 二分查找（推荐面试解法）
// ============================================================
class Solution {
public:
    int search(const ArrayReader& reader, int target) {
        // 阶段一: 确定搜索范围的右边界
        // 从 right=1 开始，不断翻倍，直到 reader.get(right) >= target
        int right = 1;
        while (reader.get(right) < target) {
            right *= 2;  // 指数增长: 1, 2, 4, 8, 16, ...
        }

        // 此时 target 一定在 [right/2, right] 区间内（如果存在的话）
        int left = right / 2;

        // 阶段二: 在 [left, right] 范围内标准二分查找
        while (left <= right) {
            int mid = left + (right - left) / 2;
            int val = reader.get(mid);

            if (val == target) {
                return mid;
            } else if (val < target) {
                left = mid + 1;
            } else {
                // val > target: 包括越界返回的 2^31 - 1
                right = mid - 1;
            }
        }

        return -1;  // target 不存在
    }
};

// ============================================================
// 解法二: 更精细的边界处理版本
// ============================================================
// 在扩展时同步推进 left，使搜索范围更紧致
class Solution_V2 {
public:
    int search(const ArrayReader& reader, int target) {
        // 特判: 检查第一个元素
        if (reader.get(0) == target) return 0;

        // 指数扩展确定右边界，同时推进左边界
        int left = 0, right = 1;
        while (reader.get(right) < target) {
            left = right;       // 更新左边界为当前 right（该位置值 < target）
            right <<= 1;        // right *= 2，位运算更高效
        }

        // 二分查找
        while (left <= right) {
            int mid = left + ((right - left) >> 1);
            int val = reader.get(mid);

            if (val == target) {
                return mid;
            } else if (val < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        return -1;
    }
};

// ============================================================
// 解法三: 使用固定上界的简化版（利用约束条件）
// ============================================================
// 题目约束 secret.length <= 10^4，所以可以直接设 right = 10000
// 但这种写法不够通用，面试中不推荐
class Solution_V3 {
public:
    int search(const ArrayReader& reader, int target) {
        int left = 0, right = 10000;  // 利用题目约束

        while (left <= right) {
            int mid = left + (right - left) / 2;
            int val = reader.get(mid);

            if (val == target) {
                return mid;
            } else if (val < target) {
                left = mid + 1;
            } else {
                // val > target 或 val == INT_MAX (越界)
                right = mid - 1;
            }
        }

        return -1;
    }
};

// ============================================================
// 关键点总结:
// 1. 指数扩展的核心: right 从 1 开始翻倍，O(log n) 步找到上界
// 2. 越界返回值 2^31 - 1 天然大于所有合法值，不影响二分逻辑
// 3. left 的初始值: right/2，保证不遗漏
// 4. 解法二更精细: left 跟随 right 一起推进，搜索范围更紧致
// 5. 解法三利用约束但不通用，面试中优先展示解法一
// ============================================================
