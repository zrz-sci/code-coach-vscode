/*
 * LeetCode 299: Bulls and Cows (猜数字游戏)
 *
 * 【题目本质】
 * 计数匹配问题：先统计"位置和数字都对"的(Bulls)，
 * 再统计"数字对但位置不对"的(Cows)，处理重复数字不多算。
 *
 * 【解法总览】
 * 解法1: 两次遍历 + 频率数组 — O(n) / O(1) — 最直觉，面试首选
 * 解法2: 一次遍历 + 正负计数 — O(n) / O(1) — 更精巧，追问时给出
 */

// ============================================================
// 解法1: 两次遍历 + 频率数组
// 时间: O(n)  空间: O(1)  (计数数组固定大小10)
//
// 【思路】
// 最自然的想法：
// 第一遍：找出所有 Bulls（位置和数字都对的）。
//         对于非 Bull 的位置，分别统计 secret 和 guess 的数字频率。
// 第二遍：对 0-9 每个数字，Cow 贡献 = min(secret频率, guess频率)。
//
// 为什么取 min？因为配对需要双方各出一个，多出的那边没人配。
//
// 示例: secret = "1123", guess = "0111"
//
//   位置:    0     1     2     3
//   secret:  1     1     2     3
//   guess:   0     1     1     1
//   匹配?    ✗    Bull   ✗     ✗
//
//   非Bull位置:
//     secret: {1, 2, 3} → freq_s: {1:1, 2:1, 3:1}
//     guess:  {0, 1, 1} → freq_g: {0:1, 1:2}
//
//   Cow = min(1,2)[数字1] + min(1,0)[数字2] + min(1,0)[数字3]
//       + min(0,1)[数字0] = 1
//
//   结果: "1A1B"
// ============================================================
class Solution1 {
public:
    string getHint(string secret, string guess) {
        int bulls = 0, cows = 0;
        int freq_s[10] = {0};  // secret 中非 Bull 位置的数字频率
        int freq_g[10] = {0};  // guess  中非 Bull 位置的数字频率

        for (int i = 0; i < (int)secret.size(); i++) {
            if (secret[i] == guess[i]) {
                // 位置和数字都对 → Bull
                bulls++;
            } else {
                // 非 Bull 位置才统计频率，避免 Bull 位置被重复计入 Cow
                freq_s[secret[i] - '0']++;
                freq_g[guess[i] - '0']++;
            }
        }

        // 对每个数字 0-9，Cow 的贡献 = 双方都有的最小值
        for (int d = 0; d < 10; d++) {
            cows += min(freq_s[d], freq_g[d]);
        }

        return to_string(bulls) + "A" + to_string(cows) + "B";
    }
};

