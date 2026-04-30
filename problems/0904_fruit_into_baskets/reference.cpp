/*
 * LeetCode 904: 水果成篮 (Fruit Into Baskets)
 *
 * 【题目本质】
 * 在整数数组中找"最多包含2种不同值的最长连续子数组"的长度。
 * 等价于 LeetCode 159 (Longest Substring with At Most Two Distinct Characters)。
 *
 * 【解法总览】
 * 解法1: 暴力枚举       — O(n²) / O(1) — 最直觉，面试中口述即可
 * 解法2: 滑动窗口+哈希表 — O(n)  / O(1) — ⭐ 面试首选，通用模板
 * 解法3: 精简变量版      — O(n)  / O(1) — 加分项，仅适用于K=2
 */

// ============================================================
// 解法1: 暴力枚举 — 对每个起点向右扩展
// 时间: O(n²)  空间: O(1)
//
// 【思路】
// 最直觉的想法：尝试从每棵树开始采摘，向右走直到出现第3种水果。
// 记录所有起点中能获得的最大长度。
//
// 瓶颈分析：
// 每次左端点右移一位后，右端点又从新位置重新扫描。
// 比如 [1,2,1,1,1,1,3]，从 i=0 扫到 i=5 发现3停止；
// 从 i=1 又重新扫到 i=5，这些 [1,2,1,1,1] 的部分完全是重复工作。
//
// 滑动窗口可视化（暴力 vs 优化）:
// 暴力：  i=0: →→→→→→ stop   (扫了6步)
//         i=1:  →→→→→ stop   (扫了5步，其中4步重复)
//         i=2:   →→→→ stop   (扫了4步，其中3步重复)
// 
// 滑窗：  left=0, right→→→→→→ stop
//         left→→→→→, right继续→  (left追上来，right不回退)
// ============================================================
class Solution1 {
public:
    int totalFruit(vector<int>& fruits) {
        int n = fruits.size(), maxLen = 0;
        for (int i = 0; i < n; i++) {
            unordered_set<int> types;
            int j = i;
            while (j < n) {
                types.insert(fruits[j]);
                if (types.size() > 2) break; // 第3种水果出现，停止
                j++;
            }
            maxLen = max(maxLen, j - i);
        }
        return maxLen;
    }
};

// ============================================================
// 解法2: 滑动窗口 + 哈希表 — ⭐ 面试首选
// 时间: O(n)  空间: O(1)（哈希表最多3个key）
//
// 【思路】
// 从暴力解的瓶颈出发：左端点右移时，右端点不需要回退。
// 
// 为什么？因为窗口有单调性：
//   - 右指针扩展 → 种类只增不减
//   - 左指针收缩 → 种类只减不增
// 所以左右指针都只向右移动，总共移动 2n 步 → O(n)。
//
// 用哈希表 count[fruit] 记录窗口内每种水果的出现次数。
// - 右指针进入：count[fruits[right]]++
// - 种类超过2：不断收缩左指针，count[fruits[left]]--
//   计数归零时从 map 中删除该 key → size() 减小
//
// 滑动窗口过程（示例 [3,3,3,1,2,1,1,2,3,3,4]）:
//
//  right=0: [3]              count={3:1}     len=1
//  right=1: [3,3]            count={3:2}     len=2
//  right=2: [3,3,3]          count={3:3}     len=3
//  right=3: [3,3,3,1]        count={3:3,1:1} len=4
//  right=4: [3,3,3,1,2]      count={3:3,1:1,2:1} size=3! 收缩:
//     left=1: count={3:2,1:1,2:1} size=3
//     left=2: count={3:1,1:1,2:1} size=3
//     left=3: count={1:1,2:1}     size=2 ✓  len=2
//  right=5: [1,2,1]          count={1:2,2:1} len=3
//  right=6: [1,2,1,1]        count={1:3,2:1} len=4
//  right=7: [1,2,1,1,2]      count={1:3,2:2} len=5 ← 最大!
//  right=8: [1,2,1,1,2,3]    count={1:3,2:2,3:1} size=3! 收缩:
//     ... 直到只剩2种
//  ... 后续都不超过5
//  答案 = 5
// ============================================================
class Solution2 {
public:
    int totalFruit(vector<int>& fruits) {
        unordered_map<int, int> count; // 水果种类 → 窗口内出现次数
        int left = 0, maxLen = 0;
        
        for (int right = 0; right < (int)fruits.size(); right++) {
            // 右指针扩展：加入新水果
            count[fruits[right]]++;
            
            // 窗口不合法时（种类 > 2），收缩左边界
            // 为什么用 while 不用 if？因为可能需要连续删除多棵同种类的树
            // 才能让某种水果的计数降为 0 并被删除
            while (count.size() > 2) {
                count[fruits[left]]--;
                if (count[fruits[left]] == 0) {
                    count.erase(fruits[left]); // 关键：计数归零才删除 key
                }
                left++;
            }
            
            // 此时窗口 [left, right] 合法（最多2种水果）
            maxLen = max(maxLen, right - left + 1);
        }
        
        return maxLen;
    }
};

