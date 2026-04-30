/*
 * 【题目本质】
 * 两个数各出现一次，其余出现两次 —— 全体异或 + 分组异或
 * 关键: a^b 的某一位为1 → 用该位将数组分成两组
 *
 * 【解法总览】
 * 解法1: 位操作分组异或  O(n) / O(1)  ⭐推荐 (满足题目 O(1) 空间要求)
 * 解法2: 哈希表计数      O(n) / O(n)
 */

// ===================== 解法1: 位操作分组异或 =====================
// 三步走:
//   1. 全体异或 → 得到 xorAll = a ^ b
//   2. 提取 xorAll 最低位的 1 → diffBit（a 和 b 在此位不同）
//   3. 按 diffBit 分组异或 → 每组恰好有一个目标数
//
// 为什么能分开?
//   - 成对的数 → 每一位都相同 → 必然在同一组 → 异或抵消
//   - a 和 b → 在 diffBit 位不同 → 分到不同组 → 各自保留
//
// 示例 [1,2,1,3,2,5]:
//   xorAll = 3^5 = 011^101 = 110 (= 6)
//   diffBit = 6 & (-6) = 010 (= 2)
//   组1 (bit2=1): 2,3,2 → 异或得 3
//   组2 (bit2=0): 1,1,5 → 异或得 5
class Solution1 {
public:
    vector<int> singleNumber(vector<int>& nums) {
        // Step 1: 全体异或 → a ^ b
        unsigned int xorAll = 0;
        for (int num : nums) {
            xorAll ^= num;
        }

        // Step 2: 提取最低位的 1
        // 用 unsigned 避免 INT_MIN 取负溢出的 UB
        unsigned int diffBit = xorAll & (-xorAll);

        // Step 3: 分组异或
        int a = 0, b = 0;
        for (int num : nums) {
            if ((unsigned int)num & diffBit) {
                a ^= num;  // diffBit 位为 1 的组
            } else {
                b ^= num;  // diffBit 位为 0 的组
            }
        }

        return {a, b};
    }
};

// ===================== 解法2: 哈希表计数 =====================
// 思路: 统计频次，找出出现 1 次的两个数
// 优点: 逻辑简单；缺点: 不满足 O(1) 空间要求
class Solution2 {
public:
    vector<int> singleNumber(vector<int>& nums) {
        unordered_map<int, int> count;
        for (int num : nums) {
            count[num]++;
        }

        vector<int> res;
        for (auto& [num, cnt] : count) {
            if (cnt == 1) {
                res.push_back(num);
            }
        }
        return res;
    }
};

/*
 * 【解法对比】
 * ┌──────────────────┬──────┬──────┬──────────────────────┐
 * │ 解法             │ 时间 │ 空间 │ 特点                 │
 * ├──────────────────┼──────┼──────┼──────────────────────┤
 * │ 位操作分组异或   │ O(n) │ O(1) │ 满足题目要求，经典   │
 * │ 哈希表计数       │ O(n) │ O(n) │ 简单但不满足 O(1)    │
 * └──────────────────┴──────┴──────┴──────────────────────┘
 *
 * 【易错点】
 * 1. xorAll & (-xorAll) 必须用 unsigned int
 *    原因: 若 xorAll == INT_MIN (0x80000000)，-INT_MIN 溢出是 UB
 * 2. 分组时两组都要异或，不能只算一组然后用 xorAll ^ a 求 b
 *    (虽然后者正确，但分组异或更清晰且同样高效)
 * 3. 理解 x & (-x) 的原理: -x = ~x + 1
 *    例: x = 01100 → ~x = 10011 → ~x+1 = 10100 → x & (-x) = 00100
 *
 * 【面试追问】
 * Q1: 如何推广到 k 个数各出现一次？
 *     → k=1: 直接异或; k=2: 本题; k=3: 更复杂的位运算
 * Q2: 能否不用 unsigned 处理溢出？
 *     → 可以用 xorAll & ~(xorAll - 1)，效果相同
 * Q3: 分组后为什么成对的数在同一组？
 *     → 相同的数每一位都相同，按任何一位分组必然同组
 */
