/*
 * LeetCode 810: 黑板异或游戏 (Chalkboard XOR Game)
 *
 * 【题目本质】
 * 博弈论 + 位运算脑筋急转弯。
 * Alice 赢当且仅当: 初始 XOR = 0 或 数组长度为偶数。
 * 面试重点在于证明过程而非代码。
 *
 * 【解法总览】
 * 解法1: 数学结论 — O(n) / O(1) — 唯一解法
 *
 * 证明核心：反证法 —— 偶数个元素时，当前玩家总能找到安全删除。
 */

// ============================================================
// 解法1: 数学结论
// 时间: O(n)  空间: O(1)
//
// 【结论】
// Alice 赢 ⟺ xorAll == 0 || n % 2 == 0
//
// 【证明】
// Case 1: xorAll == 0
//   Alice 回合开始时 XOR = 0，按题目规则 Alice 直接赢。
//
// Case 2: xorAll != 0 且 n 为偶数 → Alice 必胜
//
//   反证法：假设 Alice 无论删哪个 nums[i]，剩余 XOR 都 = 0。
//
//   XOR(剩余) = xorAll ^ nums[i] = 0
//   ⟹ nums[i] = xorAll，对所有 i 成立
//   ⟹ 所有元素都等于 xorAll
//   ⟹ xorAll = xorAll ^ xorAll ^ ... (n 次)
//            = 0 (因为 n 是偶数，偶数个相同值 XOR = 0)
//   ⟹ 矛盾！(我们假设 xorAll != 0)
//
//   所以 Alice 一定能找到某个 nums[i] 使删除后 XOR != 0。
//
//   Alice 删除后：轮到 Bob，他面对 n-1 (奇数) 个元素且 XOR != 0。
//   Bob 删除后：轮到 Alice，她面对 n-2 (偶数) 个元素。
//   如果此时 XOR != 0，同样的反证论证保证 Alice 有安全选择。
//   如果 XOR = 0，Alice 直接赢。
//
//   → Alice 始终面对偶数个元素，每次都能安全操作或直接赢。
//
// Case 3: xorAll != 0 且 n 为奇数 → Bob 必胜
//
//   Alice 面对奇数个元素，反证法的"偶数 XOR 消除"不成立。
//   Alice 可能被迫让 XOR 变 0。
//   Bob 面对偶数个元素，享受 Case 2 的优势。
//
// 验证:
// [1,1,2]  XOR=2, n=3(奇数) → false ✓
// [0,1]    XOR=1, n=2(偶数) → true  ✓
// [1,2,3]  XOR=0            → true  ✓
// ============================================================
class Solution1 {
public:
    bool xorGame(vector<int>& nums) {
        int xorAll = 0;
        for (int num : nums) {
            xorAll ^= num;
        }

        // Alice 赢: 初始 XOR=0 (直接赢) 或 长度偶数 (有必胜策略)
        return xorAll == 0 || nums.size() % 2 == 0;
    }
};

// ============================================================
// 解法1 展开版: 用 accumulate 简化 XOR 计算
// ============================================================
class Solution1_Variant {
public:
    bool xorGame(vector<int>& nums) {
        // 用 STL 的 accumulate + XOR 作为二元操作符
        int xorAll = accumulate(nums.begin(), nums.end(), 0,
                                [](int a, int b) { return a ^ b; });
        // 或者直接用 bit_xor<int>{}
        // int xorAll = accumulate(nums.begin(), nums.end(), 0, bit_xor<int>());

        return xorAll == 0 || nums.size() % 2 == 0;
    }
};

// ============================================================
// 【证明的另一种表述 (交换论证)】
//
// 当 n 为偶数且 xorAll != 0 时:
//
// 设 x_i = xorAll ^ nums[i] (删除 nums[i] 后的 XOR)
//
// 所有 x_i 的 XOR:
//   x_0 ^ x_1 ^ ... ^ x_{n-1}
//   = (xorAll ^ nums[0]) ^ (xorAll ^ nums[1]) ^ ... ^ (xorAll ^ nums[n-1])
//   = xorAll^n  ^  (nums[0] ^ nums[1] ^ ... ^ nums[n-1])
//   = xorAll^n  ^  xorAll
//
// 当 n 为偶数: xorAll^n = 0
//   → x_0 ^ x_1 ^ ... ^ x_{n-1} = 0 ^ xorAll = xorAll != 0
//   → 不可能所有 x_i 都为 0 (否则它们的 XOR 也为 0)
//   → 存在某个 i 使 x_i != 0
//   → Alice 可以安全删除 nums[i]  ■
//
// 【易错点】
// 1. 忘记 xorAll == 0 的判断:
//    ✗ return nums.size() % 2 == 0
//    ✓ return xorAll == 0 || nums.size() % 2 == 0
//    反例: [1,2,3] XOR=0, n=3(奇数), Alice 仍然赢
//
// 2. 偶数/奇数搞反:
//    ✗ return ... || nums.size() % 2 == 1
//    ✓ Alice 先手面对 n 个元素，n 偶数时她有优势
//
// 3. 尝试暴力模拟博弈:
//    ✗ 枚举所有删除顺序 → O(n!) TLE
//    ✓ 直接数学结论 O(n)
//
// 4. nums.size() 返回 size_t (无符号):
//    ✗ 用 int n = nums.size() 可能有警告
//    ✓ 直接用 nums.size() % 2 == 0，或 (int)nums.size()
//
// 【面试追问】
// Q1: 用更直观的方式解释？
//     (偶数方总有安全选择 → Alice 永不被逼入绝境)
// Q2: 规则反转(删后XOR=0则赢)怎么办？
//     (需要找到 nums[i] = xorAll 的元素才能赢，更复杂)
// Q3: 推广到 k 人博弈？
//     (需要 Sprague-Grundy 定理，偶数消除性质不再直接适用)
// ============================================================
