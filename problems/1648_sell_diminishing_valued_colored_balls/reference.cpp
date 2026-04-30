/*
 * LeetCode 1648: 销售价值减少的颜色球
 *
 * 【题目本质】
 * 贪心：每次卖当前最贵的球（库存最多的颜色）。
 * 核心难点不在"策略"而在"效率"——如何批量计算避免逐个模拟。
 *
 * 【解法总览】
 * 解法1: 大顶堆模拟   — O(orders × log n) / O(n) — 直觉但超时
 * 解法2: 排序 + 分层削减 — O(n log n) / O(1)   — 面试首选
 * 解法3: 二分查找阈值   — O(n log M) / O(1)     — 面试首选
 */

// ============================================================
// 解法1: 大顶堆模拟 — 每次卖最贵的球
// 时间: O(orders × log n)   空间: O(n)
//
// 【思路】
// 最直觉的贪心：维护大顶堆，每次弹出最大值（最贵的颜色），
// 卖掉一个（价值=当前库存数），库存-1后放回堆。
// 
// 为什么超时？orders 最大 10^9，每次只卖1个，堆操作 O(log n)。
// 10^9 × log(10^5) ≈ 1.7×10^10，远超时间限制。
// 但这个解法验证了贪心策略的正确性，是优化的起点。
// ============================================================
class Solution1 {
public:
    int maxProfit(vector<int>& inventory, int orders) {
        const int MOD = 1e9 + 7;
        priority_queue<int> pq(inventory.begin(), inventory.end());
        long long ans = 0;
        
        while (orders > 0) {
            int top = pq.top();
            pq.pop();
            ans = (ans + top) % MOD;
            if (top - 1 > 0) pq.push(top - 1);
            orders--;
        }
        return (int)ans;
    }
};

// ============================================================
// 解法2: 排序 + 贪心分层削减 — 批量处理同层球
// 时间: O(n log n)   空间: O(1)（原地排序）
//
// 【思路】
// 解法1的瓶颈：逐个卖球太慢。
// 观察：排序后从最高库存开始，每次可以把一批颜色"削平"到下一个水位。
// 例如 [10, 8, 6, 4, 2]，先把10削到8（1种颜色，卖10+9=19），
// 再把两个8削到6（2种颜色，各卖8+7=15，共30）...
//
// 这样每层只需 O(1) 计算（等差求和），总共最多 n 层 → O(n)。
// 加上排序的 O(n log n)，总时间 O(n log n)。
//
// 可视化（inventory = [10, 8, 6, 4, 2], orders = 20）:
//
//  10 |X|                     层1: width=1, 从10削到8, 卖2个
//   9 |X|
//   8 |X|X|                   层2: width=2, 从8削到6, 卖4个
//   7 |X|X|
//   6 |X|X|X|                 层3: width=3, 从6削到4, 卖6个
//   5 |X|X|X|
//   4 |X|X|X|X|               层4: width=4, 从4削到2, 卖8个
//   3 |X|X|X|X|               累计: 2+4+6+8=20 = orders ✓
//   2 |X|X|X|X|X|
//      c1 c2 c3 c4 c5
// ============================================================
class Solution2 {
public:
    int maxProfit(vector<int>& inventory, int orders) {
        const long long MOD = 1e9 + 7;
        // 2 在 MOD 下的模逆元，因为 2 × 500000004 ≡ 1 (mod 10^9+7)
        const long long INV2 = 500000004;
        
        sort(inventory.begin(), inventory.end(), greater<int>());
        int n = inventory.size();
        long long ans = 0;
        int i = 0;
        
        while (orders > 0) {
            long long cur = inventory[i];
            // 下一个不同的水位（最后一种颜色的下方是0）
            long long next = (i + 1 < n) ? inventory[i + 1] : 0;
            // 有 (i+1) 种颜色都处于 cur 这个水位或更高
            long long width = i + 1;
            long long height = cur - next;
            long long canSell = width * height;
            
            if (canSell <= orders) {
                // 整层都能卖完
                // 每种颜色从 cur 卖到 next+1:
                // 单种颜色价值 = cur + (cur-1) + ... + (next+1)
                //              = (cur + next + 1) * (cur - next) / 2
                long long singleSum = (cur + next + 1) % MOD * (height % MOD) % MOD * INV2 % MOD;
                ans = (ans + singleSum * (width % MOD)) % MOD;
                orders -= (int)canSell;
                i++;  // 进入下一层
            } else {
                // 不能整层削完，需要精确分配
                long long fullRows = orders / width;    // 能完整削几行
                long long remainder = orders % width;   // 剩余零散的球
                
                if (fullRows > 0) {
                    // 每种颜色从 cur 卖到 (cur - fullRows + 1)
                    long long bottom = cur - fullRows + 1;
                    long long singleSum = (cur + bottom) % MOD * (fullRows % MOD) % MOD * INV2 % MOD;
                    ans = (ans + singleSum * (width % MOD)) % MOD;
                }
                // 剩余 remainder 个球，每个价值 (cur - fullRows)
                // 为什么是 cur - fullRows？因为已经卖到 (cur - fullRows + 1) 了，
                // 再往下卖一个的价值就是 (cur - fullRows)
                long long leftoverVal = (cur - fullRows) % MOD;
                ans = (ans + remainder % MOD * leftoverVal) % MOD;
                orders = 0;
            }
        }
        
        return (int)((ans % MOD + MOD) % MOD);
    }
};

