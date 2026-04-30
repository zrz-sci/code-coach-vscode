// LeetCode 780: Reaching Points
// 难度: Hard | 标签: Math
//
// 解法一：逆向 + 取模（最优解）
// 时间 O(log(max(tx,ty)))  空间 O(1)
//
// 核心思路：
// 正向搜索是指数级的，但逆向每步操作唯一。
// 从 (tx, ty) 向 (sx, sy) 回溯：
//   tx > ty 时前驱为 (tx - ty, ty)，用取模加速
//   ty > tx 时前驱为 (tx, ty - tx)，用取模加速
// 和辗转相除法同构，O(log) 复杂度。

class Solution {
public:
    bool reachingPoints(int sx, int sy, int tx, int ty) {
        // 逆向回溯：从 (tx, ty) 回退到 (sx, sy)
        // 每次让较大的坐标做取模运算

        while (tx > sx && ty > sy) {
            if (tx > ty) {
                tx %= ty;
            } else {
                ty %= tx;
            }
        }

        // 情况 1: 完全匹配
        if (tx == sx && ty == sy) return true;

        // 情况 2: tx 已经回退到 sx，只需检查 ty 方向
        // ty 必须 >= sy，且 (ty - sy) 必须是 sx 的整数倍
        // （因为接下来每步只能是 ty -= sx）
        if (tx == sx) {
            return ty >= sy && (ty - sy) % sx == 0;
        }

        // 情况 3: ty 已经回退到 sy，只需检查 tx 方向
        if (ty == sy) {
            return tx >= sx && (tx - sx) % sy == 0;
        }

        // 情况 4: 两个坐标都没有到达起点，不可达
        return false;
    }
};

// =============================================================
// 解法二：逆向 + 取模（带详细注释版）
// 时间 O(log(max(tx,ty)))  空间 O(1)
//
// 与解法一完全相同，但增加了更多注释帮助理解

class Solution2 {
public:
    bool reachingPoints(int sx, int sy, int tx, int ty) {
        // 为什么逆向？
        // 正向：(x,y) -> (x,x+y) 或 (x+y,y)，两个分支
        // 逆向：(tx,ty) 的前驱唯一确定
        //   tx > ty -> 只能来自 (tx-ty, ty)
        //   ty > tx -> 只能来自 (tx, ty-tx)
        //   tx == ty -> 无法回退（除非已经是起点）

        // 取模加速：当 tx >> ty 时
        // 反复执行 tx -= ty 等价于 tx %= ty
        // 但要确保不要减过 sx

        while (tx > sx && ty > sy) {
            // 选择较大的做取模
            if (tx > ty) {
                // tx 对 ty 取模
                // 注意：如果 tx % ty == 0，tx 会变成 0
                // 但由于 ty > sy >= 1，且 tx > sx >= 1
                // 取模后 tx 在 [0, ty-1] 范围内
                tx %= ty;
            } else {
                ty %= tx;
            }
        }

        // 退出循环时至少有一个坐标 <= 对应起点

        if (tx == sx && ty == sy) {
            return true;
        }

        if (tx == sx && ty > sy) {
            // tx 已经固定为 sx
            // 接下来只能沿 y 方向减少：ty -= sx 反复执行
            // 需要 (ty - sy) 恰好是 sx 的倍数
            return (ty - sy) % sx == 0;
        }

        if (ty == sy && tx > sx) {
            // ty 已经固定为 sy
            // 接下来只能沿 x 方向减少：tx -= sy 反复执行
            return (tx - sx) % sy == 0;
        }

        return false;
    }
};

// =============================================================
// 解法三：递归版（简洁但可能栈溢出）
// 时间 O(log(max(tx,ty)))  空间 O(log(max(tx,ty))) 栈空间
//
// 递归结构清晰，但大数据时栈可能较深。

class Solution3 {
public:
    bool reachingPoints(int sx, int sy, int tx, int ty) {
        // base cases
        if (tx < sx || ty < sy) return false;
        if (tx == sx && ty == sy) return true;

        // 当一个坐标已经到位时
        if (tx == sx) return (ty - sy) % sx == 0;
        if (ty == sy) return (tx - sx) % sy == 0;

        // 递归回退
        if (tx > ty) {
            // 用取模加速
            return reachingPoints(sx, sy, tx % ty, ty);
        } else {
            return reachingPoints(sx, sy, tx, ty % tx);
        }
    }
};

// =============================================================
// 解法四：逆向减法（理解用，会 TLE）
// 时间 O(max(tx,ty))  空间 O(1)
//
// 不用取模，每次只减一步。展示基本思路但不适合提交。

class Solution4 {
public:
    bool reachingPoints(int sx, int sy, int tx, int ty) {
        while (tx >= sx && ty >= sy) {
            if (tx == sx && ty == sy) return true;

            if (tx > ty) {
                tx -= ty;
            } else if (ty > tx) {
                ty -= tx;
            } else {
                // tx == ty 且不等于 (sx, sy)
                return false;
            }
        }
        return false;
    }
};
