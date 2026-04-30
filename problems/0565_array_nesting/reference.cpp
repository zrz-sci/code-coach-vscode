// 【题目本质】
// 数组是 [0, n-1] 的排列，构成函数图（每个节点出度1）。
// 求最大环的长度。利用排列性质，每个连通分量都是环。

// 【解法总览】
// Solution1: 原地标记遍历环  O(n) / O(1)
// Solution2: visited 数组     O(n) / O(n)

// ===================== Solution 1 =====================
// 原地标记法
// 将访问过的元素标记为 -1，节省空间
class Solution1 {
public:
    int arrayNesting(vector<int>& nums) {
        int maxLen = 0;
        for (int i = 0; i < (int)nums.size(); ++i) {
            if (nums[i] == -1) continue;
            int count = 0, j = i;
            while (nums[j] != -1) {
                int next = nums[j];
                nums[j] = -1;  // 标记已访问
                j = next;
                count++;
            }
            maxLen = max(maxLen, count);
        }
        return maxLen;
    }
};

// ===================== Solution 2 =====================
// visited 数组法
// 不修改原数组，使用额外空间记录访问状态
class Solution2 {
public:
    int arrayNesting(vector<int>& nums) {
        int n = nums.size(), maxLen = 0;
        vector<bool> visited(n, false);
        for (int i = 0; i < n; ++i) {
            if (visited[i]) continue;
            int count = 0, j = i;
            while (!visited[j]) {
                visited[j] = true;
                j = nums[j];
                count++;
            }
            maxLen = max(maxLen, count);
        }
        return maxLen;
    }
};

// 【解法对比】
// Solution1: O(1) 空间但修改原数组，面试优先
// Solution2: 不修改原数组，逻辑清晰，适合只读场景

// 【易错点】
// 1. 每个元素只被访问一次 -> 总时间 O(n) 不是 O(n^2)
// 2. 原地标记时先保存 next 再覆盖当前值
// 3. 同一个环中所有起点结果相同，无需重复计算

// 【面试追问】
// Q1: 为什么排列必成环？n节点n边，每个出度=入度=1 -> 不相交环并
// Q2: 不修改数组且O(1)空间？不可能，需要额外状态。可以遍历后恢复
// Q3: 与并查集的关系？可用并查集合并，但直接遍历更高效
