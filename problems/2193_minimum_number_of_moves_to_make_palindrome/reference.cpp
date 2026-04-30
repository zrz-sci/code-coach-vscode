/*
 * LeetCode 2193: 得到回文串的最少操作次数
 *
 * 【题目本质】
 * 给定字符串，只能相邻交换，求变成回文的最少交换次数。
 * 核心是贪心：从两端向内配对，每次找最近的匹配字符交换到对称位置。
 *
 * 【解法总览】
 * 解法1: 贪心 + 双指针模拟 — O(n²) / O(n) — 面试首选，直观易实现
 * 解法2: 贪心 + BIT(树状数组) — O(n log n) / O(n) — 大数据量优化
 */

// ============================================================
// 解法1: 贪心 + 双指针模拟
// 时间: O(n²)  空间: O(n)（字符串拷贝）
//
// 【思路】
// 回文串要求 s[i] == s[n-1-i]。我们从两端向内处理：
// - 对于左端字符 s[left]，在 [left+1, right] 从右向左找第一个相同字符
// - 找到了：通过相邻交换移到 right 位置，moves += (right - k)
// - 没找到：说明该字符是奇数次的，只出现一次在当前窗口中
//   此时把它往右推一步（swap 一次），下一轮重新处理
//
// 为什么从右往左找？因为离 right（目标位置）最近，交换次数最少。
//
// 可视化示例 "mamad":
//   初始:   m a m a d
//           ↑       ↑
//           L       R
//
//   Step1: s[L]='m', 从R往左找'm' → k=2
//          交换 s[2]→s[3]→s[4]: m a a d m (2次)
//                                       → m a a d m
//          moves=2, L=1, R=3
//
//   Step2: s[1]='a', 从R=3往左找'a' → k=2
//          交换 s[2]→s[3]: m a d a m (1次)
//          moves=3, L=2, R=2 → 结束
//
//   总计: 3 次
// ============================================================
class Solution1 {
public:
    int minMovesToMakePalindrome(string s) {
        int n = s.size();
        int moves = 0;
        int left = 0, right = n - 1;

        while (left < right) {
            // 从 right 向左寻找与 s[left] 匹配的字符
            int k = right;
            while (k > left && s[k] != s[left]) {
                k--;
            }

            if (k == left) {
                // 没找到配对 —— 这个字符出现奇数次，最终要放中间
                // 贪心策略：先把它往右推一步，后续自然归位到中间
                // 为什么只推一步？因为我们不知道最终中间位置在哪
                // （随着其他交换会变），每次推一步是最安全的
                swap(s[left], s[left + 1]);
                moves++;
                // 注意：left 不移动，下一轮重新尝试配对
            } else {
                // 找到配对，把 s[k] 一步步交换到 right 位置
                while (k < right) {
                    swap(s[k], s[k + 1]);
                    k++;
                    moves++;
                }
                // 配对完成，双指针都向内收缩
                left++;
                right--;
            }
        }

        return moves;
    }
};

// ============================================================
// 解法2: 贪心 + 树状数组 (BIT)
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 解法1 的瓶颈：每次配对都要 O(n) 地移动字符。
// 优化思路：不真的移动字符，而是：
//   1. 先用贪心确定配对关系（哪两个位置的字符配在一起）
//   2. 由配对关系推出每个字符的目标位置，构成一个排列 perm
//   3. 相邻交换把原排列变成目标排列的最少次数 = 排列的逆序对数
//   4. 用树状数组在 O(n log n) 内计算逆序对数
//
// 配对策略：
//   - 每个字符的所有出现位置存入队列
//   - 对于偶数次的字符，最前面的和最后面的配对（它们分别去左端和右端）
//   - 奇数次字符的中间那个去 n/2 位置
//   - 所有配对按 front 位置排序 → 第 i 对去位置 (i, n-1-i)
//
// 逆序对计算：
//   从右到左扫描 perm，对每个 perm[i]，查询树状数组中已插入的
//   比 perm[i] 小的数的个数（即前面有多少个位置的目标值更小）
// ============================================================
class Solution2 {
    // 树状数组
    vector<int> bit;
    int sz;

    void update(int i, int val) {
        for (; i <= sz; i += i & (-i))
            bit[i] += val;
    }

