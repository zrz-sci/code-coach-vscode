// ============================================================
// LeetCode 599: 两个列表的最小索引总和 (Minimum Index Sum of Two Lists)
// ============================================================

// 【题目本质】
// 两个列表求交集，在交集中找索引和最小的所有字符串。
// 经典哈希表应用。

// 【解法总览】
// 解法一：哈希表（推荐） - O(n+m) 时间
// 解法二：暴力枚举 - O(n*m*L) 时间

// ============================================================
// 解法一：哈希表（推荐）
// ============================================================
class Solution {
public:
    vector<string> findRestaurant(vector<string>& list1, vector<string>& list2) {
        unordered_map<string, int> indexMap;
        for (int i = 0; i < (int)list1.size(); i++) {
            indexMap[list1[i]] = i;
        }

        vector<string> result;
        int minSum = INT_MAX;
        for (int j = 0; j < (int)list2.size(); j++) {
            if (indexMap.count(list2[j])) {
                int sum = indexMap[list2[j]] + j;
                if (sum < minSum) {
                    minSum = sum;
                    result.clear();
                    result.push_back(list2[j]);
                } else if (sum == minSum) {
                    result.push_back(list2[j]);
                }
            }
        }
        return result;
    }
};

// ============================================================
// 解法二：暴力枚举
// ============================================================
// class Solution {
// public:
//     vector<string> findRestaurant(vector<string>& list1, vector<string>& list2) {
//         vector<string> result;
//         int minSum = INT_MAX;
//         for (int i = 0; i < (int)list1.size(); i++) {
//             for (int j = 0; j < (int)list2.size(); j++) {
//                 if (list1[i] == list2[j]) {
//                     int sum = i + j;
//                     if (sum < minSum) {
//                         minSum = sum;
//                         result.clear();
//                         result.push_back(list1[i]);
//                     } else if (sum == minSum) {
//                         result.push_back(list1[i]);
//                     }
//                 }
//             }
//         }
//         return result;
//     }
// };

// 【解法对比】
// | 解法       | 时间      | 空间   | 优势               |
// |-----------|----------|--------|-------------------|
// | 哈希表 ⭐  | O(n+m)   | O(n)   | 线性时间，最优      |
// | 暴力枚举   | O(n*m*L) | O(1)   | 无额外空间，但慢    |

// 【易错点】
// 1. result 在发现更小 sum 时必须 clear() 再 push_back
// 2. minSum 初始值必须为 INT_MAX，不能为 0
// 3. 强转 list1.size() 为 int，避免 unsigned 比较陷阱

// 【面试追问】
// Q1: 列表很大 → 选短列表建 map，遍历长列表时 j > minSum 可提前 break
// Q2: 三个列表 → map<string, vector<int>> 存各列表索引
// Q3: 列表有重复 → 建 map 时只保留首次出现的索引
