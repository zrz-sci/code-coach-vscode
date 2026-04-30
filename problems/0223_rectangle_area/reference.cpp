/*
 * 【题目本质】
 * 两个轴对齐矩形覆盖的总面积 = A面积 + B面积 - 重叠面积。
 * 核心：区间交集公式 + 容斥原理。
 *
 * 【解法总览】
 * 解法1: 容斥原理 + 区间交集 ⭐ — O(1) 时间 O(1) 空间
 * 解法2: 分情况讨论 — 先判断是否重叠，再计算
 */

// ===================== 解法1: 容斥原理 + 区间交集 (推荐) =====================
// 思路：
// 1. 分别计算两个矩形的面积
// 2. 重叠宽度 = max(0, min(ax2,bx2) - max(ax1,bx1))
//    重叠高度 = max(0, min(ay2,by2) - max(ay1,by1))
// 3. 总面积 = area_A + area_B - overlap_w * overlap_h
//
// 区间交集公式：[a1,a2] ∩ [b1,b2] = [max(a1,b1), min(a2,b2)]
// 如果 max > min 则交集为空（overlap = 0）
class Solution1 {
public:
    int computeArea(int ax1, int ay1, int ax2, int ay2,
                    int bx1, int by1, int bx2, int by2) {
        // 两个矩形各自的面积
        int area_A = (ax2 - ax1) * (ay2 - ay1);
        int area_B = (bx2 - bx1) * (by2 - by1);

        // X 方向重叠宽度（区间交集）
        int overlap_w = max(0, min(ax2, bx2) - max(ax1, bx1));
        // Y 方向重叠高度（区间交集）
        int overlap_h = max(0, min(ay2, by2) - max(ay1, by1));

        // 容斥原理：总面积 = A + B - A∩B
        return area_A + area_B - overlap_w * overlap_h;
    }
};

// ===================== 解法2: 先判断重叠再计算 =====================
// 思路：
// 显式列出不重叠的四种情况
// 不重叠时直接返回面积之和
// 重叠时再计算交集面积
class Solution2 {
public:
    int computeArea(int ax1, int ay1, int ax2, int ay2,
                    int bx1, int by1, int bx2, int by2) {
        int area_A = (ax2 - ax1) * (ay2 - ay1);
        int area_B = (bx2 - bx1) * (by2 - by1);

        // 不重叠的四种情况：
        // A 在 B 左边 | A 在 B 右边 | A 在 B 下面 | A 在 B 上面
        if (ax2 <= bx1 || bx2 <= ax1 || ay2 <= by1 || by2 <= ay1) {
            return area_A + area_B;
        }

        // 有重叠，计算交集
        int overlap_w = min(ax2, bx2) - max(ax1, bx1);
        int overlap_h = min(ay2, by2) - max(ay1, by1);

        return area_A + area_B - overlap_w * overlap_h;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | 容斥+区间交集 ⭐ | O(1) | O(1) | 代码最简，max(0,...) 统一处理 |
 * | 先判断再计算 | O(1) | O(1) | 逻辑清晰但代码略长 |
 *
 * 【易错点】
 * 1. 重叠宽/高必须用 max(0, ...) 截断，否则不重叠时得到负值
 * 2. 区间交集是 [max(左端点), min(右端点)]，不要搞反
 * 3. 坐标范围 [-10^4, 10^4]，面积最大 4*10^8，int 不溢出
 * 4. (ax1,ay1) 是左下角不是左上角（数学坐标系，不是屏幕坐标系）
 *
 * 【面试追问】
 * Q1: N 个矩形求总面积？ → 坐标压缩 + 扫描线 O(N^2) 或线段树 O(NlogN)
 * Q2: 如何判断两矩形是否重叠？ → 四个不重叠条件取反
 * Q3: 如果允许旋转（非轴对齐）？ → Sutherland-Hodgman 多边形裁剪
 */
