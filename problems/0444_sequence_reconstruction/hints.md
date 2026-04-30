# 444. 序列重建 (Sequence Reconstruction)

## 核心思路
判断 `nums` 是否是 `sequences` 确定的**唯一拓扑排序**。建有向图后做 BFS 拓扑排序，如果任何时刻队列中有超过一个节点，说明拓扑序不唯一，返回 false。

## 思维链
1. sequences 中相邻元素构成有向边（偏序约束）
2. 拓扑排序可以恢复全序——但可能有多个合法排序
3. 唯一拓扑排序的充要条件：BFS 中每一步队列大小恰好为 1
4. 额外验证：拓扑序的结果必须与 nums 完全相同

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| ⭐ 解法1: BFS拓扑排序 | 建图+BFS，检查队列唯一性 | O(V+E) | O(V+E) | 首选 |
| 解法2: 相邻对验证 | 检查 nums 中相邻对是否都被 sequences 覆盖 | O(V+E) | O(V) | 巧妙替代 |

## 关键提示
1. 从 sequences 的每个序列提取相邻对作为有向边
2. BFS 中每步队列大小必须 == 1，否则有多个合法拓扑序
3. 拓扑排序结果必须恰好有 n 个节点且与 nums 匹配
4. 解法2 的核心观察：若 nums 中每对相邻元素 (nums[i], nums[i+1]) 都是某个 sequence 的相邻对，则 nums 唯一

## 解法详解

### 解法1: BFS拓扑排序 ⭐
```cpp
class Solution {
public:
    bool sequenceReconstruction(vector<int>& nums, vector<vector<int>>& sequences) {
        int n = nums.size();
        vector<unordered_set<int>> graph(n + 1);
        vector<int> indegree(n + 1, 0);
        
        // 建图
        for (auto& seq : sequences) {
            for (int i = 0; i + 1 < seq.size(); i++) {
                if (graph[seq[i]].insert(seq[i + 1]).second) {
                    indegree[seq[i + 1]]++;
                }
            }
        }
        
        // BFS
        queue<int> q;
        for (int i = 1; i <= n; i++) {
            if (indegree[i] == 0) q.push(i);
        }
        
        int idx = 0;
        while (!q.empty()) {
            if (q.size() > 1) return false; // 不唯一
            int curr = q.front(); q.pop();
            if (idx >= n || nums[idx] != curr) return false;
            idx++;
            for (int next : graph[curr]) {
                if (--indegree[next] == 0) q.push(next);
            }
        }
        return idx == n;
    }
};
```

### 解法2: 相邻对验证
```cpp
class Solution {
public:
    bool sequenceReconstruction(vector<int>& nums, vector<vector<int>>& sequences) {
        int n = nums.size();
        vector<int> pos(n + 1);
        for (int i = 0; i < n; i++) pos[nums[i]] = i;
        
        // 标记 nums 中哪些相邻对被 sequences 覆盖
        vector<bool> covered(n, false);
        for (auto& seq : sequences) {
            for (int i = 0; i + 1 < seq.size(); i++) {
                if (pos[seq[i]] >= pos[seq[i + 1]]) return false;
                if (pos[seq[i]] + 1 == pos[seq[i + 1]]) {
                    covered[pos[seq[i]]] = true;
                }
            }
        }
        
        // 所有相邻对都必须被覆盖
        for (int i = 0; i + 1 < n; i++) {
            if (!covered[i]) return false;
        }
        return true;
    }
};
```

## 易错点
| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 不去重直接加边 | 用 set 去重或检查 | 重复边导致 indegree 计数错误 |
| 忘记检查 idx == n | 最后验证 idx == n | 图中可能有环导致提前结束 |
| 只检查队列唯一性 | 同时验证与 nums 匹配 | 唯一拓扑序不一定等于 nums |

## 面试追问

**Q1: 为什么队列大小 > 1 就说明不唯一？**
> 队列中有多个入度为 0 的节点意味着它们的相对顺序未被约束，可以交换，从而产生多个合法拓扑序。

**Q2: 解法2 为什么只需要检查相邻对？**
> 如果 nums 中每对相邻元素 (a, b) 都有直接约束 a→b，那么 a 必须紧接在 b 前面，整个序列就被完全确定了。

**Q3: 如果 sequences 中包含无效元素（超出 [1,n]）怎么办？**
> 需要额外检查越界情况，直接返回 false。题目保证了输入合法所以不需要。

## 相关题型
- [210. 课程表 II](https://leetcode.com/problems/course-schedule-ii/) - 拓扑排序
- [269. 火星词典](https://leetcode.com/problems/alien-dictionary/) - 字符拓扑排序
