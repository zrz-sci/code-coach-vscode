# 457. 环形数组是否存在循环 (Circular Array Loop)

## 核心思路
快慢指针检测环：从每个未访问的起点出发，快指针走两步、慢指针走一步，相遇则有环。需额外检查：环内方向一致且长度 > 1。

## 思维链
1. 每个位置有唯一后继 → 函数式图/链表模型
2. 检测环 → Floyd 快慢指针
3. 额外约束：同方向 + 环长 > 1
4. 已检查过的路径标记为 0，避免重复访问 → O(n) 时间 O(1) 空间

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| Solution1 | 快慢指针 + 原地标记 ⭐ | O(n) | O(1) | 最优解 |
| Solution2 | DFS/模拟 + visited 数组 | O(n) | O(n) | 容易理解 |

## 关键提示
1. **取模处理**：`next = ((i + nums[i]) % n + n) % n`，处理负数取模
2. **同方向检查**：环中所有 nums 必须同号（全正或全负）
3. **自环排除**：长度为 1 的环不算，即 next == i 时跳过
4. **原地标记**：将已确认不在有效环中的节点标 0，避免重复

## 解法详解

### Solution 1: 快慢指针 + 原地标记 ⭐
**思路**：
1. 从每个未标记的位置出发，用快慢指针找环
2. 移动时检查方向一致性（与起始方向相同）
3. 检查自环（next == current 时该路径无效）
4. 找到环后返回 true；否则将路径上所有节点标为 0

```cpp
class Solution {
public:
    bool circularArrayLoop(vector<int>& nums) {
        int n = nums.size();
        auto nxt = [&](int i) {
            return ((i + nums[i]) % n + n) % n;
        };
        
        for (int i = 0; i < n; i++) {
            if (nums[i] == 0) continue;
            int slow = i, fast = i;
            // 方向一致性：与 nums[i] 同号
            while (nums[nxt(fast)] * nums[i] > 0 &&
                   nums[nxt(nxt(fast))] * nums[i] > 0) {
                slow = nxt(slow);
                fast = nxt(nxt(fast));
                if (slow == fast) {
                    // 检查环长 > 1
                    if (slow == nxt(slow)) break;
                    return true;
                }
            }
            // 标记路径上的节点为 0
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
```

### Solution 2: DFS + visited 数组
**思路**：对每个起点 DFS 沿链走，用 visited 数组记录当前轮次的访问状态。

```cpp
class Solution {
public:
    bool circularArrayLoop(vector<int>& nums) {
        int n = nums.size();
        vector<int> visited(n, 0);
        
        for (int i = 0; i < n; i++) {
            if (visited[i]) continue;
            int cur = i, mark = i + 1; // 用唯一标记区分不同轮次
            while (true) {
                if (visited[cur] == mark) {
                    // 检查环长 > 1
                    int nxt_c = ((cur + nums[cur]) % n + n) % n;
                    if (nxt_c != cur) return true;
                    break;
                }
                if (visited[cur] != 0) break; // 已被之前轮次处理
                visited[cur] = mark;
                int nxt_c = ((cur + nums[cur]) % n + n) % n;
                if (nums[nxt_c] * nums[i] <= 0) break; // 方向不一致
                cur = nxt_c;
            }
        }
        return false;
    }
};
```

## 易错点
- ✗ 取模写成 `(i + nums[i]) % n` → ✓ 需处理负数：`((i + nums[i]) % n + n) % n`
- ✗ 忘记排除自环（环长 == 1）→ ✓ 检查 `slow == nxt(slow)`
- ✗ 方向检查只看起终点 → ✓ 快慢指针每步都要验证方向一致性
- ✗ 标记时破坏了未处理的节点 → ✓ 只标记与当前起始方向相同的节点

## 面试追问
**Q1: 为什么需要方向一致性检查？**
A: 题目要求环中所有节点移动方向相同（全正/全负）。混合方向的环不算有效循环。

**Q2: 原地标记会不会影响后续判断？**
A: 不会。标记为 0 的节点已确认不在任何有效环中。后续遇到直接跳过。

**Q3: 时间复杂度为什么是 O(n)？**
A: 每个节点最多被访问和标记一次。一旦标记为 0 就不再参与后续循环。总操作次数 O(n)。

## 相关题型
- [141. 环形链表](https://leetcode.com/problems/linked-list-cycle/) - 快慢指针基础
- [142. 环形链表 II](https://leetcode.com/problems/linked-list-cycle-ii/) - 找环入口
- [287. 寻找重复数](https://leetcode.com/problems/find-the-duplicate-number/) - Floyd 算法变形
