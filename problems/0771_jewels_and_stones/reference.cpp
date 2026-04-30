// 771. Jewels and Stones
// 难度: Easy
// 解法: 哈希集合 + 数组标记 (两种解法)
// 时间复杂度: O(J + S)
// 空间复杂度: O(J) 哈希集合 / O(1) 数组标记

// ============================================================
// 解法一: 哈希集合 (面试推荐写法)
// ============================================================
// 将宝石类型存入 unordered_set，遍历 stones 统计命中数
class Solution {
public:
    int numJewelsInStones(string jewels, string stones) {
        unordered_set<char> jewelSet(jewels.begin(), jewels.end());

        int count = 0;
        for (char c : stones) {
            if (jewelSet.count(c)) {
                count++;
            }
        }
        return count;
    }
};

// ============================================================
// 解法二: 数组标记 (性能最优)
// ============================================================
// 用 bool 数组代替哈希表，利用 ASCII 范围有限的特点
// 常数开销极低，适合追求极致性能的场景
class Solution2 {
public:
    int numJewelsInStones(string jewels, string stones) {
        // ASCII 范围 128，覆盖所有英文字母
        bool isJewel[128] = {};

        // 标记宝石类型
        for (char c : jewels) {
            isJewel[c] = true;
        }

        // 统计石头中的宝石数量
        int count = 0;
        for (char c : stones) {
            if (isJewel[c]) {
                count++;
            }
        }
        return count;
    }
};

// ============================================================
// 解法三: 位运算标记 (极致空间优化)
// ============================================================
// 用 64 位整数的各位标记是否为宝石
// 'A'=65, 'z'=122, 范围 122-65=57 < 64, 可以用 long long
class Solution3 {
public:
    int numJewelsInStones(string jewels, string stones) {
        long long mask = 0;

        // 设置宝石类型的对应位
        for (char c : jewels) {
            mask |= (1LL << (c - 'A'));
        }

        // 检查每颗石头
        int count = 0;
        for (char c : stones) {
            if (mask & (1LL << (c - 'A'))) {
                count++;
            }
        }
        return count;
    }
};

// ============================================================
// 解法四: STL count_if 简洁写法
// ============================================================
// 利用 STL 算法一行搞定，展示 C++ 函数式风格
class Solution4 {
public:
    int numJewelsInStones(string jewels, string stones) {
        unordered_set<char> js(jewels.begin(), jewels.end());
        return count_if(stones.begin(), stones.end(),
                        [&js](char c) { return js.count(c); });
    }
};

// ============================================================
// 要点总结:
// 1. 核心是集合成员查询问题
// 2. 大小写敏感: 'a' != 'A'
// 3. 面试首选解法一(哈希集合)，清晰直观
// 4. 追求性能选解法二(数组标记)，常数开销最低
// 5. 解法三(位运算)展示位操作技巧，但可读性差
// ============================================================
