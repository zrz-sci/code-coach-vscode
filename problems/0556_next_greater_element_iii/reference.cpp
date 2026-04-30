/*
 * LeetCode 556: Next Greater Element III (下一个更大元素 III)
 * 难度: Medium
 *
 * 题目: 给定正整数 n, 找由 n 的数字重排组成的、比 n 大的最小整数
 * 本质: "下一个排列" (Next Permutation, LC 31) 的整数版
 *
 * 核心算法 — Next Permutation 四步法:
 *   1. 从右往左找第一个下降位 i (digits[i] < digits[i+1])
 *   2. 从右往左找第一个 > digits[i] 的位置 j
 *   3. swap(digits[i], digits[j])
 *   4. reverse(i+1, end)  — 使后缀最小化
 *
 * 时间 O(d), 空间 O(d), d = 位数 (最多10)
 */


// =====================================================
// 解法一: 标准 Next Permutation — 字符串操作 (推荐)
// =====================================================
class Solution {
public:
    int nextGreaterElement(int n) {
        // 将整数转为字符串, 方便逐位操作
        string s = to_string(n);
        int len = s.size();

        // ---- Step 1: 从右往左找第一个下降位 ----
        // 找 i 使得 s[i] < s[i+1] (i 右侧是非递增序列)
        //
        // 例: "12443"
        //   i=3: s[3]='4' >= s[4]='3' → 继续
        //   i=2: s[2]='4' >= s[3]='4' → 继续
        //   i=1: s[1]='2' <  s[2]='4' → 找到! i=1
        int i = len - 2;
        while (i >= 0 && s[i] >= s[i + 1]) {
            i--;
        }

        // 整个数字是降序排列 (如 54321), 已是最大排列
        if (i < 0) return -1;

        // ---- Step 2: 从右往左找第一个比 s[i] 大的位置 ----
        // s[i+1..end] 是降序, 从右找第一个 > s[i] 的就是
        // 右侧中比 s[i] 大的最小元素
        //
        // 例: s="12443", i=1, s[i]='2'
        //   j=4: s[4]='3' > '2' → 找到! j=4
        int j = len - 1;
        while (s[j] <= s[i]) {
            j--;
        }

        // ---- Step 3: 交换 s[i] 和 s[j] ----
        // 交换后 s[i] 变大 (保证结果 > 原数)
        // 且 s[i+1..end] 仍保持降序 (交换不破坏降序性质)
        //
        // 例: "12443" → swap(s[1],s[4]) → "13442"
        swap(s[i], s[j]);

        // ---- Step 4: 反转 i+1 到末尾 ----
        // 将降序变为升序, 使后缀尽可能小 (保证是"最小的更大数")
        //
        // 例: "13|442" → reverse → "13|244" = 13244
        reverse(s.begin() + i + 1, s.end());

        // ---- Step 5: 溢出检查 ----
        // 转为 long 防溢出, 检查是否在 32 位整数范围内
        long result = stol(s);
        if (result > INT_MAX) return -1;

        return (int)result;
    }
};


/*
 * 详细 trace: n = 12443
 *
 * 字符串: "12443"
 *         索引: 0 1 2 3 4
 *
 * Step 1: 从右找下降位
 *   i=3: '4' >= '3' → 继续
 *   i=2: '4' >= '4' → 继续
 *   i=1: '2' <  '4' → 找到! i=1
 *
 * Step 2: 从右找 > s[1]='2' 的位置
 *   j=4: '3' > '2' → 找到! j=4
 *
 * Step 3: swap s[1] 和 s[4]
 *   "12443" → "13442"
 *
 * Step 4: reverse s[2..4]
 *   "13442" → "13244"  (442 → 244)
 *
 * Step 5: 13244 <= INT_MAX → 返回 13244  ✓
 *
 * -----------------------------------------------
 * trace: n = 230241
 *
 * Step 1: i=3 (s[3]='2' < s[4]='4')
 * Step 2: j=4 (s[4]='4' > '2')
 * Step 3: swap → "230421"
 * Step 4: reverse [4,5] → "230412"
 * 结果: 230412  ✓
 */


