/*
 * LeetCode 666: 路径总和 IV (Path Sum IV)
 * 难度: Medium
 * 标签: Tree, DFS, Array, Hash Table, Binary Tree
 *
 * 【题目本质】
 * 三位数编码的二叉树，求所有根到叶路径的值之和。
 * 关键: 用 HashMap 存节点，利用满二叉树位置关系找子节点。
 *
 * 【解法总览】
 * 解法一: HashMap + DFS  O(n) / O(n)
 * 解法二: HashMap + BFS  O(n) / O(n)
 */


/*
 * ============================================================
 * 解法一: HashMap + DFS
 * ============================================================
 *
 * 思路:
 *   1. 解析每个三位数，用 key=depth*10+pos 存入 HashMap
 *   2. 从根节点 DFS，维护路径和
 *   3. 叶子节点(左右子都不存在)时，累加路径和到结果
 *
 * 子节点位置: 深度 d 位置 p 的节点
 *   左子: (d+1, 2p-1)  右子: (d+1, 2p)
 *
 * 示例: nums = [113, 215, 221]
 *   HashMap: {11->3, 21->5, 22->1}
 *   树结构:     3 (1,1)
 *            /     \
 *          5(2,1)  1(2,2)
 *
 *   DFS(11, 0):
 *     pathSum = 3
 *     leftKey=21 存在, rightKey=22 存在
 *     DFS(21, 3): pathSum=8, 无子 -> result += 8
 *     DFS(22, 3): pathSum=4, 无子 -> result += 4
 *   result = 12
 *
 * 时间: O(n), 空间: O(n)
 */
class Solution1 {
public:
    int pathSum(vector<int>& nums) {
        unordered_map<int, int> mp;
        for (int num : nums) {
            mp[num / 10] = num % 10;
        }

        int result = 0;
        dfs(mp, nums[0] / 10, 0, result);
        return result;
    }

private:
    void dfs(unordered_map<int, int>& mp, int key, int sum, int& result) {
        if (mp.find(key) == mp.end()) return;

        int depth = key / 10;
        int pos = key % 10;
        sum += mp[key];

        int leftKey = (depth + 1) * 10 + (2 * pos - 1);
        int rightKey = (depth + 1) * 10 + (2 * pos);

        // 叶子节点: 左右子都不存在
        if (mp.find(leftKey) == mp.end() && mp.find(rightKey) == mp.end()) {
            result += sum;
            return;
        }

        dfs(mp, leftKey, sum, result);
        dfs(mp, rightKey, sum, result);
    }
};


/*
 * ============================================================
 * 解法二: HashMap + BFS
 * ============================================================
 *
 * 思路:
 *   用队列进行层序遍历，每个元素存 (key, 累计路径和)。
 *   如果当前节点没有子节点（叶子），累加路径和。
 *
 * 时间: O(n), 空间: O(n)
 */
class Solution2 {
public:
    int pathSum(vector<int>& nums) {
        unordered_map<int, int> mp;
        for (int num : nums) {
            mp[num / 10] = num % 10;
        }

        int result = 0;
        queue<pair<int, int>> q; // (key, pathSum)
        int rootKey = nums[0] / 10;
        q.push({rootKey, mp[rootKey]});

        while (!q.empty()) {
            auto [key, sum] = q.front();
            q.pop();

            int d = key / 10, p = key % 10;
            int lk = (d + 1) * 10 + (2 * p - 1);
            int rk = (d + 1) * 10 + (2 * p);

            bool isLeaf = true;
            if (mp.count(lk)) {
                q.push({lk, sum + mp[lk]});
                isLeaf = false;
            }
            if (mp.count(rk)) {
                q.push({rk, sum + mp[rk]});
                isLeaf = false;
            }
            if (isLeaf) {
                result += sum;
            }
        }
        return result;
    }
};


/*
 * ============================================================
 * 【解法对比】
 *
 * | 解法          | 时间 | 空间 | 特点                    |
 * |--------------|------|------|------------------------|
 * | DFS (解法一)  | O(n) | O(n) | 递归，代码简洁直觉        |
 * | BFS (解法二)  | O(n) | O(n) | 迭代，层序思维            |
 *
 * 面试推荐: DFS 更自然，BFS 作为 follow-up 展示多样性
 *
 * ============================================================
 * 【易错点】
 *
 * 1. 子节点位置计算错误:
 *    ✗ 左子 = 2*p, 右子 = 2*p+1 (这是 0-indexed 的公式)
 *    ✓ 左子 = 2*p-1, 右子 = 2*p (本题 1-indexed)
 *
 * 2. 非叶节点误加路径和:
 *    ✗ 每个节点都把 pathSum 加入 result
 *    ✓ 只在叶子节点（左右子都不存在）时才加入 result
 *
 * 3. key 设计溢出:
 *    ✗ 用 depth*100+pos*10+val 作为 key (和原始数混淆)
 *    ✓ 用 depth*10+pos 作为 key，val 单独存储
 *
 * ============================================================
 * 【面试追问】
 *
 * Q1: 为什么不构建真正的树结构？
 * → 树深度最多 4（最多 15 节点），HashMap 足够。
 *   编码本身包含位置信息，可以直接计算子节点，
 *   省去了构建 TreeNode 和连接指针的开销。
 *
 * Q2: 子节点位置公式怎么来的？
 * → 满二叉树中 1-indexed 的位置编号：第 d 层第 p 个节点，
 *   前 p-1 个节点各有 2 个子节点占了位置 1..2(p-1)，
 *   所以左子位置 = 2p-1，右子位置 = 2p。
 *
 * Q3: 如果只求最大路径和而不是总和？
 * → 把 result += sum 改为 result = max(result, sum)。
 *   DFS/BFS 结构不变，只改收集结果的方式。
 * ============================================================
 */
