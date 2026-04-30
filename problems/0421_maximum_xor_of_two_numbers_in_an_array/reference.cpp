/*
 * LeetCode 421: Maximum XOR of Two Numbers in an Array (数组中两个数的最大异或值)
 *
 * 【题目本质】
 * 给定整数数组 nums，返回 nums[i] XOR nums[j] 的最大值。
 * 核心：利用 Trie（二进制字典树）或哈希表，从高位到低位贪心确定答案。
 *
 * 【字典树 ASCII 图】
 * 将 nums = [3(011), 10(1010), 5(101), 25(11001)] 的部分位插入Trie:
 *
 *                root
 *               /    \
 *             [0]    [1]         ← bit 4
 *            /   \      \
 *          [0]   [1]    [1]     ← bit 3
 *          / \    |       \
 *        [0] [1] [0]     [0]   ← bit 2
 *         |   |   |       |
 *        [1] [0] [1]     [0]   ← bit 1
 *         |   |   |       |
 *        [1] [1] [0]     [1]   ← bit 0
 *        (3) (5) (10)   (25)
 *
 *   查询 5(00101): 每位走相反方向
 *     bit4=0→走1(25), bit3=0→走1(25), bit2=1→走0(25)
 *     bit1=0→走1? 25的bit1=0, 无→走0, bit0=1→走0? 25的bit0=1, 无→走1
 *     XOR = 11100 = 28 = 5^25 ✓
 *
 * 【解法总览】
 * 解法1: Trie（字典树）    — O(32n) / O(32n)  — 面试首选
 * 解法2: 哈希表逐位确定    — O(32n) / O(n)    — 替代方案
 * 解法3: Trie（数组实现）  — O(32n) / O(32n)  — 内存优化
 */

// ============================================================
// 解法1: Trie（指针实现）— 面试首选 ⭐
// 时间: O(32n)  空间: O(32n)
//
// 【思路】
// 1. 将所有数的二进制（从 bit 31 到 bit 0）插入 Trie
// 2. 对每个数，在 Trie 中贪心查找：
//    对当前数的每一位 bit，优先走 children[1-bit]（相反方向）
//    如果走了相反方向，当前位 XOR 为 1，贡献 (1 << i) 到结果
//    如果只能走相同方向，当前位 XOR 为 0，不贡献
//
// 为什么这样做是正确的？
//   - XOR 要最大化，从高位贪心是最优的（类似贪心选最大前缀）
//   - 高位为 1 的价值永远大于低位全为 1 的总和
//     (2^k > 2^(k-1) + 2^(k-2) + ... + 2^0)
//
// 示例: nums = [3, 10, 5, 25, 2, 8]
//   插入所有数到 Trie
//   查询 num=5 (00...00101):
//     bit31~5 全0: 在 Trie 中沿 25(11001) 的路径走反向
//     bit4=0: want=1, 有(25)! curXor |= 16
//     bit3=0: want=1, 有(25)! curXor |= 8
//     bit2=1: want=0, 有(25)! curXor |= 4
//     bit1=0: want=1, 无      curXor 不变
//     bit0=1: want=0, 无      curXor 不变
//     curXor = 28 = 11100
//   maxXor = max(all queries) = 28
// ============================================================
class Solution1 {
    struct TrieNode {
        TrieNode* children[2] = {nullptr, nullptr};
    };

public:
    int findMaximumXOR(vector<int>& nums) {
        // 1. 构建 Trie: 将所有数的二进制从高位到低位插入
        TrieNode* root = new TrieNode();
        for (int num : nums) {
            TrieNode* node = root;
            for (int i = 31; i >= 0; i--) {
                int bit = (num >> i) & 1;
                if (!node->children[bit]) {
                    node->children[bit] = new TrieNode();
                }
                node = node->children[bit];
            }
        }

        // 2. 查询: 对每个数在 Trie 中贪心找最大 XOR
        int maxXor = 0;
        for (int num : nums) {
            TrieNode* node = root;
            int curXor = 0;
            for (int i = 31; i >= 0; i--) {
                int bit = (num >> i) & 1;
                int want = 1 - bit; // 想走相反方向
                if (node->children[want]) {
                    curXor |= (1 << i); // 这一位 XOR 为 1
                    node = node->children[want];
                } else {
                    // 只能走相同方向，这一位 XOR 为 0
                    node = node->children[bit];
                }
            }
            maxXor = max(maxXor, curXor);
        }
        return maxXor;
    }
};

