/*
 * 【题目本质】
 * 在 [1, n] 范围内的数字按字典序排列，找第 k 个。
 * 等价于在十叉 Trie 上做前序遍历找第 k 个节点。
 *
 * 【解法总览】
 * 解法1: 十叉Trie计数法 — O(log²n) / O(1) ⭐ 首选
 */

// ===================== Solution1: 十叉Trie计数法 =====================
// 思路：将字典序视为十叉 Trie，计算子树大小来跳过/进入
// 时间 O(log²n)  空间 O(1)
class Solution1 {
public:
    int findKthNumber(int n, int k) {
        int curr = 1;
        k--; // curr=1 已经是第1个数
        while (k > 0) {
            long steps = countSteps(n, curr);
            if (steps <= k) {
                // 跳过以 curr 为根的整棵子树
                k -= steps;
                curr++;
            } else {
                // 进入子树（前序遍历的下一个是 curr*10）
                k--;
                curr *= 10;
            }
        }
        return curr;
    }

private:
    // 计算以 prefix 为前缀的数在 [1, n] 中有多少个
    long countSteps(int n, long prefix) {
        long steps = 0;
        long first = prefix, last = prefix;
        while (first <= n) {
            // 当前层的节点数 = min(n+1, last+1) - first
            steps += min((long)n + 1, last + 1) - first;
            first *= 10;
            last = last * 10 + 9;
        }
        return steps;
    }
};

// ===================== Solution2: 直接模拟前序遍历(会TLE) =====================
// 思路：按字典序枚举每一个数直到第 k 个
// 时间 O(k)  空间 O(1)  — 当 k 接近 n 时会超时
class Solution2 {
public:
    int findKthNumber(int n, int k) {
        int curr = 1;
        k--;
        while (k > 0) {
            if ((long)curr * 10 <= n) {
                curr *= 10; // 进入子节点
            } else {
                while (curr % 10 == 9 || curr >= n) {
                    curr /= 10; // 回溯到父节点
                }
                curr++; // 移到右兄弟
            }
            k--;
        }
        return curr;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 | 劣势 |
 * |------|------|------|------|------|
 * | Solution1 十叉Trie计数 | O(log²n) | O(1) | 极快，不依赖k大小 | 逻辑需要理解 |
 * | Solution2 直接遍历 | O(k) | O(1) | 思路简单 | k大时超时 |
 *
 * 【易错点】
 * 1. long 溢出：prefix * 10 和 last * 10 + 9 都可能超 int
 * 2. countSteps 中上界是 n+1（开区间），不是 n
 * 3. 起点 curr=1 需要先 k--
 * 4. 进入子树时 curr*=10 之前要 k--（消耗当前 prefix 节点）
 *
 * 【面试追问】
 * Q1: 为什么不能直接排序？
 *     n 最大 10^9，无法存储所有数，必须用数学方法计数
 * Q2: countSteps 是如何逐层累加的？
 *     每层 [first, min(n, last)] 范围内的数都以 prefix 为前缀
 * Q3: 能否扩展到任意字符集的字典序？
 *     可以，将 10 替换为字符集大小即可
 */
