/*
 * LeetCode 1610: Maximum Number of Visible Points
 * 
 * 【题目本质】
 * 把所有点转化为相对于 location 的极角，在排序后的环形角度数组上
 * 找一个宽度 ≤ angle 的窗口，使包含的点数最多。
 *
 * 【解法总览】
 * 解法1: 暴力枚举 — O(n²) / O(n) — 枚举每个点作为左边界，暴力计数
 * 解法2: 排序+滑动窗口 — O(n log n) / O(n) — 排序+环形展开+双指针 ⭐面试首选
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举左边界，线性扫描计数
// 时间: O(n²)  空间: O(n)
//
// 【思路】
// 最直觉的做法：把每个点转成角度，然后对每个点假设它在视野左边界上，
// 遍历所有其他点看有多少在 [该角度, 该角度+angle] 内。
// 环形用角度差取模处理。
// 
// 瓶颈：每个左边界都要 O(n) 遍历，总共 O(n²)，n=10⁵ 时超时。
// ============================================================
class Solution1 {
public:
    int visiblePoints(vector<vector<int>>& points, int angle, vector<int>& location) {
        int same = 0;
        vector<double> angles;
        
        for (auto& p : points) {
            int dx = p[0] - location[0];
            int dy = p[1] - location[1];
            if (dx == 0 && dy == 0) {
                // 重合点无论怎么转都可见，单独计数
                same++;
                continue;
            }
            double deg = atan2(dy, dx) * 180.0 / M_PI;
            if (deg < 0) deg += 360.0; // 归一化到 [0, 360)
            angles.push_back(deg);
        }
        
        int n = angles.size();
        int maxVisible = 0;
        
        for (int i = 0; i < n; i++) {
            int count = 0;
            for (int j = 0; j < n; j++) {
                double diff = angles[j] - angles[i];
                // 环形处理：如果 diff < 0，说明 j 在 i 的"另一侧"，加 360°
                if (diff < -1e-9) diff += 360.0;
                if (diff <= angle + 1e-9) {
                    count++;
                }
            }
            maxVisible = max(maxVisible, count);
        }
        
        return maxVisible + same;
    }
};

// ============================================================
// 解法2: 排序 + 滑动窗口 — 环形展开 + 双指针
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 解法1 的瓶颈：对每个左边界 O(n) 线性扫描。
// 如果角度排好序，我们用双指针：
//   left 表示视野左边界（某个点的角度）
//   right 向右扩展，直到 angles[right] - angles[left] > angle
//   窗口大小 right - left 就是当前可见点数
// 
// 关键问题：角度是环形的（0° 和 360° 相邻）。
// 解决：把排序后的角度复制一份，每个加 360°，拼接到数组末尾。
// 这样 [300°, 390°(=30°+360)] 就是一个连续窗口，覆盖了跨 0° 的情况。
//
// right 只增不减（外循环 left 从 0 到 n-1，right 从不回退），
// 所以滑动窗口部分 O(n)，瓶颈在排序 O(n log n)。
//
// 角度计算与环形展开示意:
//
//   点的分布 (location = [1,1]):
//                  (2,2) 45°
//                 /
//   location ----→ 正东 0°
//        (1,1)  \
//                (2,1) 0°  (即在正东方向，可能被掉的就是这种)
//
//   排序后角度:    [0, 45, 135, 300]
//   环形展开:      [0, 45, 135, 300, 360, 405, 495, 660]
//                                     ↑ 复制+360
//
//   滑动窗口 (angle=90):
//   left=0 (0°):   right→45°(90≥45✓), →135°(135>90✗), 停. count=2
//   left=1 (45°):  right从2继续→135°(90=90✓), →300°(255>90✗), 停. count=2
//   left=2 (135°): right从3→300°(165>90✗), 停. count=1
//   left=3 (300°): right从3→360°(60≤90✓), →405°(105>90✗), 停. count=2
//   最大 = 2
// ============================================================
class Solution2 {
public:
    int visiblePoints(vector<vector<int>>& points, int angle, vector<int>& location) {
        int same = 0;
        vector<double> angles;
        
        for (auto& p : points) {
            int dx = p[0] - location[0];
            int dy = p[1] - location[1];
            if (dx == 0 && dy == 0) {
                same++;
                continue;
            }
            // atan2(dy, dx) 返回弧度 (-π, π]，转度数后归一化到 [0, 360)
            double deg = atan2((double)dy, (double)dx) * 180.0 / M_PI;
            if (deg < 0) deg += 360.0;
            angles.push_back(deg);
        }
        
        sort(angles.begin(), angles.end());
        
        int n = angles.size();
        // 环形展开：复制一份，每个角度 +360°
        for (int i = 0; i < n; i++) {
            angles.push_back(angles[i] + 360.0);
        }
        
        // 滑动窗口
        // right 在整个循环中只增不减，保证总复杂度 O(n)
        int maxVisible = 0;
        int right = 0;
        for (int left = 0; left < n; left++) {
            // 右指针扩展：只要窗口宽度 ≤ angle 就继续
            while (right < 2 * n && angles[right] - angles[left] <= (double)angle + 1e-9) {
                right++;
            }
            // right - left = 当前窗口内的点数
            maxVisible = max(maxVisible, right - left);
        }
        
        return maxVisible + same;
    }
};

// ============================================================
// 解法2 变体: 用二分查找代替双指针
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 对每个 left，用 upper_bound 找到第一个 > angles[left] + angle 的位置，
// 差值就是窗口内的点数。本质上和滑动窗口等价，但二分是 O(n log n)，
// 而滑动窗口是 O(n)（排序后）。不过总体瓶颈都在排序。
// 好处：代码更短，不用维护 right 状态。
// ============================================================
class Solution2b {
public:
    int visiblePoints(vector<vector<int>>& points, int angle, vector<int>& location) {
        int same = 0;
        vector<double> angles;
        
        for (auto& p : points) {
            int dx = p[0] - location[0];
            int dy = p[1] - location[1];
            if (dx == 0 && dy == 0) {
                same++;
                continue;
            }
            double deg = atan2((double)dy, (double)dx) * 180.0 / M_PI;
            if (deg < 0) deg += 360.0;
            angles.push_back(deg);
        }
        
        sort(angles.begin(), angles.end());
        int n = angles.size();
        for (int i = 0; i < n; i++) {
            angles.push_back(angles[i] + 360.0);
        }
        
        int maxVisible = 0;
        for (int i = 0; i < n; i++) {
            // upper_bound 找第一个严格大于 angles[i] + angle + eps 的位置
            // 由于浮点精度，我们找 > angles[i] + angle + 1e-9 的第一个
            double target = angles[i] + (double)angle + 1e-9;
            auto it = upper_bound(angles.begin() + i, angles.end(), target);
            int count = (int)(it - angles.begin()) - i;
            maxVisible = max(maxVisible, count);
        }
        
        return maxVisible + same;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 核心操作 | 环形处理 |
// |------|------|------|----------|----------|
// | 暴力 | O(n²) | O(n) | 双重循环 | 角度差取模 |
// | 排序+滑动窗口 | O(n log n) | O(n) | 双指针 | 复制+360° |
// | 排序+二分 | O(n log n) | O(n) | upper_bound | 复制+360° |
//
// 滑动窗口 vs 二分：渐近等价（瓶颈在排序），滑动窗口常数更小，
// 但二分写法更简洁不易出错。面试推荐滑动窗口（展示双指针能力）。
//
// ============================================================
// 【易错点】
//
// 1. atan2(0,0) 未定义 — 与 location 重合的点必须先分离计数
//    ✗ 直接对所有点算 atan2 → 重合点角度为 0，被当成正东方向的点
//    ✓ 先检查 dx==0 && dy==0，计入 same，不加入 angles
//
// 2. 角度归一化不统一
//    ✗ atan2 返回 (-180, 180]，直接和 [0, 360) 的角度比较
//    ✓ 统一用 if (deg < 0) deg += 360.0
//
// 3. 环形展开遗漏
//    ✗ 只在原始 [0, n) 上滑窗，漏掉跨越 0°/360° 的视野
//    ✓ 复制拼接 +360°，在 [0, 2n) 上滑窗，left 只遍历 [0, n)
//
// 4. right 重置
//    ✗ 内层循环 right = left（O(n²)）
//    ✓ right 声明在 for 外面，只增不减（摊销 O(n)）
//
// 5. 浮点精度
//    ✗ angles[right] - angles[left] <= angle（边界点可能因精度被漏掉）
//    ✓ <= angle + 1e-9
//
// ============================================================
// 【面试追问】
//
// Q1(基础): 为什么最优视野一定可以让某个点恰好在左边界上？
//    → 如果边界不卡点，微调不改变可见集合，所以最优解中一定有
//      一个等价方案使得某个点在左边界上。枚举 n 个点即可。
//
// Q2(细节): atan2 的返回值范围是什么？为什么要归一化？
//    → 返回 (-π, π]，即 (-180°, 180°]。如果不归一化到 [0, 360)，
//      复制+360° 后会出现负角度+360 = 正角度的混乱。
//      统一归一化保证排序和窗口比较的正确性。
//
// Q3(变体): 如果视野不是连续的（有两个不相邻的视野扇区），怎么办？
//    → 如果是两个固定大小的扇区，可以枚举第一个扇区的位置（O(n)），
//      然后对剩余点再做一次滑动窗口找第二个扇区的最优位置。
//      总复杂度 O(n² log n) 或用更巧妙的方法。
//
// Q4(扩展): 如果需要返回具体哪些点可见（而不只是数量），怎么改？
//    → 记录取得最大值时的 [left, right) 区间，再映射回原始点。
//      需要维护排序时的索引映射。
// ============================================================