// ============================================================
// 解法2: 哈希表（逐位确定答案）
// 时间: O(32n)  空间: O(n)
//
// 【思路】
// 从最高位到最低位，逐位确定最终答案的每一位能否为 1。
//
// 对于第 i 位：
//   1. 取所有数的前缀（只保留 bit 31 ~ bit i），存入 set
//   2. 假设答案在第 i 位为 1，即 candidate = maxXor | (1 << i)
//   3. 对集合中每个前缀 p，检查 p ^ candidate 是否也在集合中
//      如果是，说明存在两个数的前缀 XOR = candidate，第 i 位可以为 1
//      如果不是，第 i 位只能为 0
//
// 正确性: 利用 XOR 的自反性 a ^ b = c ⟺ a ^ c = b
//   如果存在 p1 和 p2 使得 p1 ^ p2 = candidate，
//   则 p1 ^ candidate = p2，所以只需检查 p ^ candidate 是否在集合中
//
// 示例: nums = [3(00011), 10(01010), 5(00101), 25(11001)]
//   bit 4:
//     prefixes = {0, 0, 0, 1} → set = {0, 1}
//     candidate = 10000, 对 0: 0^10000=10000? 不在. 对 1: 1^10000=10001? 不在
//     → maxXor 第4位=0
//   bit 3:
//     prefixes = {00, 01, 00, 11} → set = {00, 01, 11}
//     candidate = 01000, 对 00: ^=01000? 不在. 对 01: ^=01001? 不在
//     → maxXor 第3位=0
//   ... (继续逐位判断)
//   最终 maxXor = 28
// ============================================================
class Solution2 {
public:
    int findMaximumXOR(vector<int>& nums) {
        int maxXor = 0;

        for (int i = 31; i >= 0; i--) {
            // 取所有数的前缀 (bit 31 ~ bit i)
            unordered_set<int> prefixes;
            int mask = maxXor | (1 << i); // 假设第 i 位能取 1
            for (int num : nums) {
                prefixes.insert(num >> i); // 只取高位前缀
            }

            // 检查是否存在两个前缀 XOR = 候选值
            int candidate = maxXor | (1 << i);
            // 注意: candidate 也要右移到对应的前缀位数
            // 实际上我们直接用完整的前缀来检查
            bool found = false;
            // 重新构建: 用 mask 取前缀
            prefixes.clear();
            for (int num : nums) {
                int prefix = num & (~0 << i); // 保留 bit i 及以上
                prefixes.insert(prefix);
            }
            int candidateFull = maxXor | (1 << i);
            for (int p : prefixes) {
                if (prefixes.count(p ^ candidateFull)) {
                    found = true;
                    break;
                }
            }
            if (found) {
                maxXor = candidateFull;
            }
        }
        return maxXor;
    }
};

