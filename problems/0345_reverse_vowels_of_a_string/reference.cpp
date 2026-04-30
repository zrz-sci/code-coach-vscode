/*
 * 【题目本质】
 * 在字符串中，只反转元音字母的位置，其他字符保持不动。
 * 典型的对撞双指针问题，类似于"反转特定类型元素"的模板。
 *
 * 【解法总览】
 * 解法1: 对撞双指针          O(n) / O(1) ⭐推荐
 * 解法2: 提取元音 + 反转回填  O(n) / O(n)  更直观
 */

// ===================== Solution 1: 对撞双指针 =====================
// 思路：左右指针各自跳过非元音，遇到元音时交换
// 关键：元音包含大小写共 10 个字符
// 时间 O(n)，空间 O(1)
class Solution1 {
public:
    bool isVowel(char c) {
        // 用 switch 比 find 更快，O(1) 分支跳转
        switch (c) {
            case 'a': case 'e': case 'i': case 'o': case 'u':
            case 'A': case 'E': case 'I': case 'O': case 'U':
                return true;
            default:
                return false;
        }
    }

    string reverseVowels(string s) {
        int left = 0, right = (int)s.size() - 1;

        while (left < right) {
            // 跳过非元音
            while (left < right && !isVowel(s[left])) left++;
            while (left < right && !isVowel(s[right])) right--;

            if (left < right) {
                swap(s[left], s[right]);
                left++;
                right--;
            }
        }

        return s;
    }
};

// ===================== Solution 2: 提取 + 反转回填 =====================
// 思路：先收集所有元音字符并反转顺序，再依次填回原位置
// 更直观但需要额外空间
// 时间 O(n)，空间 O(n)
class Solution2 {
public:
    string reverseVowels(string s) {
        // 定义元音集合，用 unordered_set 保证 O(1) 查找
        unordered_set<char> vowelSet = {'a','e','i','o','u','A','E','I','O','U'};

        // 提取所有元音
        vector<char> vowels;
        for (char c : s) {
            if (vowelSet.count(c)) {
                vowels.push_back(c);
            }
        }

        // 从后往前填回元音（等价于反转）
        int idx = (int)vowels.size() - 1;
        for (int i = 0; i < (int)s.size(); i++) {
            if (vowelSet.count(s[i])) {
                s[i] = vowels[idx--];
            }
        }

        return s;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 | 劣势 |
 * |------|------|------|------|------|
 * | Solution1 | O(n) | O(1) | 原地操作，空间最优 | 需要注意双 while 边界 |
 * | Solution2 | O(n) | O(n) | 逻辑清晰，不易出错 | 额外空间 |
 *
 * 【易错点】
 * 1. 元音字母必须包含大写：A, E, I, O, U，共 10 个
 * 2. 内层 while 循环必须加 left < right 条件，否则可能越界
 * 3. 交换后 left++ 和 right-- 不能遗漏，否则死循环
 * 4. 空字符串和单字符字符串无需特殊处理，while 条件自动兜底
 *
 * 【面试追问】
 * Q1: 如果反转辅音？ → 条件取反：跳过元音，交换辅音
 * Q2: 如果字符串 immutable？ → 转为 char 数组操作，或用提取回填法
 * Q3: 能否用栈实现？ → 可以：第一遍入栈元音，第二遍遇到元音时出栈填入
 */
