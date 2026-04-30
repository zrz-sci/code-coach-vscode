// LeetCode 756: Pyramid Transition Matrix
// 解法：回溯 + 哈希映射
// 时间复杂度：O(6^(n*(n-1)/2))  空间复杂度：O(n^2)

// ===================== 解法一：回溯 + 哈希映射（推荐） =====================

class Solution {
public:
    // 映射：底部两字符 -> 可选的顶部字符列表
    unordered_map<string, vector<char>> mapping;

    bool pyramidTransition(string bottom, vector<string>& allowed) {
        // 步骤1：构建映射表
        for (const string& s : allowed) {
            string key = s.substr(0, 2);  // 底部两字符
            mapping[key].push_back(s[2]);  // 顶部字符
        }

        // 步骤2：从底层开始递归构建
        return build(bottom);
    }

    // 递归构建：给定当前层，尝试构建上一层
    bool build(const string& bottom) {
        // 终止条件：只剩一个字符，金字塔构建成功
        if (bottom.size() == 1) return true;

        // 先检查每对相邻字符是否都有可选的顶部字符（剪枝）
        for (int i = 0; i + 1 < (int)bottom.size(); i++) {
            string key = bottom.substr(i, 2);
            if (mapping.find(key) == mapping.end()) return false;
        }

        // 回溯构建上一层
        string nextRow;
        return backtrack(bottom, nextRow, 0);
    }

    // 回溯：逐位确定上层字符
    // bottom: 当前层字符串
    // nextRow: 正在构建的上层字符串
    // index: 当前处理到 bottom 的第 index 对相邻字符
    bool backtrack(const string& bottom, string& nextRow, int index) {
        // 上层构建完成，递归处理新的一层
        if (index == (int)bottom.size() - 1) {
            return build(nextRow);
        }

        // 取相邻两字符作为 key
        string key = bottom.substr(index, 2);

        // 遍历所有可选的顶部字符
        for (char c : mapping[key]) {
            nextRow.push_back(c);           // 选择
            if (backtrack(bottom, nextRow, index + 1)) {
                return true;                 // 找到可行方案
            }
            nextRow.pop_back();              // 回溯撤销
        }

        return false;  // 所有选择都失败
    }
};


// ===================== 解法二：位运算优化回溯 =====================
// 用 6 位掩码表示可选字符集合，加速查表

class Solution2 {
public:
    // mapping[a][b] 是一个位掩码，表示字符 'A'+a 和 'A'+b 上方可以放哪些字符
    int mapping[6][6];

    bool pyramidTransition(string bottom, vector<string>& allowed) {
        // 初始化映射
        memset(mapping, 0, sizeof(mapping));
        for (const string& s : allowed) {
            int a = s[0] - 'A';
            int b = s[1] - 'A';
            int c = s[2] - 'A';
            mapping[a][b] |= (1 << c);  // 设置对应位
        }

        return build(bottom);
    }

    bool build(const string& bottom) {
        if (bottom.size() == 1) return true;

        // 剪枝：检查每对相邻字符是否有可选顶部字符
        for (int i = 0; i + 1 < (int)bottom.size(); i++) {
            if (mapping[bottom[i] - 'A'][bottom[i + 1] - 'A'] == 0) {
                return false;
            }
        }

        string nextRow;
        return backtrack(bottom, nextRow, 0);
    }

    bool backtrack(const string& bottom, string& nextRow, int index) {
        if (index == (int)bottom.size() - 1) {
            return build(nextRow);
        }

        int mask = mapping[bottom[index] - 'A'][bottom[index + 1] - 'A'];

        // 遍历掩码中所有为1的位
        for (int bit = 0; bit < 6; bit++) {
            if (mask & (1 << bit)) {
                nextRow.push_back('A' + bit);  // 选择字符 'A'+bit
                if (backtrack(bottom, nextRow, index + 1)) {
                    return true;
                }
                nextRow.pop_back();             // 回溯
            }
        }

        return false;
    }
};


// ===================== 关键细节说明 =====================
//
// 1. 映射构建：allowed 中 "ABC" 表示 A(左) B(右) 上放 C
//    key = "AB", value 列表追加 'C'
//
// 2. 回溯框架：
//    - 对当前层从左到右逐位确定上层字符
//    - 每确定一位就递归确定下一位
//    - 上层构建完成后，以上层为新的底层递归
//
// 3. 剪枝策略：
//    - 在回溯前先检查每对相邻字符是否有映射
//    - 若任何一对没有可选项，直接返回 false
//
// 4. 字符集约束：只有 A-F 共6个字符，底层最长6个
//    总搜索空间虽为指数级，但实际规模可控
//
// 5. 位运算优化：
//    - 6个字符用6位掩码表示
//    - 查表和判空操作均为 O(1)
//    - 遍历可选字符只需检查6个位
