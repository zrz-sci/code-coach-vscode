/*
 * 【题目本质】
 * 判断字符串能否重排为回文串。
 * 回文性质：最多 1 个字符出现奇数次（放中间），其余必须偶数次。
 *
 * 【解法总览】
 * 解法1: 哈希表计数 - O(n) 时间 O(1) 空间
 * 解法2: 位运算 XOR - O(n) 时间 O(1) 空间 ⭐ 最优雅
 * 解法3: Set 翻转 - O(n) 时间 O(1) 空间
 * 解法4: 数组计数 - O(n) 时间 O(1) 空间（最快）
 */

// ============================================================
// 解法1: 哈希表计数
// 思路：统计每个字符频率，数奇数次字符个数
// 时间 O(n)，空间 O(26) = O(1)
// ============================================================
class Solution1 {
public:
    bool canPermutePalindrome(string s) {
        unordered_map<char, int> freq;
        for (char c : s) freq[c]++;

        int oddCount = 0;
        for (auto& [ch, cnt] : freq) {
            if (cnt % 2 != 0) oddCount++;
        }
        return oddCount <= 1;
    }
};

// ============================================================
// 解法2: 位运算 XOR ⭐ 推荐
// 思路：用 26 bit 表示每个字母的奇偶状态
//       XOR 翻转：偶数次 -> 0，奇数次 -> 1
//       最终最多 1 个 bit 为 1
// 时间 O(n)，空间 O(1)
// ============================================================
class Solution2 {
public:
    bool canPermutePalindrome(string s) {
        int mask = 0;
        for (char c : s) {
            mask ^= (1 << (c - 'a')); // 遇到就翻转对应 bit
        }
        // popcount <= 1 表示最多一个奇数次字符
        return __builtin_popcount(mask) <= 1;
        // 另一种写法：(mask & (mask - 1)) == 0
        // 解释：mask 最多只有 1 个 bit 为 1 时满足此条件
    }
};

// ============================================================
// 解法3: Set 翻转
// 思路：set 里有就删掉（配对成功），没有就加入
//       最终 set 里剩余的是落单的字符
// 时间 O(n)，空间 O(26) = O(1)
// ============================================================
class Solution3 {
public:
    bool canPermutePalindrome(string s) {
        unordered_set<char> st;
        for (char c : s) {
            if (st.count(c)) {
                st.erase(c); // 配对成功，删除
            } else {
                st.insert(c); // 落单，加入
            }
        }
        return st.size() <= 1;
    }
};

// ============================================================
// 解法4: 数组计数（最快）
// 思路：26 大小数组直接计数，避免哈希开销
// 时间 O(n)，空间 O(1)
// ============================================================
class Solution4 {
public:
    bool canPermutePalindrome(string s) {
        int freq[26] = {};
        for (char c : s) freq[c - 'a']++;

        int oddCount = 0;
        for (int i = 0; i < 26; i++) {
            if (freq[i] & 1) oddCount++;
        }
        return oddCount <= 1;
    }
};

/*
 * 【解法对比】
 * | 解法     | 时间  | 空间  | 特点              |
 * |---------|------|------|------------------|
 * | 哈希表   | O(n) | O(1) | 最直观             |
 * | 位运算   | O(n) | O(1) | 最优雅，一个 int    |
 * | Set     | O(n) | O(1) | 配对思维           |
 * | 数组     | O(n) | O(1) | 最快（无哈希开销）  |
 *
 * 【易错点】
 * 1. 判断 oddCount == 0 而非 <= 1
 *    -> 奇数长度字符串允许 1 个奇数次字符
 * 2. 位运算用 OR 而非 XOR
 *    -> OR 只会把 bit 设为 1，无法反映偶数次（应回到 0）
 * 3. mask & (mask-1) == 0 忘记 mask 可以为 0
 *    -> mask=0 表示所有字符偶数次，也是合法的
 *
 * 【面试追问】
 * Q: 位运算中 (mask & (mask-1)) == 0 为什么能判断最多 1 个 bit?
 * A: mask & (mask-1) 的效果是去掉最低位的 1。
 *    如果只有 0 或 1 个 bit 为 1，去掉后变成 0。
 *    如果有 2+ 个 bit 为 1，去掉一个后还不为 0。
 */
