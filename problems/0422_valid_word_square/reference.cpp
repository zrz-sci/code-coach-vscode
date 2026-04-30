// 【题目本质】
// 判断字符串数组是否关于主对角线对称：words[i][j] == words[j][i] 对所有有效位置成立。
// 核心难点在于字符串长度不等时的边界检查。

// 【解法总览】
// Solution1: 逐元素对称性检查 O(n*m) / O(1) ⭐推荐
// Solution2: 构造列字符串比较 O(n*m) / O(m)

// ===================== Solution1: 逐元素对称性检查 =====================
// 思路：遍历每个 (i,j)，检查 words[j][i] 是否存在且相等
class Solution1 {
public:
    bool validWordSquare(vector<string>& words) {
        int n = words.size();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < (int)words[i].size(); j++) {
                // 列索引超出行数，或行索引超出该行长度
                if (j >= n || i >= (int)words[j].size())
                    return false;
                if (words[i][j] != words[j][i])
                    return false;
            }
        }
        return true;
    }
};

// ===================== Solution2: 构造列字符串比较 =====================
// 思路：提取每一列构成字符串，与对应行比较
class Solution2 {
public:
    bool validWordSquare(vector<string>& words) {
        int n = words.size();
        for (int j = 0; j < n; j++) {
            string col;
            for (int i = 0; i < n; i++) {
                if (j < (int)words[i].size())
                    col += words[i][j];
                // 如果 j >= words[i].size()，该位置没有字符
                // 但后续行可能仍有字符 → 不能 break
            }
            // 列字符串可能末尾被截断（没有尾随空格的概念）
            if (col != words[j]) return false;
        }
        return true;
    }
};

// 【解法对比】
// | 解法 | 时间 | 空间 | 特点 |
// |------|------|------|------|
// | Solution1 | O(n*m) | O(1) | 最简洁，推荐 |
// | Solution2 | O(n*m) | O(m) | 需要额外空间存列字符串 |

// 【易错点】
// 1. 忘记检查 j >= n：当 words[i] 的长度大于行数时，对称位置不存在
// 2. 忘记检查 i >= words[j].size()：当对称位置的行太短时，字符不存在
// 3. size() 返回 size_t（无符号），与 int 比较可能有问题 → 显式 (int) 转换
// 4. Solution2 中不能在 j >= words[i].size() 时 break，因为后续行可能更长

// 【面试追问】
// Q1: 如果矩阵很大但很稀疏，有优化方法吗？
// A1: 可以只遍历非空位置，用哈希表存储稀疏矩阵，但一般不需要。
//
// Q2: 这题和转置矩阵有什么区别？
// A2: Word Square 要求矩阵关于主对角线对称（即矩阵等于自身转置），
//     但这里的"矩阵"是不规则的（行长度不同），所以需要边界检查。
//
// Q3: 如果要求修改数组使其成为 Valid Word Square，最少改几个字符？
// A3: 这是一个更难的问题，需要枚举不对称位置并决定修改哪一边。