    int query(int i) {
        int s = 0;
        for (; i > 0; i -= i & (-i))
            s += bit[i];
        return s;
    }

public:
    int minMovesToMakePalindrome(string s) {
        int n = s.size();
        sz = n;

        // Step1: 收集每个字符的所有出现位置
        deque<int> indices[26];
        for (int i = 0; i < n; i++) {
            indices[s[i] - 'a'].push_back(i);
        }

        // Step2: 贪心配对
        // 每个字符，最前面和最后面的配对，次前面和次后面配对，以此类推
        // 如果出现奇数次，中间那个留给回文中心
        vector<pair<int, int>> pairs; // (较小索引, 较大索引)
        int midPos = -1;

        for (int c = 0; c < 26; c++) {
            auto& dq = indices[c];
            while ((int)dq.size() >= 2) {
                int front = dq.front(); dq.pop_front();
                int back = dq.back(); dq.pop_back();
                pairs.push_back({front, back});
            }
            if (!dq.empty()) {
                midPos = dq.front();
                dq.pop_front();
            }
        }

        // Step3: 按 front 排序，第 i 对分配到位置 (i, n-1-i)
        sort(pairs.begin(), pairs.end());

        vector<int> perm(n);
        int leftSlot = 0, rightSlot = n - 1;
        for (auto& [a, b] : pairs) {
            perm[a] = leftSlot++;
            perm[b] = rightSlot--;
        }
        if (midPos != -1) {
            perm[midPos] = n / 2;
        }

        // Step4: 用树状数组计算 perm 的逆序对数
        // 逆序对 (i, j): i < j 但 perm[i] > perm[j]
        // 从右到左扫描，对每个 perm[i]，查询已插入的比它小的数的个数
        bit.assign(n + 2, 0);
        long long moves = 0;
        for (int i = n - 1; i >= 0; i--) {
            // query(perm[i]) = 右边已插入的、目标位置 < perm[i] 的个数
            // 但我们要的是 perm[i] > perm[j] 且 j > i，即右边比我小的
            moves += query(perm[i]); // perm[i] 是 0-indexed，query 范围 [1, perm[i]]
            update(perm[i] + 1, 1); // 转成 1-indexed
        }

        return (int)moves;
    }
};

// ============================================================
// 【解法对比】
// 
// 解法1 vs 解法2:
//   - 解法1 直接模拟交换过程，代码简洁，O(n²)，n≤2000 完全够用
//   - 解法2 把问题转化为"构造目标排列 + 计算逆序对"，O(n log n)
//     但实现复杂度高，配对关系的构造容易出错
//   - 面试中推荐先写解法1，然后口述解法2的优化思路
//
// 【易错点】
// 1. 解法1: 奇数字符处理
//    ✗ 遇到 k==left 时 left++（跳过）→ 这个字符永远不会被处理
//    ✓ 遇到 k==left 时 swap(s[left], s[left+1]); moves++; 不动 left
//
// 2. 解法1: 找配对方向
//    ✗ 从 left+1 向右找 → 可能找到离 right 很远的，增加不必要的交换
//    ✓ 从 right 向左找 → 离目标位置最近，交换次数最少
//
// 3. 解法2: 配对排序
//    ✗ 按 back 排序 → 目标位置分配错误，比如大索引被分到 leftSlot
//    ✓ 按 front 排序 → front 小的配对应该在外层（对应更靠左的目标位置）
//
// 4. 解法2: BIT 的 1-indexed 转换
//    ✗ 直接用 perm[i] 调用 update/query → perm[i]=0 时 BIT 循环不工作
//    ✓ 用 perm[i]+1 调用 update，query(perm[i]) 查询 [1, perm[i]] 范围
//
// 【面试追问】
// Q1: 为什么贪心"从右找最近配对"是全局最优？
//     → 交换论证：假设最优解不选最右配对，可以证明交换为最右配对后
//       不会增加总交换数。因为选更近的配对不会增加其他字符的路径长度。
//
// Q2: n 很大（10^6）时如何优化？
//     → 解法2：贪心配对 + BIT 计算逆序对，O(n log n)。
//       或者用归并排序计算逆序对也行。
//
// Q3: 如果不保证能构成回文怎么办？
//     → 先检查：统计每个字符频次，奇数频次字符数 > 1 则无解
//       （偶数长度要求所有频次为偶数，奇数长度允许恰好一个奇数频次）。
//
// Q4: 如果可以交换任意两个位置（不限相邻），最少几次？
//     → 那就是另一个问题了：构造目标回文排列，计算从原排列到目标排列
//       需要的最少交换次数 = n - 排列中环的个数。
// ============================================================
