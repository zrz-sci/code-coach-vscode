/*
 * LeetCode 93: 复原 IP 地址 (Restore IP Addresses)
 *
 * 【题目本质】
 * 把一个纯数字字符串切成恰好 4 段，每段是合法的 IP 段（0-255，无前导零）。
 * 这是"字符串分割"的经典回溯题。
 *
 * 【解法总览】
 * 解法1: 回溯（逐段选择）— O(3⁴·n) / O(n) — 面试首选 ⭐
 * 解法2: 三重循环暴力      — O(n³) / O(n) — 简单直接，但不够通用
 */

// ============================================================
// 解法1: 回溯（逐段选择）— 面试首选 ⭐
// 时间: O(3⁴ · n)  空间: O(n)（递归深度4 + 存结果）
//
// 【思路】
// IP 地址需要分成 4 段，每段可以取 1/2/3 个字符。
// 这是一个"从左到右逐段确定"的过程，天然适合回溯。
//
// 回溯三要素：
//   - 路径：已经选好的若干段（segments 数组）
//   - 选择列表：当前位置开始取 1/2/3 个字符
//   - 终止条件：4 段都选好 且 恰好用完所有字符
//
// 决策树（以 s = "25525511135" 为例，展示前几层）：
//
//                          (seg=0, start=0)
//                     /          |          \
//               取"2"          取"25"       取"255"
//           (seg=1,s=1)     (seg=1,s=2)   (seg=1,s=3)
//          /    |    \       /   |   \     /    |    \
//       "5"  "55"  "552"  "5" "52" "525" "2"  "25" "255"
//       ...   ...   ...   ...  ...  ...   ...  ...   ...
//
//  最终合法路径示例：
//  "255" → "255" → "11"  → "135"  => "255.255.11.135"
//  "255" → "255" → "111" → "35"   => "255.255.111.35"
//
// 剪枝策略：
//   1. 前导零 → break（更长的也必有前导零）
//   2. 值 > 255 → break（更长的值只会更大）
//   3. 剩余字符数过多或过少 → 直接 return
// ============================================================
class Solution1 {
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> result;
        vector<string> segments;
        backtrack(s, 0, segments, result);
        return result;
    }

private:
    void backtrack(const string& s, int start,
                   vector<string>& segments, vector<string>& result) {
        // 终止条件：已选满4段
        if (segments.size() == 4) {
            // 必须恰好用完所有字符（否则是非法的分割）
            if (start == (int)s.size()) {
                result.push_back(segments[0] + "." + segments[1] + "."
                                 + segments[2] + "." + segments[3]);
            }
            return;
        }

        // 剪枝：剩余字符数检查
        // 还需要选 needSegs 段，每段至少1字符、最多3字符
        int remaining = s.size() - start;
        int needSegs = 4 - segments.size();
        if (remaining < needSegs || remaining > needSegs * 3) return;

        // 每段尝试取 1、2、3 个字符
        for (int len = 1; len <= 3 && start + len <= (int)s.size(); len++) {
            string seg = s.substr(start, len);

            // 前导零判断：长度>1 且首字符为'0' → 非法
            // 为什么用 break 不用 continue：
            //   "01" 有前导零，"012" 也必有前导零，无需继续
            if (seg.size() > 1 && seg[0] == '0') break;

            // 值范围判断：超过 255 → 非法
            // 为什么用 break：长度增加，数值只会更大
            int val = stoi(seg);
            if (val > 255) break;

            // 做选择
            segments.push_back(seg);
            // 递归处理下一段
            backtrack(s, start + len, segments, result);
            // 撤销选择
            segments.pop_back();
        }
    }
};

// ============================================================
// 解法2: 三重循环暴力枚举
// 时间: O(n³)  空间: O(n)
//
// 【思路】
// IP 地址恰好 4 段，需要确定 3 个切点的位置。
// 直接用三重循环枚举切点 i, j, k，分别代表第2、3、4段的起始位置。
// 然后验证每段是否合法。
//
// 这种方法简单直接，但只适用于固定段数的情况。
// 如果段数变成 k，就需要 k-1 重循环，不实际。
//
// 切点示意（以 s = "25525511135" 为例）：
//
//   s = 2 5 5 | 2 5 5 | 1 1 | 1 3 5
//       ^       ^       ^     ^
//       0       i=3     j=6   k=8
//   seg1="255"  seg2="255"  seg3="11"  seg4="135"  ✓
//
//   s = 2 5 5 | 2 5 5 | 1 1 1 | 3 5
//       ^       ^       ^       ^
//       0       i=3     j=6     k=9
//   seg1="255"  seg2="255"  seg3="111"  seg4="35"  ✓
// ============================================================
class Solution2 {
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> result;
        int n = s.size();