// ============================================================
// 解法3: 精简变量版 — 不用哈希表
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 观察：当出现第3种水果时，我们只需要保留 right-1 位置的水果种类
// 和新水果种类，左边界跳到 right-1 位置那种水果最后连续出现的起点。
//
// 举例：[A, A, B, B, B, A, A, C]
//                               ↑ right 遇到 C（第3种）
// 此时 right-1 是 A，A 最后连续出现从 index=5 开始
// 所以 left 跳到 5，窗口变为 [A, A, C]
//
// 维护变量 lastTypeChangePos：最近一次水果种类变化的位置
// 当出现第3种时：left = lastTypeChangePos
// ============================================================
class Solution3 {
public:
    int totalFruit(vector<int>& fruits) {
        int n = fruits.size();
        if (n <= 2) return n;
        
        int maxLen = 1;
        int left = 0;
        int typeA = fruits[0], typeB = -1; // 窗口内的两种水果
        int lastTypeChangePos = 0;         // 最近一次种类变化的位置
        
        for (int right = 1; right < n; right++) {
            if (fruits[right] == typeA || fruits[right] == typeB) {
                // 仍在已有的两种之内，无需处理
            } else {
                // 出现第3种水果
                // 左边界跳到最近一段连续相同水果的起点
                left = lastTypeChangePos;
                typeA = fruits[right - 1]; // 保留紧邻新水果的那种
                typeB = fruits[right];     // 加入新种类
            }
            
            // 更新最近种类变化位置
            if (fruits[right] != fruits[right - 1]) {
                lastTypeChangePos = right;
            }
            
            maxLen = max(maxLen, right - left + 1);
        }
        
        return maxLen;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 通用性 | 代码复杂度 |
// |------|------|------|--------|-----------|
// | 暴力 | O(n²)| O(1) | -      | 低        |
// | 滑窗+哈希| O(n)| O(1)| K种通用 | 中      |
// | 精简变量 | O(n)| O(1)| 仅K=2  | 高        |
//
// 面试首选解法2：代码简洁、思路清晰、容易推广到K种水果。
// 解法3作为追问"能否不用哈希表"的加分回答。
//
// 【推广到 K 种水果】
// 解法2 只需改一行：while (count.size() > 2) → while (count.size() > k)
// 这就是 LeetCode 340 的解法。
//
// ============================================================
// 【易错点】
//
// 1. 忘记 erase 导致种类数永远不减：
//    ✗ count[fruits[left]]--;  left++;
//      → size() 不变，while 死循环或逻辑错误
//    ✓ count[fruits[left]]--;
//      if (count[fruits[left]] == 0) count.erase(fruits[left]);
//      left++;
//
// 2. 用 if 代替 while 收缩窗口：
//    ✗ if (count.size() > 2) { ... left++; }
//      → 只收缩一步，可能种类仍然 > 2
//    ✓ while (count.size() > 2) { ... left++; }
//
// 3. 窗口长度 off-by-one：
//    ✗ maxLen = max(maxLen, right - left);     → 少算1
//    ✓ maxLen = max(maxLen, right - left + 1); → 闭区间长度
//
// 4. 精简变量版忽略 typeB 初始为 -1 的情况：
//    当数组全是同一种水果时，typeB 始终为 -1，
//    如果判断 fruits[right] != typeB 就会误认为出现了第3种。
//    需要特殊处理：typeB == -1 时直接赋值而非触发收缩。
//
// ============================================================
// 【面试追问】
//
// Q1（基础理解）: 暴力解时间复杂度多少？为什么不够好？
//     → O(n²)，因为每个左端点都重新从头扫描右端点。
//        滑动窗口利用了"左指针只右移"的性质，避免重复扫描。
//
// Q2（推广）: 如果有 K 个篮子呢？代码怎么改？
//     → 滑窗+哈希版只改一处：count.size() > 2 → count.size() > k。
//        这就是 LeetCode 340 的原题。时间 O(n)，空间 O(K)。
//
// Q3（变体）: 如果不要求连续采摘（可以跳过某些树），怎么做？
//     → 这变成了"选最多的两种水果"，直接用哈希表统计每种数量，
//        取出现次数最大的两种之和即可，O(n) 时间。
//        关键理解：原题的"连续"约束是滑动窗口存在的意义。
//
// Q4（数据流）: 如果果树是流式输入的，怎么做？
//     → 滑动窗口天然支持流式处理。每次读入一个果树，
//        执行扩展右指针 → 收缩左指针的流程。
//        但需要缓存窗口内的元素（队列/deque），因为收缩时要知道左端的值。
// ============================================================
