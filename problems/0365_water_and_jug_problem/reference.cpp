// 解法1: 裴蜀定理 (Bezout's Identity) - 数学最优解
// 时间 O(log(min(x,y))), 空间 O(1)
//
// 核心定理: 对于整数 a, b，方程 ax + by = c 有整数解
// 当且仅当 gcd(a, b) | c (c 是 gcd(a,b) 的倍数)
//
// 应用到本题:
// - 每次操作本质: 装满 = +x 或 +y, 清空 = -x 或 -y, 倒水 = 总量不变
// - 最终总水量 = (某整数)*x + (某整数)*y = ax + by
// - 所以能量出的水量 = gcd(x, y) 的所有整数倍
// - 物理约束: 总量不能超过 x + y
//
// 判断条件: target <= x + y 且 target % gcd(x, y) == 0
class Solution1 {
public:
    bool canMeasureWater(int x, int y, int target) {
        // 特判: 两壶加起来不够
        if (target > x + y) return false;
        // 特判: 目标为 0，两壶都空即可
        if (target == 0) return true;
        // 裴蜀定理核心判断
        return target % gcd(x, y) == 0;
    }

private:
    // 辗转相除法求 GCD
    // 递归版本: gcd(a, b) = gcd(b, a % b)，直到 b = 0
    // 时间复杂度: O(log(min(a, b)))
    int gcd(int a, int b) {
        return b == 0 ? a : gcd(b, a % b);
    }
};

// 解法2: BFS 状态搜索 - 枚举所有可能的 (壶1水量, 壶2水量) 状态
// 时间 O(x*y), 空间 O(x*y)
//
// 状态: (a, b) 表示壶1有 a 升水, 壶2有 b 升水
// 初始: (0, 0)
// 目标: 存在 (a, b) 使得 a + b == target
//
// 6种转移操作:
// 1. 装满壶1: (a, b) -> (x, b)
// 2. 装满壶2: (a, b) -> (a, y)
// 3. 清空壶1: (a, b) -> (0, b)
// 4. 清空壶2: (a, b) -> (a, 0)
// 5. 壶1倒入壶2: pour = min(a, y-b), (a, b) -> (a-pour, b+pour)
// 6. 壶2倒入壶1: pour = min(b, x-a), (a, b) -> (a+pour, b-pour)
class Solution2 {
public:
    bool canMeasureWater(int x, int y, int target) {
        // 提前剪枝
        if (target > x + y) return false;
        if (target == 0) return true;

        // BFS 初始化
        set<pair<int,int>> visited;
        queue<pair<int,int>> q;
        q.push({0, 0});
        visited.insert({0, 0});

        while (!q.empty()) {
            auto [a, b] = q.front();
            q.pop();

            // 检查是否达到目标
            if (a + b == target) return true;

            // 生成 6 种操作的下一状态
            vector<pair<int,int>> nextStates;

            // 操作1: 装满壶1
            nextStates.push_back({x, b});
            // 操作2: 装满壶2
            nextStates.push_back({a, y});
            // 操作3: 清空壶1
            nextStates.push_back({0, b});
            // 操作4: 清空壶2
            nextStates.push_back({a, 0});
            // 操作5: 壶1 -> 壶2
            int pour1to2 = min(a, y - b);
            nextStates.push_back({a - pour1to2, b + pour1to2});
            // 操作6: 壶2 -> 壶1
            int pour2to1 = min(b, x - a);
            nextStates.push_back({a + pour2to1, b - pour2to1});

            // 入队未访问的状态
            for (auto& next : nextStates) {
                if (visited.find(next) == visited.end()) {
                    visited.insert(next);
                    q.push(next);
                }
            }
        }

        return false;
    }
};

// 解法3: 裴蜀定理 - 使用 C++ 标准库 __gcd / std::gcd
// 时间 O(log(min(x,y))), 空间 O(1)
// 最简洁的写法，适合竞赛
class Solution3 {
public:
    bool canMeasureWater(int x, int y, int target) {
        if (target > x + y) return false;
        if (x == 0 || y == 0) return target == 0 || target == x + y;
        return target % __gcd(x, y) == 0;
    }
};

// 补充: GCD 迭代版本 (避免递归栈溢出，面试可能被追问)
class Solution4_GCD_Iterative {
public:
    bool canMeasureWater(int x, int y, int target) {
        if (target > x + y) return false;
        if (target == 0) return true;
        return target % gcdIterative(x, y) == 0;
    }

private:
    int gcdIterative(int a, int b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }
};

/*
 * 解法对比:
 * ┌──────────────────────┬──────────────────┬─────────┬───────────────────┐
 * │ 解法                 │ 时间             │ 空间    │ 特点              │
 * ├──────────────────────┼──────────────────┼─────────┼───────────────────┤
 * │ Solution1 裴蜀定理   │ O(log(min(x,y))) │ O(1)    │ 数学最优解        │
 * │ Solution2 BFS搜索    │ O(x*y)           │ O(x*y)  │ 直觉理解更容易    │
 * │ Solution3 标准库GCD  │ O(log(min(x,y))) │ O(1)    │ 最简洁            │
 * │ Solution4 迭代GCD    │ O(log(min(x,y))) │ O(1)    │ 无递归栈风险      │
 * └──────────────────────┴──────────────────┴─────────┴───────────────────┘
 *
 * 裴蜀定理直觉理解:
 *   x=3, y=5 操作序列: (0,0) -> (0,5) -> (3,2) -> (0,2) -> (2,0) -> (2,5) -> (3,4)
 *   总量变化: 0 -> +5 -> 不变 -> -3 -> 不变 -> +5 -> 不变
 *   净效果: +5+5-3-3 = 4 = 2*5 + (-2)*3
 *   这就是 ax + by 的形式!
 *
 * 手动验证:
 *   x=3, y=5, target=4: gcd=1, 4%1==0, 4<=8 -> true
 *   x=2, y=6, target=5: gcd=2, 5%2==1!=0    -> false
 *   x=1, y=2, target=3: gcd=1, 3%1==0, 3<=3 -> true
 *
 * 易错点:
 * 1. target > x + y 必须先判断
 * 2. target == 0 时应返回 true
 * 3. BFS 法的状态空间可能很大，注意剪枝
 * 4. 倒水操作的 pour 计算: min(当前壶水量, 目标壶剩余容量)
 *
 * 面试追问:
 * Q: 不用数学定理怎么做? -> BFS 搜索
 * Q: 3 个壶怎么办? -> gcd(x, gcd(y, z))
 * Q: 如何输出操作步骤? -> BFS 记录 parent 路径回溯
 * Q: GCD 时间复杂度证明? -> Fibonacci 是最坏情况, O(log n)
 */