// ============================================================
// 解法3: 二分查找阈值 — 找最终"水位线"后批量求和
// 时间: O(n log M)   空间: O(1)，M = max(inventory)
//
// 【思路】
// 换个角度想：最终状态是所有颜色的库存都被"削"到某个 threshold 以下。
// 我们直接二分找这个 threshold：
//   - 给定 threshold，能卖的球数 = Σ max(0, inventory[i] - threshold)
//   - 如果能卖 >= orders，threshold 可以更高（少卖点）
//   - 如果能卖 < orders，threshold 要更低（多卖点）
//
// 二分搜索空间收缩过程 (inventory=[2,8,4,10,6], orders=20):
//
// lo=0, hi=10
// Round1: mid=5, canSell=(10-5)+(8-5)+(6-5)=9 < 20  → lo=5 太少
//   等等，canSell <= orders 时应该 hi=mid
//   重新理解：我们找的是 "使 canSell >= orders 的最大 threshold"
//
// 实际用 "lo < hi" 模板：
// lo=0, hi=10
// mid=5: canSell=5+3+1=9 <= 20 → hi=5
// lo=0, hi=5
// mid=2: canSell=8+6+4+2=20 <= 20 → hi=2
// lo=0, hi=2
// mid=1: canSell=9+7+5+3+1=25 > 20 → lo=2
// lo=2, hi=2 → 结束, threshold=2
//
// 然后：
// 所有 >2 的部分: (10→3)+(8→3)+(6→3)+(4→3) = 8+6+4+2 = 20 个
// 刚好 = orders，不需要在 threshold 层额外卖
// 价值 = Σ等差求和 = 52+33+18+7 = 110 ✓
// ============================================================
class Solution3 {
public:
    int maxProfit(vector<int>& inventory, int orders) {
        const long long MOD = 1e9 + 7;
        const long long INV2 = 500000004;
        int n = inventory.size();
        
        // ---- 第一步: 二分找 threshold ----
        // threshold 含义: 所有颜色的库存最终削到 threshold
        // 我们要找最大的 threshold 使得能卖的球 >= orders
        long long lo = 0, hi = *max_element(inventory.begin(), inventory.end());
        
        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            
            // check: 如果 threshold=mid，总共能卖多少个球？
            long long canSell = 0;
            for (int x : inventory) {
                if (x > mid) canSell += x - mid;
            }
            
            if (canSell <= (long long)orders) {
                // 卖得不够或刚好够 → threshold 可以更低以卖更多
                // 但 mid 本身可能是答案（刚好够时），所以 hi = mid 不排除 mid
                hi = mid;
            } else {
                // 卖得太多了 → threshold 应该更高
                lo = mid + 1;
            }
        }
        
        long long threshold = lo;
        
        // ---- 第二步: 计算总价值 ----
        // 先把所有严格 > threshold 的部分算掉
        long long ans = 0;
        long long totalSold = 0;
        
