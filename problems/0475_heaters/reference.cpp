/*
 * LeetCode 475: 供暖器 (Heaters)
 *
 * 【题目本质】
 * 找最小半径 r，使得每栋房屋到最近供暖器的距离 ≤ r。
 * 即 ans = max over all houses of (min distance to any heater)。
 *
 * 【解法总览】
 * 解法1: 暴力枚举           — O(m×n) / O(1)       — 最直觉
 * 解法2: 排序+二分查找      — O(m log n + n log n) / O(log n) — 面试首选
 * 解法3: 排序+双指针        — O(m log m + n log n) / O(log n) — 同样推荐
 * 解法4: 二分答案+贪心判定  — O((m+n) log V) / O(log n) — 通用思想加分项
 */

// ============================================================
// 解法1: 暴力枚举
// 时间: O(m×n)  空间: O(1)
//
// 【思路】
// 最直觉的做法：对每栋房屋，遍历所有供暖器找最近的。
// 所有房屋的"最近距离"中取最大值就是答案。
// 瓶颈：对每栋房屋都要线性扫描所有供暖器。
// ============================================================
class Solution1 {
public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        int ans = 0;
        for (int house : houses) {
            int minDist = INT_MAX;
            for (int heater : heaters) {
                minDist = min(minDist, abs(house - heater));
            }
            // 所有房屋中，离最近供暖器最远的那个决定了半径
            ans = max(ans, minDist);
        }
        return ans;
    }
};

// ============================================================
// 解法2: 排序 + 二分查找 — 面试首选
// 时间: O(m log n + n log n)  空间: O(log n) (排序栈空间)
//
// 【思路】
// 解法1的瓶颈：对每栋房屋线性找最近供暖器 O(n)。
// 突破：排序供暖器后，用 lower_bound 二分找到 >= house 的第一个供暖器，
// 最近的要么是它，要么是它左边一个，比较两者取较小距离。
//
// 为什么正确：排序后二分保证不遗漏任何供暖器，
// 而 >= house 的第一个和 < house 的最后一个一定包含最近点。
//
// 二分查找示例:
// heaters(sorted) = [1, 4, 8], house = 5
//
// lower_bound(5) → 指向 8（第一个 >= 5）
// distRight = 8 - 5 = 3
// 左邻居 = 4
// distLeft = 5 - 4 = 1
// minDist = min(3, 1) = 1
// ============================================================
class Solution2 {
public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        sort(heaters.begin(), heaters.end());
        int ans = 0;
        
        for (int house : houses) {
            // 找第一个 >= house 的供暖器
            auto it = lower_bound(heaters.begin(), heaters.end(), house);
            
            int distRight = INT_MAX, distLeft = INT_MAX;
            
            // 右侧候选（>= house 的第一个供暖器）
            if (it != heaters.end()) {
                distRight = *it - house;
            }
            
            // 左侧候选（< house 的最后一个供暖器）
            // 易错：必须检查两侧！只看 lower_bound 结果会漏掉左边更近的
            if (it != heaters.begin()) {
                --it;
                distLeft = house - *it;
            }
            
            int minDist = min(distLeft, distRight);
            ans = max(ans, minDist);
        }
        return ans;
    }
};

// ============================================================
// 解法3: 排序 + 双指针
// 时间: O(m log m + n log n)  空间: O(log n)
//
// 【思路】
// 房屋和供暖器都排序后，从左到右扫描：
// 对当前房屋，比较当前供暖器和下一个供暖器谁更近。
// 如果下一个更近（或等距），指针右移。
//
// 为什么指针不回退：房屋有序，后面的房屋位置 >= 当前，
// 不可能需要更左边的供暖器。
//
// 双指针过程:
// houses(sorted) = [1, 2, 3, 4], heaters(sorted) = [1, 4]
//
// i=0(house=1): j=0(heater=1), |1-1|=0
//   next heater=4, |4-1|=3 > 0 → 不移 j
//   dist=0, ans=0
//
// i=1(house=2): j=0(heater=1), |1-2|=1
//   next heater=4, |4-2|=2 > 1 → 不移 j
//   dist=1, ans=1
//
// i=2(house=3): j=0(heater=1), |1-3|=2
//   next heater=4, |4-3|=1 <= 2 → j=1
//   dist=1, ans=1
//
// i=3(house=4): j=1(heater=4), |4-4|=0
//   无下一个 → 不移 j
//   dist=0, ans=1
// ============================================================
class Solution3 {
public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        sort(houses.begin(), houses.end());
        sort(heaters.begin(), heaters.end());
        
        int ans = 0;
        int j = 0;
        
