// LeetCode 760: Find Anagram Mappings
// 解法：哈希表
// 时间复杂度：O(n)  空间复杂度：O(n)

// ===================== 解法一：简单哈希表（推荐） =====================
// 适用于题目允许重复映射到同一索引的情况

class Solution {
public:
    vector<int> anagramMappings(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();

        // 步骤1：建立 nums2 中值到索引的映射
        // 如果有重复值，后出现的索引会覆盖先出现的
        unordered_map<int, int> indexMap;
        for (int j = 0; j < (int)nums2.size(); j++) {
            indexMap[nums2[j]] = j;
        }

        // 步骤2：查表构建映射结果
        vector<int> result(n);
        for (int i = 0; i < n; i++) {
            result[i] = indexMap[nums1[i]];
        }

        return result;
    }
};


// ===================== 解法二：哈希表 + 队列（严格一一映射） =====================
// 保证每个 nums2 的索引最多被使用一次

class Solution2 {
public:
    vector<int> anagramMappings(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();

        // 步骤1：建立 nums2 中值到索引队列的映射
        // 同一值的多个索引按顺序存入队列
        unordered_map<int, queue<int>> indexMap;
        for (int j = 0; j < (int)nums2.size(); j++) {
            indexMap[nums2[j]].push(j);
        }

        // 步骤2：查表构建映射结果，每次取一个索引后弹出
        vector<int> result(n);
        for (int i = 0; i < n; i++) {
            result[i] = indexMap[nums1[i]].front();  // 取队首索引
            indexMap[nums1[i]].pop();                  // 弹出，避免重复使用
        }

        return result;
    }
};


// ===================== 解法三：排序配对法 =====================
// 不使用哈希表，用排序 + 配对的方式建立映射

class Solution3 {
public:
    vector<int> anagramMappings(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();

        // 步骤1：创建带原始索引的数组对
        vector<pair<int, int>> sorted1(n), sorted2(n);
        for (int i = 0; i < n; i++) {
            sorted1[i] = {nums1[i], i};
            sorted2[i] = {nums2[i], i};
        }

        // 步骤2：按值排序
        sort(sorted1.begin(), sorted1.end());
        sort(sorted2.begin(), sorted2.end());

        // 步骤3：排序后值相同的元素会对齐
        // sorted1[k].first == sorted2[k].first
        // 建立映射：nums1 原始索引 -> nums2 原始索引
        vector<int> result(n);
        for (int k = 0; k < n; k++) {
            int origIdx1 = sorted1[k].second;  // nums1 中的原始位置
            int origIdx2 = sorted2[k].second;  // nums2 中的原始位置
            result[origIdx1] = origIdx2;
        }

        return result;
    }
};


// ===================== 解法四：暴力搜索（最朴素） =====================

class Solution4 {
public:
    vector<int> anagramMappings(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        vector<int> result(n);
        vector<bool> used(n, false);  // 标记 nums2 中已使用的索引

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (!used[j] && nums1[i] == nums2[j]) {
                    result[i] = j;
                    used[j] = true;  // 标记为已使用
                    break;
                }
            }
        }

        return result;
    }
};


// ===================== 关键细节说明 =====================
//
// 1. 题目条件：nums2 一定是 nums1 的变位词
//    - 两数组长度相同
//    - 包含完全相同的元素（含重复次数）
//    - 因此不需要处理"找不到"的边界情况
//
// 2. 重复元素的处理：
//    - 简单哈希表：同一值只存最后一个索引，多个相同值映射到同一位置
//      例: nums1=[1,1], nums2=[1,1] -> 可能返回 [1,1]
//    - 队列方案：每个值的所有索引按序存储，保证一一对应
//      例: nums1=[1,1], nums2=[1,1] -> 返回 [0,1]
//    - 题目明确说 "any of them" 即可，所以两种都正确
//
// 3. 排序法的思路：
//    - 把 (值, 原始索引) 配对后排序
//    - 排序后两数组在相同位置有相同值
//    - 通过原始索引建立映射关系
//    - 时间 O(n log n)，比哈希表略慢但不依赖哈希函数
//
// 4. 面试中推荐解法一（简单哈希表）：
//    - 代码最简洁，一眼看懂
//    - 时间最优 O(n)
//    - 面试官如果追问重复元素，再展示解法二
