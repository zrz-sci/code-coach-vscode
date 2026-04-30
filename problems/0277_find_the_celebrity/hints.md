# 277. 搜寻名人

## 核心思路

本质上是在 n 个人中找到一个"入度为 n-1、出度为 0"的节点，但你不能直接访问邻接矩阵，只能通过 `knows(a, b)` 逐个查询。核心在于：**每次调用 `knows(a, b)` 都能淘汰一个候选人**。

## 思维链

1. **读完题第一反应**：名人的定义是"所有人都认识他，他不认识任何人"。最朴素的做法就是对每个人检查是否满足这两个条件——遍历所有其他人问两个方向。
2. **暴力解的瓶颈**：对每个候选人 i，需要检查 n-1 个人是否认识 i，以及 i 是否不认识他们，总共 O(n²) 次调用。
3. **关键观察——一次询问淘汰一人**：调用 `knows(a, b)` 时：
   - 如果 a 认识 b → a 一定**不是**名人（名人不认识任何人）
   - 如果 a 不认识 b → b 一定**不是**名人（名人被所有人认识）
   - 无论哪种结果，都能淘汰一个人！
4. **用 n-1 次询问找到唯一候选**：从 0 号开始，逐个和下一个人比较，每次淘汰一个，n-1 次后剩下唯一候选人。
5. **再用最多 2(n-1) 次验证候选人**：候选人不一定是名人（可能根本没有名人），需要遍历所有其他人验证两个条件。
6. **总计最多 3(n-1) 次调用**，满足 Follow-up 的 3n 要求。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 对每个人检查是否满足名人条件 | O(n²) | O(1) | 能说出即可 |
| 候选人淘汰 | 两两比较淘汰 + 最终验证 | O(n) | O(1) | ⭐ 必须写出 |
| 栈方法 | 栈中两两比较淘汰 | O(n) | O(n) | 加分项（理解即可） |

## 关键提示

1. **一次 `knows` 调用能给你什么信息？** 想想 `knows(a, b) = true` 和 `false` 分别能排除谁。

2. **类比"擂台赛"**：想象 n 个选手打擂台，每轮比一次就淘汰一个人，n-1 轮后留下的是"最有可能"的那个——但还需要验证。

3. **为什么候选人可能不是名人？** 因为淘汰过程只保证候选人没被"认识别人"这条规则淘汰，但没验证"所有人都认识他"。

4. **Follow-up 的 3n 限制**：第一遍 n-1 次找候选，第二遍最多 2(n-1) 次验证，总计 3(n-1) < 3n。

5. **图的视角理解**：

```
// 示例1: graph = [[1,1,0],[0,1,0],[1,1,1]]
//
//   0 ---认识--→ 1        0 ←--不认识--- 1
//   2 ---认识--→ 1        2 ←--不认识--- 1
//   2 ---认识--→ 0        0 ←--不认识--- 2
//
// 1号: 入度=2(所有人认识他), 出度=0(不认识任何人) → 名人!
//
// 淘汰过程:
// candidate=0, 问 knows(0,1)=true → 0认识1, 淘汰0 → candidate=1
// candidate=1, 问 knows(1,2)=false → 1不认识2, 淘汰2 → candidate=1
// 候选人=1, 验证: knows(0,1)=true ✓, knows(1,0)=false ✓
//                  knows(2,1)=true ✓, knows(1,2)=false ✓ → 返回1
```

## 解法详解

### 解法1: 暴力枚举 — O(n²) / O(1)

**思考过程**: 最直接的思路——逐个检查每个人是否满足名人定义。对于候选人 i，检查所有 j≠i：是否 `knows(j, i) == true` 且 `knows(i, j) == false`。

```cpp
/* The knows API is defined for you.
      bool knows(int a, int b); */

class Solution : public Relation {
public:
    int findCelebrity(int n) {
        for (int i = 0; i < n; i++) {
            bool isCelebrity = true;
            for (int j = 0; j < n; j++) {
                if (i == j) continue;
                // 名人条件: j认识i 且 i不认识j
                if (!knows(j, i) || knows(i, j)) {
                    isCelebrity = false;
                    break; // 提前终止，但最坏仍O(n²)
                }
            }
            if (isCelebrity) return i;
        }
        return -1;
    }
};
```