// ============================================================
// 解法2: 一次遍历 + 正负计数
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法1 需要一次遍历 + 一次 0-9 循环。虽然实际差异极小，
// 但面试官可能追问"能否一次遍历解决？"
//
// 核心技巧：用一个 count[10] 数组，正负含义不同：
//   count[d] > 0: secret 中数字 d 的"供给"多于 guess 的"需求"
//   count[d] < 0: guess 中数字 d 的"需求"多于 secret 的"供给"
//   count[d] = 0: 供需平衡
//
// 处理非 Bull 位置 (i) 时：
//   - 看 secret[i] 的数字 s: 在 count[s]++ 之前，如果 count[s] < 0，
//     说明之前 guess 需求过 s 但没配上，现在 secret 供给了一个 → cows++
//   - 看 guess[i] 的数字 g: 在 count[g]-- 之前，如果 count[g] > 0，
//     说明之前 secret 供给过 g 但没被消耗，现在 guess 需要它 → cows++
//
// 一次遍历演示: secret = "1807", guess = "7810"
//
// count[0..9] = {0,0,0,0,0,0,0,0,0,0}
//
// i=0: s=1, g=7, 不匹配
//      count[1]=0, 不<0           → count[1]++ → count[1]=1
//      count[7]=0, 不>0           → count[7]-- → count[7]=-1
//      bulls=0, cows=0
//
// i=1: s=8, g=8, 匹配! bulls=1
//
// i=2: s=0, g=1, 不匹配
//      count[0]=0, 不<0           → count[0]++ → count[0]=1
//      count[1]=1, >0, cows++!    → count[1]-- → count[1]=0
//      bulls=1, cows=1
//
// i=3: s=7, g=0, 不匹配
//      count[7]=-1, <0, cows++!   → count[7]++ → count[7]=0
//      count[0]=1, >0, cows++!    → count[0]-- → count[0]=0
//      bulls=1, cows=3
//
// 结果: "1A3B" ✓
// ============================================================
class Solution2 {
public:
    string getHint(string secret, string guess) {
        int bulls = 0, cows = 0;
        int count[10] = {0};  // 正=secret供给盈余, 负=guess需求盈余

        for (int i = 0; i < (int)secret.size(); i++) {
            int s = secret[i] - '0';
            int g = guess[i] - '0';

            if (s == g) {
                bulls++;
            } else {
                // 关键：先判断再修改，顺序不能反
                // secret 提供了数字 s，检查 guess 之前是否"欠"这个数字
                if (count[s] < 0) cows++;
                // guess 需要数字 g，检查 secret 之前是否"多"这个数字
                if (count[g] > 0) cows++;

                count[s]++;  // secret 供给 +1
                count[g]--;  // guess 需求 +1 (体现为 count 减少)
            }
        }

        return to_string(bulls) + "A" + to_string(cows) + "B";
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 解法1: 两次遍历      | 解法2: 一次遍历       |
// |-------------|---------------------|---------------------|
// | 遍历次数     | 1次 + 10次小循环     | 1次                 |
// | 代码复杂度   | 简单直觉             | 需理解正负计数技巧    |
// | 实际性能     | 几乎相同             | 常数略优             |
// | 面试建议     | 首选，清晰不易错      | 追问优化时给出        |
//
// 两种解法时间和空间复杂度完全相同：O(n) / O(1)。
// 性能差异可以忽略（第二个循环只有10次迭代）。
// 面试中建议先写解法1展示清晰思路，被追问时再给解法2展示技巧。
//
// ============================================================
// 【易错点】
//
// 1. Bull 位置参与 Cow 统计导致重复计算
//    ✗ 先统计全部频率，再减去 Bulls → 重复数字场景下容易减错
//    ✓ 在 else 分支(非Bull)中才统计频率
//
// 2. 解法2中先修改 count 再判断
//    ✗ count[s]++; if (count[s] < 0) cows++;  // 已经+1了，判断条件变了
//    ✓ if (count[s] < 0) cows++; count[s]++;   // 先判断再修改
//
// 3. 用 set 代替频率数组导致多算 Cow
//    ✗ 用 set 只记录 secret 中出现哪些数字 → secret="1123",guess="0111"
//       guess 中三个 '1' 都能在 set 中找到，算出 cow=3（实际应为1）
//    ✓ 用频率数组，取 min 保证每个数字按实际剩余次数配对
//
// 4. C++ 字符串拼接类型不匹配
//    ✗ return bulls + "A" + cows + "B";  // int 不能直接和字面量拼接
//    ✓ return to_string(bulls) + "A" + to_string(cows) + "B";
//
// ============================================================
// 【面试追问】
//
// Q1 (基础理解):
//   "为什么 Cow 要对每个数字取 min(freq_s, freq_g)？"
//   → 配对需要双方各出一个。secret 中数字d出现a次(非Bull)，
//     guess 中出现b次(非Bull)，只能配 min(a,b) 对。
//
// Q2 (优化):
//   "能否只用一次遍历？"
//   → 用正负计数：count[d]>0 表示 secret 的供给盈余，
//     count[d]<0 表示 guess 的需求盈余。
//     处理 secret[i] 时若 count[s]<0 说明可以配对，cows++；
//     处理 guess[i] 时若 count[g]>0 同理。详见解法2。
//
// Q3 (变体):
//   "如果字符不限于 '0'-'9'，而是任意 ASCII 字符，怎么改？"
//   → 把 count[10] 改成 count[128] 或 unordered_map<char,int>。
//     算法逻辑不变，只是哈希表/数组大小变了。
//     如果字符集极大，用 unordered_map 更合适（只记录出现过的字符）。
// ============================================================
