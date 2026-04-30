/*
 * LeetCode 1044: Longest Duplicate Substring (最长重复子串)
 *
 * 【解法】二分 + Rabin-Karp 滚动哈希
 * 时间: O(n log n)  空间: O(n)
 *
 * 核心思想:
 * 1. 答案长度具有单调性: 存在长度L的重复子串 => 存在长度L-1的
 *    不存在长度L的重复子串 => 不存在长度L+1的
 *    因此可以二分搜索答案长度
 *
 * 2. check(L): 用 Rabin-Karp 滚动哈希判断是否存在长度L的重复子串
 *    - 将子串看作 base 进制数，取大质数模
 *    - 滑动窗口 O(1) 更新哈希: hash_new = (hash_old - s[i]*base^(L-1)) * base + s[i+L]
 *    - 用 HashMap 存 <哈希值, 起始位置列表>，发现重复哈希时逐字符验证
 *
 * 3. 使用梅森素数 (1LL << 61) - 1 作为模数:
 *    - 极大的模数(~2*10^18)，冲突概率极低
 *    - 需要 __int128 中间乘法防溢出
 *
 * 示例: s = "banana"
 *   二分: mid=3 -> check(3): "ban","ana","nan","ana" -> "ana"重复 -> true
 *         mid=4 -> check(4): "bana","anan","nana" -> 无重复 -> false
 *   答案长度=3, 返回 "ana"
 */


class Solution {
public:
    string longestDupSubstring(string s) {
        int n = s.size();
        // ---------- Rabin-Karp 参数 ----------
        // 梅森素数: 2^61 - 1, 极大且利于位运算优化
        long long mod = (1LL << 61) - 1;
        // base 取大于字符集大小(26)的质数
        long long base = 31;

        // ---------- 二分搜索答案长度 ----------
        // 范围 [1, n-1]: 长度为0无意义, 长度为n不可能重复
        int lo = 1, hi = n - 1;
        int ansStart = -1;  // 记录找到的重复子串起始位置
        int ansLen = 0;     // 记录找到的重复子串长度

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            int startIdx = check(s, mid, base, mod);

            if (startIdx != -1) {
                // 存在长度为 mid 的重复子串，尝试更长
                ansStart = startIdx;
                ansLen = mid;
                lo = mid + 1;
            } else {
                // 不存在，缩短
                hi = mid - 1;
            }
        }

        return ansLen > 0 ? s.substr(ansStart, ansLen) : "";
    }

private:
    // ---------- 安全乘法取模 (防 long long 溢出) ----------
    // a, b 均在 [0, mod) 范围内, mod < 2^61
    // a*b 可能达到 ~2^122, 超过 long long 范围
    // 用 __int128 做中间计算
    long long mulmod(long long a, long long b, long long mod) {
        return (__int128)a * b % mod;
    }

    // ---------- check 函数: 判断是否存在长度为 L 的重复子串 ----------
    // 返回重复子串的起始索引 (不存在返回 -1)
    //
    // 工作原理:
    //   1. 计算 power = base^(L-1) mod mod (用于滑动时去掉最左字符)
    //   2. 计算第一个窗口 s[0..L-1] 的哈希值
    //   3. 滑动窗口, 每次 O(1) 更新哈希:
    //      hash = (hash - s[i-1] * power) * base + s[i+L-1]
    //   4. 用 HashMap<hash, list<startIdx>> 记录见过的哈希
    //   5. 发现重复哈希时, 逐字符验证消除冲突
    int check(const string& s, int L, long long base, long long mod) {
        if (L == 0) return -1;
        int n = s.size();

        // Step 1: 计算 power = base^(L-1) % mod
        long long power = 1;
        for (int i = 0; i < L - 1; i++) {
            power = mulmod(power, base, mod);
        }

        // Step 2: 计算第一个窗口的哈希值 s[0..L-1]
        // hash = s[0]*base^(L-1) + s[1]*base^(L-2) + ... + s[L-1]*base^0
        long long hash = 0;
        for (int i = 0; i < L; i++) {
            hash = (mulmod(hash, base, mod) + (s[i] - 'a' + 1)) % mod;
            // 注意: 用 s[i]-'a'+1 (从1开始), 避免前导零问题
        }

        // Step 3: HashMap 存储 <哈希值 -> 所有对应起始位置>
        // 用 vector<int> 因为不同位置可能产生相同哈希(冲突)
        unordered_map<long long, vector<int>> seen;
        seen[hash].push_back(0);

        // Step 4: 滑动窗口
        for (int i = 1; i + L - 1 < n; i++) {
            // 去掉最左字符 s[i-1], 加入新字符 s[i+L-1]
            // hash_new = (hash_old - s[i-1] * power) * base + s[i+L-1]
            //
            // 注意负数取模! hash - s[i-1]*power 可能为负
            // 加 mod 确保非负
            long long leftChar = s[i - 1] - 'a' + 1;
            hash = (hash - mulmod(leftChar, power, mod) % mod + mod) % mod;
            hash = (mulmod(hash, base, mod) + (s[i + L - 1] - 'a' + 1)) % mod;

            // Step 5: 检查是否有重复
            if (seen.count(hash)) {
                // 哈希相同 -> 可能真重复, 也可能是冲突
                // 逐字符验证每个候选位置
                for (int prev : seen[hash]) {
                    if (s.compare(prev, L, s, i, L) == 0) {
                        return i;  // 确认重复!
                    }
                }
            }
            seen[hash].push_back(i);
        }

        return -1;  // 不存在长度为 L 的重复子串
    }
};

