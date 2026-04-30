/*
 * LeetCode 925: 长按键入 (Long Pressed Name)
 *
 * 【题目本质】
 * 验证 typed 能否由 name 通过"长按某些键"得到。
 * 即 typed 是 name 的"扩展版"，每个字符可重复但不可增删。
 *
 * 【解法总览】
 * 解法1: 双指针逐字符 — O(m+n) / O(1) — 面试首选
 * 解法2: 分组比较 — O(m+n) / O(m+n) — 思路更直观
 */

// ============================================================
// 解法1: 双指针逐字符匹配
// 时间: O(m+n)  空间: O(1)
//
// 【思路】
// i 指向 name，j 指向 typed。遍历 typed 的每个字符:
// - 如果 name[i] == typed[j]: 正常匹配，i++ j++
// - 否则如果 typed[j] == typed[j-1]: 长按重复，j++
// - 否则: 非法字符，return false
// 最后检查 i == name.size() 确保 name 全部匹配完。
//
// 过程示意:
// name = "alex", typed = "aaleex"
//         i=0            j=0
//
// j=0: name[0]='a' == typed[0]='a' → i=1,j=1
// j=1: name[1]='l' != typed[1]='a', typed[1]==typed[0]? ✓ 长按 → j=2
// j=2: name[1]='l' == typed[2]='l' → i=2,j=3
// j=3: name[2]='e' == typed[3]='e' → i=3,j=4
// j=4: name[3]='x' != typed[4]='e', typed[4]==typed[3]? ✓ 长按 → j=5
// j=5: name[3]='x' == typed[5]='x' → i=4,j=6
// j=6: 循环结束, i=4==name.size() → true ✓
//
// name = "saeed", typed = "ssaaedd"
//         i=0              j=0
//
// j=0: 's'=='s' → i=1,j=1
// j=1: 'a'!='s', typed[1]==typed[0]? ✓ 长按 → j=2
// j=2: 'a'=='a' → i=2,j=3
// j=3: 'e'!='a', typed[3]==typed[2]? ✓ 长按 → j=4
// j=4: 'e'=='e' → i=3,j=5
// j=5: 'd'=='d' → i=4,j=6
// j=6: name[4]='d' != typed[6]='d'... 等等让我重新看
//
// 实际: name="saeed", typed="ssaaedd"
// name 分组: s(1) a(1) e(2) d(1)
// typed 分组: s(2) a(2) e(1) d(2)
// e 的次数: name=2, typed=1 → typed 的 e 不够 → false ✓
// ============================================================
class Solution1 {
public:
    bool isLongPressedName(string name, string typed) {
        int i = 0, j = 0;
        int m = name.size(), n = typed.size();

        while (j < n) {
            if (i < m && name[i] == typed[j]) {
                // 正常匹配: name 的当前字符和 typed 对上了
                i++;
                j++;
            } else if (j > 0 && typed[j] == typed[j - 1]) {
                // 长按重复: typed 当前字符和前一个相同，跳过
                j++;
            } else {
                // 既不匹配 name，也不是前一个字符的长按
                // → 非法输入
                return false;
            }
        }

        // name 必须全部被匹配完
        // 如果 i < m，说明 typed 不够长，name 有字符没匹配到
        return i == m;
    }
};

// ============================================================
// 解法2: 分组比较
// 时间: O(m+n)  空间: O(m+n)
//
// 【思路】
// 将 name 和 typed 分别按连续相同字符分组:
//   "aaleex" → [('a',2), ('l',1), ('e',2), ('x',1)]
//   "alex"   → [('a',1), ('l',1), ('e',1), ('x',1)]
//
// 逐组比较:
// 1. 组数必须相同（字符种类序列一致）
// 2. 每组的字符必须相同
// 3. typed 每组的长度 >= name 对应组的长度
//    （可以长按多次，但不能少于原始次数）
//
// 这种方法把问题分解为"相同字符段"的比较，更容易理解。
//
// 示意:
// name = "alex"   → [(a,1),(l,1),(e,1),(x,1)]
// typed = "aaleex" → [(a,2),(l,1),(e,2),(x,1)]
//
// 组0: a==a, 2>=1 ✓
// 组1: l==l, 1>=1 ✓
// 组2: e==e, 2>=1 ✓
// 组3: x==x, 1>=1 ✓ → true
// ============================================================
class Solution2 {
public:
    bool isLongPressedName(string name, string typed) {
        // 辅助函数: 把字符串按连续相同字符分组
        auto getGroups = [](const string& s)
            -> vector<pair<char, int>>
        {
            vector<pair<char, int>> groups;
            int i = 0;
            while (i < (int)s.size()) {
                char c = s[i];
                int count = 0;
                while (i < (int)s.size() && s[i] == c) {
                    i++;
                    count++;
                }
                groups.push_back({c, count});
            }
            return groups;
        };

        auto nameGroups = getGroups(name);
        auto typedGroups = getGroups(typed);

        // 组数必须相同: 字符种类序列必须一致
        if (nameGroups.size() != typedGroups.size()) {
            return false;
        }

        for (int i = 0; i < (int)nameGroups.size(); i++) {
            // 字符必须相同
            if (nameGroups[i].first != typedGroups[i].first) {
                return false;
            }
            // typed 的该字符出现次数 >= name 的次数
            // (长按可以增加次数，但不能减少)
            if (typedGroups[i].second < nameGroups[i].second) {
                return false;
            }
        }

        return true;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间    | 空间    | 特点                      |
// |--------------|---------|---------|--------------------------|
// | 双指针逐字符 | O(m+n)  | O(1)   | 空间最优，代码最短        |
// | 分组比较     | O(m+n)  | O(m+n) | 逻辑更清晰，易于解释     |
//
// 【易错点】
// 1. 忘记最终检查 i == m:
//    ✗ 循环结束直接 return true → name 可能没匹配完
//    ✓ return i == m → 确保 name 全部消耗
//
// 2. 长按判断时 j=0 越界:
//    ✗ typed[j] == typed[j-1] 在 j=0 时访问 typed[-1]
//    ✓ 加 j > 0 前置条件
//
// 3. 分支优先级搞错:
//    ✗ 先检查长按再检查匹配 → 可能跳过本该匹配的字符
//    ✓ 先匹配（推进 name 指针），匹配不上再看是否长按
//
// 4. 分组比较时只比字符不比长度:
//    ✗ 字符相同就通过 → 长按次数不够的情况被放过
//    ✓ 必须同时检查 typed 的次数 >= name 的次数
//
// 【面试追问】
// Q1: typed 末尾有多余长按字符怎么办？
//    （双指针法: i 到末尾后 j 继续，多余字符必须是长按）
// Q2: 时间复杂度 O(m+n) 还是 O(n)？
//    （严格说 O(m+n)，但 n>=m 所以可简写为 O(n)）
// Q3: 正则表达式能解吗？
//    （可以生成 a+l+e+x+ 匹配，但效率不如双指针）
// ============================================================