// =====================================================
// 解法二: 使用 STL next_permutation (简洁写法)
// =====================================================
// C++ 标准库提供了 next_permutation 函数, 可以直接使用
// 面试中不建议直接用, 因为考的就是手写这个算法
class Solution2 {
public:
    int nextGreaterElement(int n) {
        string s = to_string(n);

        // STL next_permutation 返回 false 表示已是最大排列
        if (!next_permutation(s.begin(), s.end())) {
            return -1;
        }

        long result = stol(s);
        return result > INT_MAX ? -1 : (int)result;
    }
};


// =====================================================
// 解法三: 纯数组操作 (不用 string/stol)
// =====================================================
// 某些面试官可能要求不用 string/stol, 纯数学操作
class Solution3 {
public:
    int nextGreaterElement(int n) {
        // 提取每一位数字 (低位在前)
        vector<int> digits;
        int temp = n;
        while (temp > 0) {
            digits.push_back(temp % 10);
            temp /= 10;
        }
        // 反转使高位在前
        reverse(digits.begin(), digits.end());
        int len = digits.size();

        // Step 1: 找下降位
        int i = len - 2;
        while (i >= 0 && digits[i] >= digits[i + 1]) i--;
        if (i < 0) return -1;

        // Step 2: 从右找第一个 > digits[i] 的
        int j = len - 1;
        while (digits[j] <= digits[i]) j--;

        // Step 3: 交换
        swap(digits[i], digits[j]);

        // Step 4: 反转后缀
        reverse(digits.begin() + i + 1, digits.end());

        // Step 5: 重建数字并检查溢出
        long result = 0;
        for (int d : digits) {
            result = result * 10 + d;
            if (result > INT_MAX) return -1;  // 提前剪枝
        }
        return (int)result;
    }
};


// =====================================================
// 边界测试用例
// =====================================================
/*
 * n = 12       → i=0, j=1, swap → "21", 结果 21
 * n = 21       → 全降序, 返回 -1
 * n = 1        → 单位数, 返回 -1
 * n = 11       → 无下降位, 返回 -1
 * n = 230241   → 结果 230412
 * n = 1999999999 → 下一排列 9199999999 > INT_MAX, 返回 -1
 * n = 2147483476 → 下一排列可能在 INT_MAX 范围, 需检查
 *
 * 易错点:
 *   1. Step 1 用 > 而不是 >= → [4,4,3] 中 4==4 不算下降, 必须 >=
 *   2. Step 2 从左往右找 → 找到的不是最小的大于 digits[i] 的元素; 必须从右往左
 *   3. 忘记溢出检查 → n=1999999999 的下一排列超 INT_MAX; 必须用 long 中转
 *   4. 单位数 / 全同数字 → 没有下降位直接返回 -1
 *
 * 面试追问:
 * Q1: 如果要找下一个更小的排列 (Previous Permutation)?
 *     → 反过来: 从右找第一个"上升点" (digits[i] > digits[i+1]),
 *       从右找第一个 < digits[i] 的交换, 然后反转后缀为降序.
 *
 * Q2: 如果要求第 k 个排列?
 *     → LC 60 Permutation Sequence, 用阶乘数系统解决.
 *
 * Q3: 如果 n 可以非常大(超出 long)?
 *     → 使用字符串操作全程, 不转数值. 比较时逐位比较字符串.
 *
 * 关联题目:
 *   31.  Next Permutation — 数组版 (本题核心算法来源)
 *   496. Next Greater Element I — 单调栈 (名字类似但方法不同)
 *   503. Next Greater Element II — 单调栈 + 循环 (名字类似但方法不同)
 *   60.  Permutation Sequence — 第 k 个排列 (阶乘数系统)
 */
