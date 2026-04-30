/*
 * LCR 016: 无重复字符的最长子串 (同主站第 3 题)
 * 难度: Medium | 标签: Hash Table, String, Sliding Window
 *
 * 题意: 给定字符串 s，找出不含重复字符的最长连续子串的长度。
 *
 * 核心思路:
 *   滑动窗口 [left, right] 维护无重复字符的区间。
 *   用 last[128] 记录每个字符最后出现的位置。
 *   当 s[right] 已在窗口内时, left 跳到 last[s[right]] + 1。
 *   关键: left = max(left, last[s[right]] + 1) 防止回退。
 *
 * 示例: s = "abcabcbb"
 *   right=0 'a': left=0, 窗口[a], ans=1
 *   right=1 'b': left=0, 窗口[ab], ans=2
 *   right=2 'c': left=0, 窗口[abc], ans=3
 *   right=3 'a': left=1, 窗口[bca], ans=3
 *   right=4 'b': left=2, 窗口[cab], ans=3
 *   ...最终 ans=3
 */


// ============================================================
// 解法1: 滑动窗口 + 数组记录位置 ⭐ 推荐
// 时间: O(n)  空间: O(128) = O(1)
//
// last[c]: 字符 c 最后出现的索引, 初始 -1
// left 跳跃: left = max(left, last[s[right]] + 1)
//
// 为什么需要 max?
//   反例 s="abba": right=3('a'), last['a']=0
//   不用 max: left 回退到 1, 窗口 [b,b,a] 含重复!
//   用 max: left 保持 2, 窗口 [b,a] ✓
// ============================================================
class Solution1 {
public:
    int lengthOfLongestSubstring(string s) {
        vector<int> last(128, -1);  // ASCII 字符集, 初始化 -1
        int ans = 0, left = 0;

        for (int right = 0; right < (int)s.size(); right++) {
            // 核心: left 跳到重复字符的下一个位置(但不能回退)
            left = max(left, last[s[right]] + 1);
            // 更新答案: 窗口长度 = right - left + 1
            ans = max(ans, right - left + 1);
            // 记录当前字符的最新位置
            last[s[right]] = right;
        }

        return ans;
    }
};

// ============================================================
// 解法2: 滑动窗口 + HashSet (逐步收缩)
// 时间: O(n)  空间: O(min(n, 128))
//
// 思路: 用 unordered_set 记录窗口内的字符。
//       右指针扩展时, 如果 s[right] 在 Set 中,
//       逐步移出 s[left] 直到无重复。
// 优点: 逻辑直观, 不需要记录位置
// 缺点: 常数因子较大(hash 操作)
//
// 摊还分析: 每个字符最多被 left 和 right 各访问一次 → O(2n) = O(n)
// ============================================================
class Solution2 {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_set<char> window;  // 窗口内的字符集合
        int ans = 0, left = 0;

        for (int right = 0; right < (int)s.size(); right++) {
            // 收缩: 逐步移出 left 处的字符, 直到 s[right] 不在窗口中
            while (window.count(s[right])) {
                window.erase(s[left]);
                left++;
            }
            // 加入 s[right]
            window.insert(s[right]);
            // 更新答案
            ans = max(ans, right - left + 1);
        }

        return ans;
    }
};

// ============================================================
// 解法3: 滑动窗口 + HashMap (跳跃式)
// 时间: O(n)  空间: O(min(n, 128))
//
// 思路: HashMap 记录每个字符最后出现的位置, 跳跃更新 left。
//       和解法1 逻辑相同, 但用 unordered_map 代替固定数组。
// 适用: 字符集未知或很大 (如 Unicode)
// ============================================================
class Solution3 {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> lastPos;
        int ans = 0, left = 0;

        for (int right = 0; right < (int)s.size(); right++) {
            // 如果 s[right] 在窗口内出现过, 跳跃更新 left
            if (lastPos.count(s[right]) && lastPos[s[right]] >= left) {
                left = lastPos[s[right]] + 1;
            }
            ans = max(ans, right - left + 1);
            lastPos[s[right]] = right;
        }

        return ans;
    }
};

/*
 * ============================================================
 * 解法对比
 * ============================================================
 * | 解法         | 时间 | 空间      | left 更新 | 常数 | 面试推荐 |
 * |-------------|------|-----------|----------|------|---------|
 * | 数组跳跃     | O(n) | O(128)   | 直接跳跃  | 最小 | ⭐首选   |
 * | HashSet收缩  | O(n) | O(min字符)| 逐步收缩  | 较大 | 通用    |
 * | HashMap跳跃  | O(n) | O(min字符)| 直接跳跃  | 中等 | 大字符集 |
 *
 * ============================================================
 * 易错点
 * ============================================================
 * 1. left 回退导致窗口含重复:
 *    错误: left = last[s[right]] + 1  (无 max)
 *    反例: "abba", right=3('a'), last['a']=0, left 回退到1
 *    正确: left = max(left, last[s[right]] + 1)
 *
 * 2. last 数组大小不够:
 *    错误: 只开 26 (字母), 漏掉数字/符号/空格
 *    正确: 开 128 (完整 ASCII)
 *
 * 3. HashSet 收缩时忘记删除:
 *    错误: while(window.count(s[right])) { left++; }
 *    正确: while(window.count(s[right])) { window.erase(s[left++]); }
 *
 * 4. 窗口长度差一:
 *    错误: right - left
 *    正确: right - left + 1 (闭区间)
 *
 * 5. 空串未处理:
 *    s="" 时循环不执行, ans=0 → 正确(但要确认不会数组越界)
 *
 * ============================================================
 * 面试追问
 * ============================================================
 * Q1: 字符集是 Unicode 怎么办?
 *     A: 不能用固定数组, 改用 HashMap (解法3)。
 *
 * Q2: 至多 K 个不同字符的最长子串?
 *     A: 窗口 + HashMap 计数, 不同字符数>K 时收缩。(LC 340)
 *
 * Q3: 返回子串本身而非长度?
 *     A: 记录 ans 更新时的 left, 最后 s.substr(bestLeft, ans)。
 *
 * Q4: 时间真的是 O(n)? 解法2 有 while 循环。
 *     A: 摊还 O(n)。每个字符最多被 left/right 各访问一次, 总 ≤ 2n。
 *
 * Q5: 空间能做到 O(1)?
 *     A: 字符集固定(ASCII)时, 数组大小128是常数, 已是 O(1)。
 */