        for (int x : inventory) {
            if (x > threshold) {
                long long high = x;
                long long low = threshold + 1;  // 只卖到 threshold+1，不包含 threshold
                long long count = high - low + 1;
                
                // 等差求和: high + (high-1) + ... + low = (high + low) * count / 2
                long long sum = (high + low) % MOD * (count % MOD) % MOD * INV2 % MOD;
                ans = (ans + sum) % MOD;
                totalSold += count;
            }
        }
        
        // 可能 totalSold < orders，差的部分每个价值恰好是 threshold
        // 为什么？因为已经卖到 threshold+1 了，再往下一个就是 threshold
        long long remaining = (long long)orders - totalSold;
        ans = (ans + remaining % MOD * (threshold % MOD)) % MOD;
        
        return (int)((ans % MOD + MOD) % MOD);
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 (堆模拟):
//   - 优点: 逻辑简单，容易想到
//   - 缺点: O(orders × log n) 超时，orders 最大 10^9
//   - 适用: 理解贪心正确性、小数据验证
//
// 解法2 (排序+分层):
//   - 优点: 时间 O(n log n)，空间 O(1)，思路直观
//   - 缺点: 分层逻辑的边界处理（最后不足一层时）有些繁琐
//   - 适用: 面试首选，好讲好理解
//
// 解法3 (二分阈值):
//   - 优点: 时间 O(n log M)，逻辑干净（二分 + 求和分离）
//   - 缺点: 需要理解"二分答案"的范式
//   - 适用: 面试首选，代码更简洁，思路更通用
//
// 面试建议: 解法2和解法3择一即可。解法2更容易画图讲解，
//           解法3更体现"二分答案"这个通用技巧。
//
// ============================================================
// 【易错点】
//
// 1. 整数溢出:
//    ✗ int sum = (high + low) * count / 2;  // 10^9 × 10^9 溢出 int 甚至 long long
//    ✓ 全程 long long，每步乘法后立即 % MOD
//
// 2. 除以2的取模:
//    ✗ (a * b / 2) % MOD  // 取模后除法没有意义
//    ✓ (a * b % MOD) * INV2 % MOD  // 乘以2的模逆元
//    注: INV2 = 500000004，因为 2 × 500000004 ≡ 1 (mod 10^9+7)
//
// 3. 二分方向搞反:
//    ✗ canSell > orders 时 hi = mid  // 卖多了应该提高threshold
//    ✓ canSell > orders 时 lo = mid + 1
//    ✗ canSell <= orders 时 lo = mid + 1  // 可能跳过恰好满足的答案
//    ✓ canSell <= orders 时 hi = mid
//
// 4. 忘记处理 remaining:
//    二分找到 threshold 后，Σ(inventory[i] - threshold) 可能 > orders
//    （因为二分找的是使 canSell >= orders 的最大 threshold）
//    必须只卖 orders 个，多的部分通过 remaining 在 threshold 层精确补齐
//
// 5. 解法2中 i 越界:
//    当所有颜色库存相同时 i 会走到 n-1，
//    此时 next 应该为 0，不检查 i+1 < n 就会越界
//
// ============================================================
// 【面试追问】
//
// Q1 (基础): 为什么贪心（每次卖最贵的）一定最优？
// → 交换论证：如果某步不卖最贵的 A 而卖便宜的 B，
//   交换后总价值 = 原来 - b + a > 原来（因为 a > b），所以贪心不劣。
//
// Q2 (优化): 等差求和时为什么要用模逆元而不是直接除？
// → 在模运算下除法没有意义，(a/b) % m ≠ (a%m) / (b%m)。
//   需要乘以 b 的模逆元，即 b^(-1) mod m = b^(m-2) mod m（费马小定理）。
//   2 的逆元预计算为 500000004。
//
// Q3 (变体): 如果球的价值不是线性递减（如每卖一个降2），怎么改？
// → 等差求和公式的公差从1变成2，首项不变，但"能卖几个"的计算也要变：
//   从 inventory[i] 到 threshold 之间能卖 ceil((inventory[i]-threshold)/2) 个球。
//   二分+求和的框架不变，只是具体公式调整。
// ============================================================