        for (int i = 0; i < (int)houses.size(); i++) {
            // 贪心：如果下一个供暖器离当前房屋更近/等距，移动指针
            // 易错：比较距离必须用 abs，因为供暖器可能在房屋左侧
            while (j + 1 < (int)heaters.size() &&
                   abs(heaters[j + 1] - houses[i]) <= abs(heaters[j] - houses[i])) {
                j++;
            }
            ans = max(ans, abs(heaters[j] - houses[i]));
        }
        return ans;
    }
};

// ============================================================
// 解法4: 二分答案 + 贪心判定
// 时间: O((m + n) × log(max_val))  空间: O(log n)
//
// 【思路】
// 换角度：不直接算半径，而是猜一个半径 r，检验可行性。
// 半径越大越容易覆盖 → 单调性 → 可以二分。
//
// 判定函数 canCover(r):
//   房屋和供暖器都排序后，用双指针贪心检查：
//   对每栋房屋，跳过覆盖不到它的供暖器，
//   看当前供暖器能否覆盖它。
//
// 二分搜索空间收缩:
// houses=[1,5], heaters=[2]
//
// lo=0, hi=3
// Round1: mid=1, canCover(1)? heater=2覆盖[1,3], house=5✗ → false, lo=2
// Round2: mid=2, canCover(2)? heater=2覆盖[0,4], house=5✗ → false, lo=3
// lo==hi==3 → ans=3
// ============================================================
class Solution4 {
public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        sort(houses.begin(), houses.end());
        sort(heaters.begin(), heaters.end());
        
        // 二分范围：最小0，最大是最远房屋到最远供暖器的距离
        int lo = 0;
        int hi = max(
            abs(houses.back() - heaters.front()),
            abs(houses.front() - heaters.back())
        );
        
        // 找满足 canCover 的最小 r → 用 left < right 模板
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (canCover(houses, heaters, mid)) {
                hi = mid;     // mid 可行，尝试更小
            } else {
                lo = mid + 1; // mid 不够，需要更大
            }
        }
        return lo;
    }
    
private:
    bool canCover(vector<int>& houses, vector<int>& heaters, int r) {
        int j = 0;
        for (int i = 0; i < (int)houses.size(); i++) {
            // 跳过无法覆盖 houses[i] 的供暖器
            // heaters[j] + r < houses[i] 意味着供暖器太靠左，够不着
            while (j < (int)heaters.size() && heaters[j] + r < houses[i]) {
                j++;
            }
            // 所有供暖器用完了，或者当前供暖器太靠右也够不着
            if (j >= (int)heaters.size() || heaters[j] - r > houses[i]) {
                return false;
            }
            // houses[i] 被 heaters[j] 覆盖，继续下一栋
            // 注意 j 不递增——同一个供暖器可能覆盖多栋房屋
        }
        return true;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 暴力: 简单但 O(m×n)，大数据超时
// 解法2 二分: 只排序供暖器，对每栋房屋独立二分，无需房屋有序
// 解法3 双指针: 需同时排序两个数组，代码最简洁，一次遍历
// 解法4 二分答案: 体现通用"最小化最大值"思想，面试中能展示思维深度
//
// 面试推荐：先说暴力思路，然后写解法2或3。
// 如果面试官追问"还有别的方法吗"，提出解法4。
//
// 【易错点】
// 1. 解法2只看 lower_bound 右侧，忘记检查左邻居
//    ✗ 只用 *it - house 作为距离
//    ✓ 必须 min(右侧距离, 左侧距离)
//
// 2. 解法3双指针比较距离时没用 abs
//    ✗ heaters[j+1] - houses[i] <= heaters[j] - houses[i]
//    ✓ abs(heaters[j+1] - houses[i]) <= abs(heaters[j] - houses[i])
//    供暖器在房屋左侧时差值为负，不加 abs 结果完全错误
//
// 3. 解法4二分上界不够大
//    ✗ hi = max element  (没考虑跨越的距离)
//    ✓ hi = max(|houses末 - heaters首|, |houses首 - heaters末|)
//
// 4. 忘记排序
//    解法2必须排序 heaters；解法3/4必须两个都排序
//    不排序会导致二分查找/双指针/贪心逻辑全部失效
//
// 【面试追问】
// Q1: 这道题的本质？
//     → ans = max_i(min_j |houses[i] - heaters[j]|)
//
// Q2: 暴力瓶颈在哪？怎么优化？
//     → 线性查找最近供暖器 O(n) → 排序后二分 O(log n)
//
// Q3: 如果变成"任意放 k 个供暖器求最小半径"？
//     → 二分答案 r，判定：排序房屋，贪心放供暖器
//        第一个未覆盖的房屋 +r 处放一个，看 k 个够不够
//
// Q4: 坐标范围到 10^18？
//     → 算法不变，注意用 long long 防溢出，
//        二分答案的 log(10^18) ≈ 60，仍然高效
// ============================================================
