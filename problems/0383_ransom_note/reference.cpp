/*
 * LeetCode 383: 赎金信 (Ransom Note)
 *
 * 【题目本质】
 * 判断 ransomNote 中每个字符的出现次数是否都 ≤ magazine 中对应字符的出现次数。
 * 本质是字符频次匹配问题。
 *
 * 【解法总览】
 * 解法1: 暴力查找      — O(m×n) / O(n) — 最直觉，逐字符搜索并删除
 * 解法2: 哈希表计数    — O(m+n) / O(1) — 用 unordered_map 统计频次
 * 解法3: 数组计数      — O(m+n) / O(1) — 用 int[26] 统计频次（面试首选）
 */

// ============================================================
// 解法1: 暴力查找 — 在 magazine 中逐个找并删除
// 时间: O(m×n)  空间: O(n) (因为修改了 magazine 的副本)
//
// 【思路】
// 最直觉的想法：ransomNote 的每个字符都得在 magazine 中找到"供应"。
// 找到一个就把它"用掉"（从 magazine 中删除），找不到就说明凑不成。
//
// 瓶颈：每次 find 是 O(n)，erase 也是 O(n)，总共 O(m×n)。
// 当 m 和 n 都是 10^5 时，10^10 次操作会超时。
// ============================================================
class Solution1 {
public:
    bool canConstruct(string ransomNote, string magazine) {
        for (char c : ransomNote) {
            size_t pos = magazine.find(c);
            if (pos == string::npos) {
                return false;  // magazine 中没有（或已用完）这个字符
            }
            // 用掉这个字符：从 magazine 中删除
            magazine.erase(pos, 1);
        }
        return true;
    }
};

// ============================================================
// 解法2: 哈希表计数 — unordered_map 统计频次
// 时间: O(m+n)  空间: O(1) — 最多26个key，常数空间
//
// 【思路】
// 解法1 的瓶颈在于"查找某字符还剩几个"需要 O(n)。
// 如果先把 magazine 中所有字符的出现次数统计好（建表 O(n)），
// 之后每次查询就是 O(1)。
//
// 适用场景：字符集大小未知或不固定时（如 Unicode）。
// ============================================================
class Solution2 {
public:
    bool canConstruct(string ransomNote, string magazine) {
        unordered_map<char, int> freq;
        
        // 统计 magazine 的"供应量"
        for (char c : magazine) {
            freq[c]++;
        }
        
        // 消耗 ransomNote 的"需求量"
        for (char c : ransomNote) {
            // 先判断再减：如果当前供应量 <= 0，说明不够用
            if (freq[c] <= 0) {
                return false;
            }
            freq[c]--;
        }
        return true;
    }
};

// ============================================================
// 解法3: 数组计数 — int[26] 统计频次 ⭐ 面试首选
// 时间: O(m+n)  空间: O(1) — 固定26个int
//
// 【思路】
// 题目限定只有小写字母(a-z)，字符集大小固定为26。
// 用 int[26] 替代 unordered_map：
//   - 省去哈希计算的开销
//   - 数组连续内存，缓存友好
//   - 代码更简洁
//
// count[i] 表示字符 'a'+i 在 magazine 中还剩几个可用。
// 遍历 ransomNote 时逐个消耗，消耗到负数说明不够。
//
// 示例: ransomNote = "aa", magazine = "aab"
//
//   统计 magazine 后: count['a'-'a']=2, count['b'-'a']=1
//
//   消耗 ransomNote:
//     'a': count[0]=2 → 减为1 → ≥0 ✓
//     'a': count[0]=1 → 减为0 → ≥0 ✓
//
//   全部消耗完，return true
//
// 示例: ransomNote = "aa", magazine = "ab"
//
//   统计 magazine 后: count['a'-'a']=1, count['b'-'a']=1
//
//   消耗 ransomNote:
//     'a': count[0]=1 → 减为0 → ≥0 ✓
//     'a': count[0]=0 → 减为-1 → <0 ✗ → return false
// ============================================================
class Solution3 {
public:
    bool canConstruct(string ransomNote, string magazine) {
        // 快速剪枝：如果需求总量 > 供应总量，不可能成功
        if (ransomNote.size() > magazine.size()) {
            return false;
        }
        
        int count[26] = {0};  // 必须初始化为0！
        
        // 统计 magazine 中每个字符的供应量
        for (char c : magazine) {
            count[c - 'a']++;  // 'a'→0, 'b'→1, ..., 'z'→25
        }
        
        // 逐个消耗 ransomNote 的需求
        for (char c : ransomNote) {
            count[c - 'a']--;
            // 为什么是 < 0 而不是 <= 0？
            // 因为减完之后等于0说明"刚好用完最后一个"，是合法的
            // 减完之后 < 0 才说明"供不应求"
            if (count[c - 'a'] < 0) {
                return false;
            }
        }
        return true;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间    | 空间  | 优势                    |
// |-----------|---------|------|------------------------|
// | 暴力查找   | O(m×n)  | O(n) | 直觉简单               |
// | 哈希表计数 | O(m+n)  | O(1) | 通用，支持任意字符集     |
// | 数组计数   | O(m+n)  | O(1) | 最快，字符集固定时首选   |
//
// 面试推荐：直接写解法3，然后补充说"如果字符集是 Unicode 可改用 map"。
//
// 【易错点】
// 1. 数组未初始化：int count[26]; 值是垃圾
//    ✗ int count[26];
//    ✓ int count[26] = {0};
//
// 2. 判断阈值搞混：
//    ✗ 先减再判断时用 <= 0：count[c-'a']--; if (count[c-'a'] <= 0) 
//      → 这会把"刚好用完"也判为失败
//    ✓ 先减再判断用 < 0：  count[c-'a']--; if (count[c-'a'] < 0)
//    或者：先判断用 <= 0 再减：if (count[c-'a'] <= 0) return false; count[c-'a']--;
//    两种写法的阈值不同，逻辑等价但不能交叉混用。
//
// 3. 用 freq[c] 访问 unordered_map 中不存在的 key 会自动插入默认值 0，
//    这在本题中恰好是正确行为，但在其他场景可能导致意外副作用。
//
// 【面试追问】
// Q1: 暴力解能过吗？瓶颈在哪？
//     → 数据量 10^5，暴力 O(m×n)=O(10^10) 会超时。
//       瓶颈在于每次查找是线性的，需要预处理成 O(1) 查询。
//
// Q2: 如果 ransomNote 比 magazine 长，能否提前返回？
//     → 可以，一行剪枝：if (ransomNote.size() > magazine.size()) return false;
//       因为即使 magazine 每个字符都用上，总量也不够。
//
// Q3: 如果字符集不是小写字母而是整个 Unicode，怎么改？
//     → 把 int[26] 换成 unordered_map<char32_t, int>，
//       其余逻辑完全一样。
//
// Q4: 如果 magazine 的字符可以无限次使用，怎么改？
//     → 只需检查存在性，不需要计数。
//       用 unordered_set<char> 存 magazine 的字符集，
//       遍历 ransomNote 检查每个字符是否在 set 中。
// ============================================================
