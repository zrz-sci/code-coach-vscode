// LeetCode 777: Swap Adjacent in LR String
// 难度: Medium | 标签: Two Pointers, String
//
// 解法一：双指针法（推荐）
// 时间 O(n)  空间 O(1)
//
// 核心思路：
// 1. L 只能向左移动（XL -> LX），R 只能向右移动（RX -> XR）
// 2. L 和 R 的相对顺序不变
// 3. 用双指针跳过 X，逐一比对每个 L/R 的字符和位置约束

class Solution {
public:
    bool canTransform(string start, string result) {
        int n = start.size();
        if (n != (int)result.size()) return false;

        // 快速检查：去掉 X 后的序列必须相同
        // 同时用双指针做，省去额外空间
        int i = 0, j = 0;
        while (i < n || j < n) {
            // 跳过 start 中的 X
            while (i < n && start[i] == 'X') i++;
            // 跳过 result 中的 X
            while (j < n && result[j] == 'X') j++;

            // 两个都到末尾 -> 完全匹配
            if (i == n && j == n) return true;

            // 一个到末尾另一个没有 -> 不匹配
            if (i == n || j == n) return false;

            // 对应的字符必须相同（L 对 L，R 对 R）
            if (start[i] != result[j]) return false;

            // 位置约束检查
            // L 只能向左移动：start 中的位置 >= result 中的位置
            if (start[i] == 'L' && i < j) return false;

            // R 只能向右移动：start 中的位置 <= result 中的位置
            if (start[i] == 'R' && i > j) return false;

            i++;
            j++;
        }
        return true;
    }
};

// =============================================================
// 解法二：提取位置法
// 时间 O(n)  空间 O(n)
//
// 先把所有非 X 字符和对应下标提取出来，再逐一比对。
// 代码更清晰，适合面试时先写这个版本再优化。

class Solution2 {
public:
    bool canTransform(string start, string result) {
        int n = start.size();
        // 提取 (字符, 原始下标)
        vector<pair<char, int>> sPairs, rPairs;
        for (int i = 0; i < n; i++) {
            if (start[i] != 'X') sPairs.push_back({start[i], i});
        }
        for (int i = 0; i < n; i++) {
            if (result[i] != 'X') rPairs.push_back({result[i], i});
        }

        // 非 X 字符数量必须相同
        if (sPairs.size() != rPairs.size()) return false;

        for (int k = 0; k < (int)sPairs.size(); k++) {
            char sc = sPairs[k].first,  rc = rPairs[k].first;
            int  si = sPairs[k].second, ri = rPairs[k].second;

            // 字符必须相同
            if (sc != rc) return false;

            // L 只能左移: start 位置 >= result 位置
            if (sc == 'L' && si < ri) return false;

            // R 只能右移: start 位置 <= result 位置
            if (sc == 'R' && si > ri) return false;
        }
        return true;
    }
};

// =============================================================
// 解法三：替换 + 检查法（简洁但不推荐面试用）
// 时间 O(n)  空间 O(n)
//
// 利用 erase 或 replace 去掉 X 做字符串比较，
// 然后再用双指针检查位置约束。
// 这是两步法，代码简洁但多遍扫描。

class Solution3 {
public:
    bool canTransform(string start, string result) {
        int n = start.size();

        // 步骤 1：去掉 X 后序列必须相同
        string sNoX, rNoX;
        for (char c : start)  if (c != 'X') sNoX += c;
        for (char c : result) if (c != 'X') rNoX += c;
        if (sNoX != rNoX) return false;

        // 步骤 2：双指针检查位置约束
        int j = 0;
        for (int i = 0; i < n; i++) {
            if (start[i] == 'X') continue;
            // 找到 result 中下一个非 X 字符
            while (j < n && result[j] == 'X') j++;

            // 位置约束
            if (start[i] == 'L' && i < j) return false;
            if (start[i] == 'R' && i > j) return false;
            j++;
        }
        return true;
    }
};