        // i: 第2段起始位置，j: 第3段起始，k: 第4段起始
        // 每段长度 1-3，所以循环上界为 prev + 3
        for (int i = 1; i <= 3 && i < n; i++) {
            for (int j = i + 1; j <= i + 3 && j < n; j++) {
                for (int k = j + 1; k <= j + 3 && k < n; k++) {
                    // 第4段长度 = n - k，必须在 [1, 3] 范围内
                    if (n - k > 3) continue;

                    string s1 = s.substr(0, i);
                    string s2 = s.substr(i, j - i);
                    string s3 = s.substr(j, k - j);
                    string s4 = s.substr(k);

                    if (isValid(s1) && isValid(s2) && isValid(s3) && isValid(s4)) {
                        result.push_back(s1 + "." + s2 + "." + s3 + "." + s4);
                    }
                }
            }
        }
        return result;
    }

private:
    bool isValid(const string& seg) {
        // 空串或超过3位 → 非法
        if (seg.empty() || seg.size() > 3) return false;
        // 前导零：长度>1 且首字符为'0' → 非法（但"0"本身合法）
        if (seg.size() > 1 && seg[0] == '0') return false;
        // 值范围：必须 <= 255
        if (stoi(seg) > 255) return false;
        return true;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 解法1 (回溯)                    | 解法2 (三重循环)          |
// |------------|--------------------------------|--------------------------|
// | 时间复杂度 | O(3⁴ · n)，实际 ≈ O(81n)       | O(n³)，实际也很小         |
// | 空间复杂度 | O(n)（递归栈+segments）         | O(n)（存放子串）          |
// | 扩展性     | 改常数 4 为 k 即可处理 k 段分割 | k 段需 k-1 重循环，不可行 |
// | 代码风格   | 回溯模板，面试通用              | 简单暴力，专用于4段       |
// | 面试推荐   | ⭐ 首选                         | 可以先提思路再写回溯版    |
//
// 【易错点】
//
// 1. 终止条件漏检 start == s.size()：
//    ✗ if (segments.size() == 4) { result.push_back(...); }
//      → 可能4段选满但字符没用完，如 s="1111" 选了 "1","1","1" 后
//        还剩 "1" 会被忽略（这里恰好对了，但 s="11111" 就会出错）
//    ✓ if (segments.size() == 4 && start == s.size()) { ... }
//
// 2. 前导零判断用 continue 导致多余搜索：
//    ✗ if (seg.size() > 1 && seg[0] == '0') continue;
//      → "01" 有前导零还会继续尝试 "012"，浪费时间
//    ✓ 用 break：一旦出现前导零，更长的也必然有前导零
//
// 3. 三重循环中循环上界写错：
//    ✗ for (int i = 1; i <= 3; i++)  // 没有 i < n 的约束
//      → 如果 s 长度只有 4，i=3 时后面只剩1个字符，j/k 无空间
//    ✓ for (int i = 1; i <= 3 && i < n; i++)
//
// 4. "0" 被误判为非法：
//    ✗ if (seg[0] == '0') return false;  // "0" 本身是合法的！
//    ✓ if (seg.size() > 1 && seg[0] == '0') return false;
//
// 【面试追问】
//
// Q1（基础理解）: 如何判断一个 IP 段是否合法？有几个条件？
//   → 三个条件：① 非空且长度 ≤ 3；② 无前导零（长度>1时首字符不为'0'）；
//     ③ 数值在 [0, 255] 范围内。
//
// Q2（优化）: 回溯中有哪些剪枝策略？
//   → ① 剩余字符数过多或过少提前 return；
//     ② 前导零和超过255时用 break 而不是 continue（因为更长只会更差）。
//
// Q3（变体）: 如果要分成 k 段（每段 0-255），代码怎么改？
//   → 把终止条件 segments.size() == 4 改为 == k；
//     剩余段数计算改为 k - segments.size()。
//     回溯天然支持任意 k，而三重循环不行。
//
// Q4（变体）: 如果输入可能有非数字字符，如何处理？
//   → 方法1: 预处理检查，包含非数字直接返回空。
//     方法2: 在 isValid 中增加字符合法性检查。
// ============================================================
