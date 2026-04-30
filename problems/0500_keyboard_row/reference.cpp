/*
 * 500. Keyboard Row
 *
 * 【题目本质】
 * 字符分类问题：判断单词所有字母是否属于键盘同一行
 * 核心操作是字母→行号映射 + 一致性检查
 *
 * 【解法总览】
 * Solution1: 数组映射法 — O(N·L) / O(1)  ⭐推荐
 * Solution2: Set 集合法 — O(N·L) / O(1)
 */

// ===================== Solution 1: 数组映射 =====================
// 思路：预计算26个字母的行号，检查每个单词所有字母行号一致
// 时间: O(N·L)  空间: O(1)  N=单词数, L=平均单词长度

class Solution1 {
public:
    vector<string> findWords(vector<string>& words) {
        // row[i] = 字母 'a'+i 所在行号 (0/1/2)
        //          a b c d e f g h i j k l m n o p q r s t u v w x y z
        int row[26]={1,2,2,1,0,1,1,1,0,1,1,1,2,2,0,0,0,0,1,0,0,2,0,2,0,2};

        vector<string> res;
        for (auto& word : words) {
            int target = row[tolower(word[0]) - 'a'];
            bool ok = true;
            for (char c : word) {
                if (row[tolower(c) - 'a'] != target) { ok = false; break; }
            }
            if (ok) res.push_back(word);
        }
        return res;
    }
};

// ===================== Solution 2: Set 集合 =====================
// 思路：三行字母分别建 set，判断单词字母是否全部属于某一个 set
// 时间: O(N·L)  空间: O(1)

class Solution2 {
public:
    vector<string> findWords(vector<string>& words) {
        vector<unordered_set<char>> rows = {
            {'q','w','e','r','t','y','u','i','o','p'},
            {'a','s','d','f','g','h','j','k','l'},
            {'z','x','c','v','b','n','m'}
        };

        vector<string> res;
        for (auto& word : words) {
            int r = -1;
            for (int i = 0; i < 3; i++) {
                if (rows[i].count(tolower(word[0]))) { r = i; break; }
            }
            bool ok = true;
            for (char c : word) {
                if (!rows[r].count(tolower(c))) { ok = false; break; }
            }
            if (ok) res.push_back(word);
        }
        return res;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优劣 |
 * |------|------|------|------|
 * | Solution1 数组映射 | O(N·L) | O(1) | 常数最小，直接索引 |
 * | Solution2 Set集合 | O(N·L) | O(1) | 更直观，便于扩展自定义布局 |
 *
 * 【易错点】
 * 1. 大小写处理 → 必须 tolower() 统一转换后再查映射
 * 2. 行号映射数组写错 → 逐字母校验 qwertyuiop=0, asdfghjkl=1, zxcvbnm=2
 * 3. 返回原始单词 → 不要返回转换后的小写版本
 *
 * 【面试追问】
 * Q1: 位运算优化？每行一个26-bit掩码，单词字母OR后 & 行掩码 == 单词掩码即可
 * Q2: 自定义键盘布局？将行字符串参数化，动态构建映射
 * Q3: 如果要返回行号而非筛选？改返回类型为 vector<pair<string,int>>
 */