**关键点**: 
- 内层循环可以提前 break，但最坏情况（如名人是最后一个人，或不存在名人）仍需 O(n²) 次 `knows` 调用。
- 这个解法的瓶颈：**我们在盲目地逐个检查，没有利用每次询问能淘汰一个候选人的信息。**

---

### 解法2: 候选人淘汰法 — O(n) / O(1) ⭐ 面试首选

**从解法1优化**: 暴力法的问题是没有利用 `knows` 调用的"副产品"。关键洞察是：

> **每调用一次 `knows(a, b)`，无论结果如何，都能确定 a 和 b 中至少一个不是名人。**

这就像一场淘汰赛：两两PK，每轮淘汰一人，n-1 轮后剩下唯一候选。

```
// 淘汰逻辑:
// knows(a, b) = true  → a认识b → a不可能是名人 → 淘汰a, 保留b
// knows(a, b) = false → a不认识b → b不可能是名人(名人应被所有人认识) → 淘汰b, 保留a
```

```cpp
/* The knows API is defined for you.
      bool knows(int a, int b); */

class Solution : public Relation {
public:
    int findCelebrity(int n) {
        // === 第一步: 找候选人 (n-1 次调用) ===
        int candidate = 0;
        for (int i = 1; i < n; i++) {
            if (knows(candidate, i)) {
                // candidate认识i → candidate不是名人 → 换i当候选
                candidate = i;
            }
            // 否则: candidate不认识i → i不是名人 → candidate不变
        }
        
        // === 第二步: 验证候选人 (最多 2(n-1) 次调用) ===
        for (int i = 0; i < n; i++) {
            if (i == candidate) continue;
            // 名人必须: 所有人都认识他 且 他不认识任何人
            if (!knows(i, candidate) || knows(candidate, i)) {
                return -1;
            }
        }
        
        return candidate;
    }
};
```

**为什么第一步后 candidate 是唯一可能的名人？**

```
// 设最终候选人是 c，那么:
// - 所有在 c 之前被淘汰的人: 因为他们"认识某人"而被淘汰 → 不是名人
// - 所有在 c 之后的人 i (i > c): 因为 knows(c, i)=false → c不认识i → i不被所有人认识 → 不是名人  
// - 所以如果存在名人，只可能是 c
//
// 但 c 也不一定是名人! 例如:
//   0不认识1, 0不认识2 → candidate=0
//   但可能 2 也不认识 0 → 0 不被所有人认识 → 没有名人
//   所以第二步验证是必须的
```

**关键点**: 第一步只用了 n-1 次 `knows` 调用，第二步最多 2(n-1) 次，总计不超过 3(n-1) 次，满足 Follow-up。

---

### 解法3: 栈方法 — O(n) / O(n)

**不同视角**: 用栈来组织"两两比较淘汰"的过程。把所有人入栈，每次弹出两个人比较，淘汰一个，胜者压回栈。最后栈中只剩一个候选人。

```cpp
/* The knows API is defined for you.
      bool knows(int a, int b); */

class Solution : public Relation {
public:
    int findCelebrity(int n) {
        // 所有人入栈
        stack<int> stk;
        for (int i = 0; i < n; i++) {
            stk.push(i);
        }
        
        // 两两PK淘汰
        while (stk.size() > 1) {
            int a = stk.top(); stk.pop();
            int b = stk.top(); stk.pop();
            if (knows(a, b)) {
                stk.push(b); // a认识b → 淘汰a
            } else {
                stk.push(a); // a不认识b → 淘汰b
            }
        }
        
        int candidate = stk.top();
        
        // 验证
        for (int i = 0; i < n; i++) {
            if (i == candidate) continue;
            if (!knows(i, candidate) || knows(candidate, i)) {
                return -1;
            }
        }
        
        return candidate;
    }
};
```

