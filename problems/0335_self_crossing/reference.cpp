/*
 * 【题目本质】
 * 按逆时针（北→西→南→东）移动，判断路径是否自交。
 * 核心：第 i 条线段只可能与第 i-3/i-4/i-5 条线段相交，检查三种模式。
 *
 * 【解法总览】
 * Solution1: 三种交叉模式判定 — O(n) 时间, O(1) 空间 ⭐推荐
 * Solution2: 状态机思路 — O(n) 时间, O(1) 空间（更清晰的分类方式）
 */

// ===== Solution1: 三种交叉模式判定 =====
// Case 1: d[i] >= d[i-2] && d[i-1] <= d[i-3]  (i 碰到 i-3)
// Case 2: d[i-1] == d[i-3] && d[i]+d[i-4] >= d[i-2]  (i 碰到 i-4)
// Case 3: 综合条件 (i 碰到 i-5)
class Solution1 {
public:
    bool isSelfCrossing(vector<int>& d) {
        int n = d.size();
        for (int i = 3; i < n; i++) {
            // Case 1: 当前线段足够长，碰到前第3条线段
            //   ┌──d[i-2]──┐
            //   │          │d[i-3]
            //   d[i-1]     │
            //   │          │
            //   └──────────┘──→ d[i]
            if (d[i] >= d[i - 2] && d[i - 1] <= d[i - 3])
                return true;

            // Case 2: 当前线段与前第4条线段在同一条直线上重叠
            if (i >= 4 &&
                d[i - 1] == d[i - 3] &&
                d[i] + d[i - 4] >= d[i - 2])
                return true;

            // Case 3: 当前线段穿过前第5条线段
            // 需要同时满足：
            // - d[i-2] > d[i-4]（内层比外层宽）
            // - d[i-1] <= d[i-3]（开始收缩）
            // - d[i-1] + d[i-5] >= d[i-3]（收缩但未完全脱离）
            // - d[i] + d[i-4] >= d[i-2]（当前步足够长）
            if (i >= 5 &&
                d[i - 2] > d[i - 4] &&
                d[i - 1] <= d[i - 3] &&
                d[i - 1] + d[i - 5] >= d[i - 3] &&
                d[i] + d[i - 4] >= d[i - 2])
                return true;
        }
        return false;
    }
};

// ===== Solution2: 状态机思路 =====
// 路径有三种状态：扩展中 / 收缩中 / 刚从扩展转收缩
// 扩展 → 扩展：不交叉
// 收缩 → 收缩：检查 d[i] < d[i-2]
// 扩展 → 收缩：过渡期，需要调整 d[i-2] 的有效长度
class Solution2 {
public:
    bool isSelfCrossing(vector<int>& d) {
        int n = d.size();
        if (n < 4) return false;

        int i = 2;
        // Phase 1: 扩展阶段 — d[i] > d[i-2]
        while (i < n && d[i] > d[i - 2]) i++;
        if (i >= n) return false;  // 一直在扩展，不交叉

        // 过渡：处理扩展→收缩的边界
        // 如果 d[i] + d[i-4](若存在) >= d[i-2]，则调整 d[i-2]
        if (d[i] >= d[i - 2] - (i >= 4 ? d[i - 4] : 0))
            d[i - 1] -= (i >= 3 ? d[i - 3] : 0);
        i++;

        // Phase 2: 收缩阶段 — 必须严格 d[i] < d[i-2]
        while (i < n) {
            if (d[i] >= d[i - 2]) return true;
            i++;
        }
        return false;
    }
};

/*
 * 【解法对比】
 * Solution1: 直接枚举三种交叉模式，代码简洁，容易记忆
 * Solution2: 状态机思路更系统化，但过渡处理较难理解
 * 面试推荐 Solution1，画图解释三种 case
 *
 * 【易错点】
 * 1. Case 1: d[i] >= d[i-2] 包含等于（刚好碰到也算交叉）
 * 2. Case 2: d[i-1] == d[i-3] 是严格等于（平行重叠）
 * 3. Case 3: 四个条件缺一不可，需要画图验证
 * 4. 下标越界保护：Case 2 需 i>=4，Case 3 需 i>=5
 *
 * 【面试追问】
 * Q1: 为什么只检查 i-3 到 i-5？
 *     → 方向4步一循环，i 与 i-4 同向，几何上最远碰到 i-5
 * Q2: 能否用线段相交通用算法？
 *     → 可以但 O(n^2)，利用方向循环特性可 O(n)
 * Q3: 如何证明三种 case 的完备性？
 *     → 路径从扩展到收缩，过渡期最多影响前5条线段
 */
