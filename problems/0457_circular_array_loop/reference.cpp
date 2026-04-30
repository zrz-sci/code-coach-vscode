/*
 * 【题目本质】
 * 环形数组中每个位置有唯一后继，判断是否存在长度 > 1 且方向一致的环。
 * 核心：快慢指针 + 方向检查 + 自环排除。
 *
 * 【解法总览】
 * Solution1: 快慢指针 + 原地标记 O(n)/O(1) ⭐ 最优
 * Solution2: DFS + visited 数组 O(n)/O(n) 易理解
 */

// ===================== Solution 1: 快慢指针 + 原地标记 =====================
// 从每个未标记节点出发，快慢指针检测环
// 每步检查方向一致性，找到环后验证环长 > 1
// 无效路径标记为 0 避免重复访问
class Solution1 {
public:
    bool circularArrayLoop(vector<int>& nums) {
        int n = nums.size();
        auto nxt = [&](int i) -> int {
            return ((i + nums[i]) % n + n) % n; // 处理负数取模
        };

        for (int i = 0; i < n; i++) {
            if (nums[i] == 0) continue;
            int slow = i, fast = i;
            // 快指针走两步，每步都验证方向
            while (nums[nxt(fast)] * nums[i] > 0 &&
                   nums[nxt(nxt(fast))] * nums[i] > 0) {
                slow = nxt(slow);
                fast = nxt(nxt(fast));
                if (slow == fast) {
                    if (slow == nxt(slow)) break; // 自环，长度 1
                    return true;
                }
            }
            // 标记路径节点为 0（不在有效环中）
            int j = i;
            while (nums[j] * nums[i] > 0) {
                int tmp = nxt(j);
                nums[j] = 0;
                j = tmp;
            }
        }
        return false;
    }
};

// ===================== Solution 2: DFS + visited 数组 =====================
// 用 visited 数组标记访问轮次，检测同轮再次到达的节点
class Solution2 {
public:
    bool circularArrayLoop(vector<int>& nums) {
        int n = nums.size();
        vector<int> color(n, 0); // 0=未访问, 1=本轮, 2=已完成

        auto nxt = [&](int i) -> int {
            return ((i + nums[i]) % n + n) % n;
        };

        for (int i = 0; i < n; i++) {
            if (color[i] == 2) continue;
            // 沿链走，记录本轮路径
            vector<int> path;
            int cur = i;
            bool valid = true;
            while (color[cur] == 0) {
                color[cur] = 1;
                path.push_back(cur);
                int nx = nxt(cur);
                // 方向不一致或自环
                if (nums[nx] * nums[i] <= 0 || nx == cur) {
                    valid = false;
                    break;
                }
                cur = nx;
            }
            if (valid && color[cur] == 1) {
                // cur 是环入口，验证环长 > 1
                if (nxt(cur) != cur) return true;
            }
            for (int p : path) color[p] = 2;
        }
        return false;
    }
};

/*
 * 【解法对比】
 * Solution1: O(1) 空间，原地标记，快慢指针经典
 * Solution2: O(n) 空间，DFS 逻辑更清晰
 *
 * 【易错点】
 * 1. 负数取模：((i + nums[i]) % n + n) % n
 * 2. 自环排除：next == current 不算有效环
 * 3. 方向一致性：快指针每步都要检查，不能只检查起点和终点
 * 4. 标记时只标记同方向的节点
 *
 * 【面试追问】
 * Q1: 为什么原地标记 0 安全？
 *     A: 0 表示该节点已确认不在有效环中，后续跳过不影响正确性
 * Q2: 如果 nums[i] 很大（大于 n），取模后是否正确？
 *     A: 是的，取模运算保证结果在 [0, n-1] 范围内
 * Q3: 该算法能否找到所有环？
 *     A: 不能，只判断是否存在。要找所有环需要记录环路径
 */
