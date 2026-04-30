// 789. Escape The Ghosts
// 难度: Medium
// 标签: Array, Math

// ============================================================
// 解法一：曼哈顿距离比较 (唯一最优解)
// 时间 O(G), 空间 O(1), G 为鬼怪数量
// ============================================================
// 核心定理: 玩家能逃脱 <==> 所有鬼怪到目标的曼哈顿距离
//           都 严格大于 玩家到目标的曼哈顿距离
//
// 证明: 鬼怪的最优策略是直奔目标点"守株待兔"。
//       如果鬼怪离目标点不比玩家远, 它一定能先到或同时到,
//       无论玩家走什么路线都会被拦截。
class Solution {
public:
    bool escapeGhosts(vector<vector<int>>& ghosts, vector<int>& target) {
        // 玩家从 (0,0) 到 target 的曼哈顿距离
        int playerDist = abs(target[0]) + abs(target[1]);

        // 检查每个鬼怪到 target 的曼哈顿距离
        for (auto& g : ghosts) {
            int ghostDist = abs(g[0] - target[0]) + abs(g[1] - target[1]);
            // 如果鬼怪能先到或同时到达目标, 玩家无法逃脱
            if (ghostDist <= playerDist) {
                return false;
            }
        }
        return true;
    }
};

// ============================================================
// 解法二：使用 any_of 的函数式写法
// 时间 O(G), 空间 O(1)
// ============================================================
class Solution2 {
public:
    bool escapeGhosts(vector<vector<int>>& ghosts, vector<int>& target) {
        int pd = abs(target[0]) + abs(target[1]);

        // 如果任何一个鬼怪距离目标 <= 玩家距离, 则无法逃脱
        return !any_of(ghosts.begin(), ghosts.end(),
            [&](const vector<int>& g) {
                return abs(g[0] - target[0]) + abs(g[1] - target[1]) <= pd;
            });
    }
};

// ============================================================
// 解法三：展开详细逻辑 (适合面试手写)
// 时间 O(G), 空间 O(1)
// ============================================================
class Solution3 {
public:
    bool escapeGhosts(vector<vector<int>>& ghosts, vector<int>& target) {
        int tx = target[0], ty = target[1];

        // 玩家最短路径 = 曼哈顿距离 (从原点出发)
        int myDist = abs(tx) + abs(ty);

        for (int i = 0; i < (int)ghosts.size(); ++i) {
            int gx = ghosts[i][0];
            int gy = ghosts[i][1];

            // 鬼怪到目标的曼哈顿距离
            int ghostDist = abs(gx - tx) + abs(gy - ty);

            // 鬼怪能先到或同时到 -> 玩家被抓
            if (ghostDist <= myDist) {
                return false;
            }
        }

        // 所有鬼怪都追不上
        return true;
    }
};

// ============================================================
// 数学证明附注:
// ============================================================
// 为什么鬼怪的最优策略是去目标点等待?
//
// 假设鬼怪 G 在位置 (gx, gy), 目标 T 在 (tx, ty), 玩家 P 在 (0,0)
//
// 情况1: d(G,T) <= d(P,T)
//   鬼怪直接去 T 等着。无论玩家走什么路线, 玩家至少需要
//   d(P,T) 步才能到 T, 而鬼怪最多需要 d(G,T) 步。
//   由于 d(G,T) <= d(P,T), 鬼怪先到或同时到, 玩家被拦。
//
// 情况2: d(G,T) > d(P,T)
//   玩家直线走到 T, 需要 d(P,T) 步。
//   鬼怪无论走去哪里, 都来不及在 d(P,T) 步内到达 T。
//   鬼怪如果想在中途拦截玩家于某点 M:
//     d(G,M) + d(M,T) >= d(G,T) > d(P,T)
//   但玩家可以选择不经过 M 的替代路线, 或者
//   直线路线本身就不经过 M。
//   因此鬼怪无法同时拦截中途又回到终点。