// ============================================================
// 解法3: Trie（数组实现，避免 new/delete）
// 时间: O(32n)  空间: O(32n)
//
// 【思路】
// 和解法1相同的 Trie 思路，但用二维数组代替指针。
// 内存更友好（连续内存，cache-friendly）。
// 预分配 32 * n 个节点足够。
//
// trie[nodeId][0/1] = 子节点的编号
// nodeId=0 是根节点
// cnt 是下一个可用的节点编号
// ============================================================
class Solution3 {
public:
    int findMaximumXOR(vector<int>& nums) {
        int n = nums.size();
        // 最多 32 * n 个节点（每个数最多创建 32 个新节点）
        vector<array<int, 2>> trie(32 * n + 1, {0, 0});
        int cnt = 1; // 根节点是 0，下一个可分配的是 1

        // 插入
        for (int num : nums) {
            int node = 0;
            for (int i = 31; i >= 0; i--) {
                int bit = (num >> i) & 1;
                if (!trie[node][bit]) {
                    trie[node][bit] = cnt++;
                }
                node = trie[node][bit];
            }
        }

        // 查询
        int maxXor = 0;
        for (int num : nums) {
            int node = 0;
            int curXor = 0;
            for (int i = 31; i >= 0; i--) {
                int bit = (num >> i) & 1;
                int want = 1 - bit;
                if (trie[node][want]) {
                    curXor |= (1 << i);
                    node = trie[node][want];
                } else {
                    node = trie[node][bit];
                }
            }
            maxXor = max(maxXor, curXor);
        }
        return maxXor;
    }
};

/*
 * 易错点总结:
 * 1. Trie 必须从高位到低位构建 (bit 31 → bit 0)
 *    从低位开始会导致贪心方向错误（先确定高位更重要）
 * 2. 查询时 children[bit] 一定存在（当前数自己的路径在 Trie 中）
 *    所以 else 分支不需要判空
 * 3. 1 << 31 在 C++ 中可能导致未定义行为（signed overflow）
 *    安全做法：用 1u << 31 或从 bit 30 开始
 * 4. 哈希表方法中前缀的取法：用 mask = ~0 << i 保留高位
 *
 * 面试追问:
 * Q: Trie 的空间优化?
 *    → 数组实现（解法3），或者只在查询时边插边查
 * Q: 如果增加约束 nums[j] <= m?
 *    → LC 1707，Trie + 排序 + 离线查询
 * Q: 时间复杂度为什么是 O(n) 而不是 O(n*32)?
 *    → O(32n) = O(n)，32 是常数（int 位数）
 * Q: 能否 O(1) 空间?
 *    → 目前已知方法都需要 O(n) 空间（Trie 或 HashSet）
 */

// ============================================================
// 总结区块
// ============================================================
/*
 * 【解法对比】
 * | 解法            | 时间     | 空间     | 特点                     |
 * |----------------|----------|----------|--------------------------|
 * | Trie 指针(解法1)| O(32n)  | O(32n)   | 面试首选，思路直观         |
 * | 哈希表(解法2)   | O(32n)  | O(n)     | 空间略优，但实现易错       |
 * | Trie 数组(解法3)| O(32n)  | O(32n)   | cache-friendly，竞赛常用  |
 *
 * 面试首选: 解法1(Trie 指针)。结构清晰，面试官容易理解。
 * 竞赛场景: 解法3(Trie 数组) 更快，避免 new/delete 开销。
 *
 * 【易错点清单】
 * 1. 遍历方向: 必须从 bit 31 → bit 0（高位到低位），
 *    因为高位的 1 价值 > 低位所有 1 之和
 * 2. 1 << 31 溢出: signed int 左移 31 位是 UB，
 *    安全写法: 1u << 31 或只遍历 bit 30 → bit 0
 * 3. Trie 查询中 else 分支: 不需要判空！
 *    因为当前数自己的路径一定在 Trie 中
 * 4. 哈希表解法中前缀掩码: 用 ~0 << i 保留高位，
 *    不能用 (1 << i) - 1（那是低位掩码）
 * 5. 内存泄漏: 指针 Trie 面试中可以不管，但提一句加分
 *
 * 【面试追问链】
 * Q1: 如何求数组中两个数的最大 XOR？（本题）
 *     → Trie 贪心：每一位尽量走相反方向
 * Q2: 如果加约束 nums[j] <= m，怎么办？（LC 1707）
 *     → 离线查询：按 m 排序，边插 Trie 边查询
 *       保证查询时 Trie 中只有 <= m 的数
 * Q3: 如果求子数组异或和的最大值？（LC 1938 / CF）
 *     → 前缀异或 + Trie：prefix[i] ^ prefix[j] 的最大值
 *       等价于 Trie 上两个前缀异或的最大值
 */
