// ============================================================
// 575. Distribute Candies
// 难度: Easy | 标签: Array, Hash Table
// ============================================================

// ============================================================
// 解法一: HashSet 去重 (推荐)
// 思路: 种类数 = set 大小，答案 = min(种类数, n/2)
// 时间: O(n) | 空间: O(n)
// ============================================================
class Solution {
public:
    int distributeCandies(vector<int>& candyType) {
        unordered_set<int> types(candyType.begin(), candyType.end());
        return min(types.size(), candyType.size() / 2);
    }
};

// ============================================================
// 解法二: 排序 + 连续去重计数
// 思路: 排序后相邻不同元素即为一个新种类
// 时间: O(n log n) | 空间: O(1)
// 适用: 对空间要求严格，且允许修改原数组
// ============================================================
// class Solution {
// public:
//     int distributeCandies(vector<int>& candyType) {
//         sort(candyType.begin(), candyType.end());
//         int types = 1;
//         for (int i = 1; i < (int)candyType.size(); ++i) {
//             if (candyType[i] != candyType[i - 1]) {
//                 ++types;
//             }
//         }
//         return min(types, (int)candyType.size() / 2);
//     }
// };

// ============================================================
// 解法三: bitset (值域有限时的常数空间优化)
// 思路: 利用 candyType[i] 范围 [-10^5, 10^5]，用 bitset 替代 set
// 时间: O(n) | 空间: O(1) (固定 ~25KB bitset)
// ============================================================
// class Solution {
// public:
//     int distributeCandies(vector<int>& candyType) {
//         bitset<200001> seen;
//         int types = 0;
//         for (int c : candyType) {
//             if (!seen[c + 100000]) {
//                 seen.set(c + 100000);
//                 ++types;
//             }
//         }
//         return min(types, (int)candyType.size() / 2);
//     }
// };

// ============================================================
// 关键洞察:
// 1. 答案 = min(种类数, n/2)，与每种糖的个数无关
// 2. 贪心: 每种只吃一颗就能最大化种类数
// 3. HashSet 去重是最自然的实现方式
// 4. 注意 size() 返回 size_t (unsigned)，min 比较时类型需一致
// ============================================================
