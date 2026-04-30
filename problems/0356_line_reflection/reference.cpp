/*
 * 【题目本质】
 * 判断 2D 平面上的点集是否关于某条平行于 y 轴的直线对称
 * 核心：确定对称线位置 x = (minX+maxX)/2，然后验证每个点的对称点是否存在
 *
 * 【解法总览】
 * 解法1: HashSet 验证 — O(N) 时间，最优解
 * 解法2: 排序 + 双指针 — O(NlogN)，不需要哈希
 */

// ===================== Solution 1: HashSet 对称验证（推荐）=====================
// 思路：
// 1. 找 minX 和 maxX → 对称线 x = (minX+maxX)/2
// 2. 用 sum = minX + maxX 避免浮点运算
// 3. 将所有点放入 set 去重
// 4. 对每个点 (x,y)，检查对称点 (sum-x, y) 是否在 set 中
// 时间：O(N)   空间：O(N)
class Solution {
public:
    bool isReflected(vector<vector<int>>& points) {
        int minX = INT_MAX, maxX = INT_MIN;
        // 用 string 编码避免 pair hash 问题
        unordered_set<string> pointSet;

        for (auto& p : points) {
            minX = min(minX, p[0]);
            maxX = max(maxX, p[0]);
            pointSet.insert(to_string(p[0]) + "," + to_string(p[1]));
        }

        int sum = minX + maxX;  // = 2d, 整数运算避免浮点精度

        for (auto& p : points) {
            // 对称点 x' = sum - x, y' = y
            string mirror = to_string(sum - p[0]) + "," + to_string(p[1]);
            if (!pointSet.count(mirror)) return false;
        }
        return true;
    }
};

// ===================== Solution 2: set<pair> 验证 =====================
// 思路：用 set<pair<int,int>> 自带去重和查找
// 优点：无需字符串编码，代码更简洁
// 时间：O(N logN)   空间：O(N)
class Solution2 {
public:
    bool isReflected(vector<vector<int>>& points) {
        int minX = INT_MAX, maxX = INT_MIN;
        set<pair<int,int>> ps;

        for (auto& p : points) {
            minX = min(minX, p[0]);
            maxX = max(maxX, p[0]);
            ps.insert({p[0], p[1]});
        }

        int sum = minX + maxX;
        for (auto& [x, y] : ps) {
            if (!ps.count({sum - x, y})) return false;
        }
        return true;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | unordered_set + string | O(N) | O(N) | 最快，string 编码简单 |
 * | set<pair> | O(NlogN) | O(N) | 代码更简洁，无编码 |
 *
 * 【易错点】
 * 1. 浮点精度：对称线 x=(minX+maxX)/2 可能是 0.5
 *    → 用 sum = minX + maxX 做整数运算，对称点 x' = sum - x
 * 2. 必须去重：重复点不影响对称性，但不去重会重复验证
 *    → set 自动去重
 * 3. y 坐标必须匹配：(x,y) 的对称点是 (sum-x, y) 不是 (sum-x, -y)
 *    → 对称轴平行于 y 轴，y 不变
 * 4. 单点 / 所有点 x 相同：都是合法的（对称线过这些点）
 *    → sum - x == x 时自对称，set 中已包含自身
 *
 * 【面试追问】
 * Q1: 坐标为浮点数怎么处理？
 *     → 乘以公倍数转整数，或用 epsilon 比较
 * Q2: 如何找平行于 x 轴的对称线？
 *     → 交换 x/y 角色，找 minY/maxY，检查 (x, sumY-y)
 * Q3: 如何判断是否存在任意方向的对称线？
 *     → 需要几何分析：找质心，枚举方向角，复杂度更高
 */