/*
 * ==================== 方法对比 ====================
 *
 * 方法一: 二分 + Rabin-Karp (本解法)
 *   时间: O(n log n) -- 二分O(log n)轮, 每轮O(n)哈希检查
 *   空间: O(n)       -- HashMap 存储哈希值
 *   优点: 实现直观, 面试友好
 *   缺点: 哈希冲突需要额外处理
 *
 * 方法二: 后缀数组 + LCP
 *   时间: O(n log n) (倍增) 或 O(n) (SA-IS)
 *   空间: O(n)
 *   优点: 无冲突问题, 理论更优
 *   缺点: 实现极复杂, 面试不推荐
 *
 * ==================== 关键易错点 ====================
 *
 * 1. 负数取模:
 *    hash - x*power 可能为负, 必须 +mod 再 %mod
 *    这是 Rabin-Karp 最常见的 bug!
 *
 * 2. __int128 乘法:
 *    mod = 2^61-1 约 2*10^18, 两个 mod 范围的数相乘溢出 long long
 *    必须用 __int128 中间计算
 *
 * 3. 字符编码从 1 开始:
 *    s[i]-'a'+1 而非 s[i]-'a', 避免 'a'=0 导致 "a","aa","aaa"... 的
 *    哈希前缀为零, 增加冲突概率
 *
 * 4. 二分边界:
 *    lo=1, hi=n-1, 不是 lo=0 或 hi=n
 *    长度0没意义, 长度n整个字符串不可能重复
 *
 * 5. 冲突验证:
 *    哈希相同不代表字符串相同, 必须用 s.compare() 或 substr 二次验证
 *    否则极端用例会 WA
 */

// ============================================================
// 总结
// ============================================================
/*
 * -------- 解法对比一览 --------
 *
 * | 解法                | 时间         | 空间 | 适用场景            | 面试推荐 |
 * |--------------------|-------------|------|---------------------|---------|
 * | 二分+Rabin-Karp    | O(n log n)  | O(n) | 实现直观，面试友好    | ⭐首选  |
 * | 后缀数组+LCP       | O(n log n)  | O(n) | 理论更优，无冲突     | 了解     |
 * | 后缀数组(SA-IS)    | O(n)        | O(n) | 竞赛级，实现极复杂    | 知道即可 |
 *
 * -------- 易错点 --------
 *
 * 1. 负数取模 -- hash - x*power 可能为负，必须 +mod 再 %mod
 *    这是 Rabin-Karp 最常见的 bug
 * 2. 乘法溢出 -- mod=2^61-1 时两个 mod 范围数相乘超 long long
 *    必须用 __int128 做中间计算 (mulmod)
 * 3. 字符编码从 1 开始 -- s[i]-'a'+1 而非 s[i]-'a'
 *    避免 'a'=0 导致前导零，增加哈希冲突概率
 * 4. 二分边界 -- lo=1, hi=n-1; 长度 0 无意义，长度 n 不可能重复
 * 5. 必须二次验证 -- 哈希相同不等于字符串相同
 *    用 s.compare(pos1, L, s, pos2, L) 逐字符确认
 *
 * -------- 面试追问链 --------
 *
 * Q1: 如何处理哈希冲突? 能否完全避免?
 *     -> 双哈希(两组 base/mod)可将冲突概率降到 ~1/mod^2
 *        后缀数组方案完全无冲突，但实现复杂度高
 *
 * Q2: 如果要找出现至少 k 次的最长子串?
 *     -> 二分长度不变，check 函数改为: HashMap 中某个哈希
 *        出现 >= k 次即返回 true; 后缀数组方案用 LCP 数组
 *        的滑动窗口最小值
 *
 * Q3: 如果字符集不是小写字母而是任意 byte?
 *     -> base 取 257 (>256), 字符映射改为 s[i]+1
 *        其余逻辑不变; 注意 base 越大冲突越少但溢出风险增加
 */