**关键点**: 
- 栈方法和解法2的淘汰逻辑完全一样，只是组织形式不同。
- 解法2 是线性扫描（类似"打擂台"），更简洁且 O(1) 空间。
- 栈方法的好处是更直观地体现"两两比较"的思想，在面试中容易解释。

## 解法对比

| | 暴力枚举 | 候选人淘汰 ⭐ | 栈方法 |
|---|---|---|---|
| `knows` 调用次数 | O(n²) 最坏 | ≤ 3(n-1) | ≤ 3(n-1) |
| 空间 | O(1) | O(1) | O(n) |
| 思路复杂度 | 低 | 中 | 中 |
| 面试推荐 | 起步思路 | **首选** | 备选 |

**候选人淘汰法 vs 栈方法**：本质相同，都是"每次淘汰一人"。候选人淘汰法更简洁（不需要额外数据结构），面试中优先写这个。

## 易错点

1. **✗ 第一步找到候选人后直接返回，不做验证**
   ```cpp
   // 错误: 直接返回 candidate
   return candidate;
   ```
   **✓ 必须做第二步验证**
   ```cpp
   // 候选人只是"最有可能"，不一定是名人
   // 例如 n=2, knows(0,1)=false, knows(1,0)=false → candidate=0 但没有名人
   for (int i = 0; i < n; i++) { /* 验证... */ }
   ```

2. **✗ 验证时只检查一个方向**
   ```cpp
   // 错误: 只检查别人是否认识候选人
   if (!knows(i, candidate)) return -1;
   ```
   **✓ 两个方向都要检查**
   ```cpp
   // 名人条件有两个: (1)所有人认识他 (2)他不认识任何人
   if (!knows(i, candidate) || knows(candidate, i)) return -1;
   ```

3. **✗ 验证时忘记跳过 candidate 自身**
   ```cpp
   // 错误: 没有 if (i == candidate) continue;
   // knows(candidate, candidate) 总是 true，会误判为"候选人认识某人"
   ```

4. **✗ 候选人初始化为 -1 或其他无效值**
   ```cpp
   // 错误: int candidate = -1;
   // 正确: int candidate = 0; // 从第一个人开始
   ```

## 面试追问

**Q1: 暴力解的时间复杂度是多少？能优化吗？**
> O(n²)，因为对每个候选人要检查 n-1 个人。关键观察是每次 `knows(a,b)` 调用能淘汰一个候选人，因此可以用 n-1 次调用找到唯一候选，再用 2(n-1) 次验证，总计 O(n)。

**Q2: 为什么候选人淘汰后还需要验证？能否省略验证？（Follow-up 核心）**
> 不能省略。第一遍只保证候选人没被"他认识别人"淘汰，但没验证"所有人都认识他"。例如 `knows(0,1)=false, knows(0,2)=false` 但 `knows(1,0)=false, knows(2,0)=false`，候选人是 0 但没人认识 0。

**Q3: 能否进一步优化验证步骤，使总调用次数 < 3(n-1)？**
> 可以小幅优化：在第一遍淘汰过程中，对已经获得的信息做记录（缓存 `knows` 结果），避免第二遍重复询问。例如淘汰过程中 `knows(candidate, i)=false` 已经知道了，验证时不需要再问。但渐近复杂度仍是 O(n)，只是常数更小。

**Q4: 如果有多个名人怎么办？**
> 不可能存在两个名人。假设 A 和 B 都是名人，则 A 认识 B（因为所有人认识名人 B）但 A 不认识任何人（名人定义）——矛盾。所以名人最多一个。

## 相关题型

- **LeetCode 997. 找到小镇的法官** — 几乎相同的问题，但直接给了 `trust` 数组而非交互 API。可以用入度/出度统计，也可以复用本题的淘汰思想。区别：997 可以直接遍历所有边，不需要交互。
- **LeetCode 1436. 旅行终点站** — 同样是找"入度>0、出度=0"的节点，用哈希集合即可。
- **拓扑排序问题** — 名人本质是 DAG 中入度=n-1、出度=0 的节点，理解入度/出度概念后可以迁移到拓扑排序类题目。