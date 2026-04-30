/*
 * 【题目本质】
 * 判断一个数字字符串旋转180度后是否与原数相同。
 * 本质是双指针对称匹配：左右字符必须满足特定的旋转映射关系。
 * 有效映射: 0<->0, 1<->1, 6<->9, 8<->8, 9<->6
 *
 * 【解法总览】
 * 解法1 (推荐): 双指针 + HashMap — O(n) / O(1)
 * 解法2: 构建翻转字符串比较 — O(n) / O(n)
 */

// ===================== 解法1: 双指针 + HashMap (推荐) =====================
// 思路: 建立旋转映射表, 双指针从两端向中间检查
class Solution1 {
public:
    bool isStrobogrammatic(string num) {
        // 旋转180度的映射关系
        unordered_map<char, char> mp = {
            {'0', '0'}, {'1', '1'}, {'6', '9'}, {'8', '8'}, {'9', '6'}
        };

        int l = 0, r = num.size() - 1;
        while (l <= r) {
            // 检查当前字符是否有有效的旋转映射
            if (mp.find(num[l]) == mp.end() || mp[num[l]] != num[r]) {
                return false;
            }
            l++;
            r--;
        }
        return true;
    }
};

// ===================== 解法2: 构建翻转字符串比较 =====================
// 思路: 将每个字符映射为旋转后的字符, 整体反转后与原串比较
class Solution2 {
public:
    bool isStrobogrammatic(string num) {
        unordered_map<char, char> mp = {
            {'0', '0'}, {'1', '1'}, {'6', '9'}, {'8', '8'}, {'9', '6'}
        };

        string rotated;
        for (char c : num) {
            if (mp.find(c) == mp.end()) return false;
            rotated += mp[c];
        }
        // 旋转180度 = 每个字符映射 + 整体反转
        reverse(rotated.begin(), rotated.end());
        return rotated == num;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | 双指针+Map | O(n) | O(1) | 最优, 原地判断 |
 * | 翻转比较 | O(n) | O(n) | 逻辑直观易理解 |
 *
 * 【易错点】
 * 1. ✗ while(l < r) → ✓ while(l <= r)
 *    奇数长度时中间字符也必须是自对称的 (0/1/8)
 * 2. ✗ 只判断 num[l] == num[r] → ✓ 判断 mp[num[l]] == num[r]
 *    6和9不相等, 但互为旋转映射
 * 3. ✗ 遗漏无效字符检查 → ✓ 2,3,4,5,7 不在映射表中, 直接返回false
 *
 * 【面试追问】
 * Q1: 如何扩展到生成所有n位中心对称数? → 递归由内向外构建 (LC 247)
 * Q2: 能否用数组替代HashMap? → char mp[10], 无效位置设为 '\0'
 * Q3: 如何统计[low, high]范围内的中心对称数个数? → 枚举长度+边界判断 (LC 248)
 */
