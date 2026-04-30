/*
 * 520. Detect Capital
 * 难度: Easy | 标签: String
 *
 * 【题目本质】
 * 判断单词大写用法是否合法：全大写 / 全小写 / 仅首字母大写。
 * 核心：统计大写字母个数，一行判断。
 *
 * 【解法总览】
 * Solution1: 计数法             — O(n) 时间, O(1) 空间 ⭐⭐⭐
 * Solution2: 直接判断三种模式   — O(n) 时间, O(1) 空间 ⭐⭐
 */

// ===================== Solution1: 计数法 =====================
// cnt == 0: 全小写
// cnt == len: 全大写
// cnt == 1 && isupper(word[0]): 仅首字母大写
class Solution1 {
public:
    bool detectCapitalUse(string word) {
        int cnt = 0;
        for (char c : word) {
            if (isupper(c)) cnt++;
        }
        return cnt == 0 || cnt == (int)word.size() || (cnt == 1 && isupper(word[0]));
    }
};

// ===================== Solution2: 直接判断三种模式 =====================
class Solution2 {
public:
    bool detectCapitalUse(string word) {
        // 全大写
        if (all_of(word.begin(), word.end(), ::isupper)) return true;
        // 全小写
        if (all_of(word.begin(), word.end(), ::islower)) return true;
        // 首字母大写 + 其余小写
        if (isupper(word[0]) && all_of(word.begin() + 1, word.end(), ::islower)) return true;
        return false;
    }
};

/*
 * 【解法对比】
 * 计数法: 一次遍历，逻辑紧凑
 * 三模式判断: 语义更清晰，但可能遍历多次
 *
 * 【易错点】
 * 1. cnt==1 时必须检查 isupper(word[0])，"mL" 不合法
 * 2. 单字符无论大小写都合法
 * 3. 别忘了 (int)word.size() 的类型转换
 *
 * 【面试追问】
 * Q1: 正则解法？→ ^[A-Z]+$|^[a-z]+$|^[A-Z][a-z]+$
 * Q2: 增加驼峰模式？→ 需要额外逻辑检测大小写交替规律
 * Q3: 能否优于 O(n)？→ 不能，至少遍历一次
 */
