/*
 * 519. Random Flip Matrix
 * 难度: Medium | 标签: Reservoir Sampling, Hash Table, Math, Randomized
 *
 * 【题目本质】
 * m*n 全0矩阵，flip 随机选一个0位置翻成1，要求等概率。
 * 核心：将矩阵展平为 [0, total-1]，用 Fisher-Yates 洗牌 + HashMap 实现 O(1) flip。
 *
 * 【解法总览】
 * Solution1: Fisher-Yates + HashMap — flip O(1), reset O(k) ⭐⭐⭐
 * Solution2: HashSet 拒绝采样       — flip 期望 O(1) 最坏 O(k) ⭐
 */

// ===================== Solution1: Fisher-Yates + HashMap =====================
// 思路：
// 将矩阵看作一维 [0, total-1]，维护可选范围 [0, remain-1]
// 每次 flip: rand(0, remain-1) → 选中 r → 用 map 记录交换 → remain--
// map[x] 存的是 x 当前映射到的"实际值"，不存在则映射到自己
class Solution1 {
    int rows, cols, remain;
    unordered_map<int, int> mp;
public:
    Solution1(int m, int n) : rows(m), cols(n), remain(m * n) {}

    vector<int> flip() {
        int r = rand() % remain;
        int val = mp.count(r) ? mp[r] : r;
        remain--;
        mp[r] = mp.count(remain) ? mp[remain] : remain;
        return {val / cols, val % cols};
    }

    void reset() {
        mp.clear();
        remain = rows * cols;
    }
};

// ===================== Solution2: HashSet 拒绝采样 =====================
// 思路：随机生成 [0, total-1]，如果已选过则重新生成
// 缺点：接近满时碰撞严重，最坏 O(k) per flip
class Solution2 {
    int rows, cols, total;
    unordered_set<int> used;
public:
    Solution2(int m, int n) : rows(m), cols(n), total(m * n) {}

    vector<int> flip() {
        int r;
        do {
            r = rand() % total;
        } while (used.count(r));
        used.insert(r);
        return {r / cols, r % cols};
    }

    void reset() {
        used.clear();
    }
};

/*
 * 【解法对比】
 * Fisher-Yates: 每次 flip 严格 O(1)，空间 O(flip次数)
 * 拒绝采样: 实现简单，但接近满时性能退化严重
 *
 * 【易错点】
 * 1. 坐标转换是 val/cols 和 val%cols，不是 rows
 * 2. map[r] 不存在时默认映射到 r 自身
 * 3. 交换时 map[remain-1] 也可能不存在，需要同样处理
 *
 * 【面试追问】
 * Q1: Fisher-Yates 为何保证等概率？→ 每个位置被选中概率 = 1/remain
 * Q2: 空间复杂度？→ O(flip次数)，远优于 O(m*n) 的完整数组
 * Q3: reset 的复杂度？→ O(k) 清空 map，k = 已 flip 次数
 */
