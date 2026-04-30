# 269. 火星词典 (Alien Dictionary)

## 核心思路

本质上是**从有序单词列表中提取字母之间的偏序关系，然后对这些关系做拓扑排序**。每对相邻单词的第一个不同字符，给出了一条"谁排在谁前面"的有向边。

## 思维链

1. **读完题第一反应**：单词已经按火星语字典序排好了，我需要从中"逆向工程"出字母的顺序。但字母的顺序信息藏在哪里？

2. **关键观察 — 相邻单词的比较**：就像英语字典里 "abc" 排在 "abd" 前面能告诉我们 c < d 一样，比较相邻两个单词，找到第一个不同的字符位置，就得到一条顺序关系。例如 "wrt" 和 "wrf"，前两个字符相同，第三个 t ≠ f → t 排在 f 前面。

3. **建模为图问题**：每个字母是一个节点，每条"a 排在 b 前面"的关系是一条有向边 a→b。我们需要找到一个线性顺序使得所有边的方向一致 → **这就是拓扑排序！**

4. **检测非法情况**：什么时候无解？(a) 图中有环 → 矛盾的顺序关系 (b) 长单词排在短单词前面且短单词是长单词的前缀（如 "abc" 排在 "ab" 后面是合法的，但 "abc" 排在 "ab" 前面就非法）。

5. **别忘了孤立字母**：有些字母可能不参与任何比较关系（没有入边也没有出边），但它们确实出现在词典中，需要包含在结果里。

6. **实现选择**：拓扑排序可以用 BFS（Kahn 算法，入度法）或 DFS（后序反转）。BFS 更直观且容易检测环。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS 拓扑排序 (Kahn) | 建图 + 入度表 + BFS | O(C) | O(1) 或 O(U+E) | ⭐ 必须写出 |
| DFS 拓扑排序 | 建图 + DFS 后序 + 反转 | O(C) | O(1) 或 O(U+E) | 能说出即可 |

> C = 所有单词的总字符数，U = 出现的不同字母数（≤26），E = 提取的边数（≤U²）

## 关键提示

- **提示1**：只需比较**相邻**两个单词。非相邻单词的关系可以通过传递性得出，拓扑排序会自动处理。

- **提示2**：比较两个单词时，只有**第一个不同字符**能提供信息。之后的字符无法确定顺序（因为前面已经决定了大小）。

- **提示3**：别忘了一个特殊非法情况 — `["abc", "ab"]`，前缀相同但长的在前面，这违反了字典序规则。

- **提示4**：结果必须包含**所有出现过的字母**，不只是有偏序关系的那些。

- **提示5 (图解建图过程)**：
```
words = ["wrt", "wrf", "er", "ett", "rftt"]

比较 "wrt" vs "wrf" → t 在 f 前面 → t → f
比较 "wrf" vs "er"  → w 在 e 前面 → w → e
比较 "er"  vs "ett" → r 在 t 前面 → r → t
比较 "ett" vs "rftt"→ e 在 r 前面 → e → r

有向图:
  w → e → r → t → f

入度: w:0, e:1, r:1, t:1, f:1
BFS: w → e → r → t → f
结果: "wertf"
```

## 解法详解

### 解法1: BFS 拓扑排序 (Kahn 算法) — O(C) / O(U+E) ⭐ 面试首选

**思考过程**：

我们要找字母的线性顺序，且必须满足若干"a 在 b 前面"的约束。这是经典的拓扑排序场景。

BFS 版拓扑排序（Kahn 算法）的好处：
- 直觉清晰：先处理没有前驱的节点（入度为0），然后删除其出边，产生新的入度为0的节点
- 天然检测环：如果最终处理的节点数 < 总节点数，说明有环

**建图过程**：
```
// 比较相邻单词 words[i] 和 words[i+1]:
// 找到第一个不同字符的位置 j
// words[i][j] → words[i+1][j] 是一条有向边
// 
// 特殊情况: 如果所有字符都相同但 words[i] 更长
// → 非法! 例如 ["abc", "ab"]
```

**BFS 拓扑排序过程**（以示例1为例）：
```
// 图:  w→e, e→r, r→t, t→f
// 入度: {w:0, e:1, r:1, t:1, f:1}
//
// 初始队列: [w]  (入度为0)
// 
// Step1: 弹出 w, 结果="w"
//        w→e: e 入度 1→0, 入队 → 队列: [e]
//
// Step2: 弹出 e, 结果="we"
//        e→r: r 入度 1→0, 入队 → 队列: [r]
//
// Step3: 弹出 r, 结果="wer"
//        r→t: t 入度 1→0, 入队 → 队列: [t]
//
// Step4: 弹出 t, 结果="wert"
//        t→f: f 入度 1→0, 入队 → 队列: [f]
//
// Step5: 弹出 f, 结果="wertf"
//        队列空, 5个字母全部处理 → 合法!
```

```cpp
class Solution {
public:
    string alienOrder(vector<string>& words) {
        // Step 1: 初始化 — 收集所有出现的字母，建立入度表
        unordered_map<char, unordered_set<char>> graph; // 邻接表
        unordered_map<char, int> indegree;              // 入度
        
        // 先把所有出现的字母都注册进来（入度初始化为0）
        for (const string& w : words) {
            for (char c : w) {
                if (indegree.find(c) == indegree.end()) {
                    indegree[c] = 0;
                }
            }
        }
        
        // Step 2: 建图 — 比较每对相邻单词
        for (int i = 0; i < (int)words.size() - 1; i++) {
            const string& w1 = words[i];
            const string& w2 = words[i + 1];
            
            // 特殊非法情况：w1 是 w2 的前缀但 w1 更长
            // 例如 ["abc", "ab"] → 不合法
            if (w1.size() > w2.size() && w1.substr(0, w2.size()) == w2) {
                return "";
            }
            
            // 找第一个不同字符
            int minLen = min(w1.size(), w2.size());
            for (int j = 0; j < minLen; j++) {
                if (w1[j] != w2[j]) {
                    // w1[j] 排在 w2[j] 前面 → 有向边 w1[j] → w2[j]
                    // 用 set 避免重复边
                    if (graph[w1[j]].find(w2[j]) == graph[w1[j]].end()) {
                        graph[w1[j]].insert(w2[j]);
                        indegree[w2[j]]++;
                    }
                    break; // 只有第一个不同字符有信息量！
                }
            }
        }
        
        // Step 3: BFS 拓扑排序
        queue<char> q;
        for (auto& [ch, deg] : indegree) {
            if (deg == 0) {
                q.push(ch);
            }
        }
        
        string result;
        while (!q.empty()) {
            char curr = q.front();
            q.pop();
            result += curr;
            
            for (char next : graph[curr]) {
                indegree[next]--;
                if (indegree[next] == 0) {
                    q.push(next);
                }
            }
        }
        
        // Step 4: 检测环 — 如果结果长度 < 总字母数，说明有环
        if (result.size() != indegree.size()) {
            return "";
        }
        
        return result;
    }
};
```

**关键点**：
- `break` 不能忘！找到第一个不同字符后就要停止，后续字符不提供有效信息。
- 用 `unordered_set` 存邻接表避免重复边，否则入度会多算。
- 前缀判断必须在找不同字符之前处理。

---

### 解法2: DFS 拓扑排序 — O(C) / O(U+E)

**从解法1的另一个角度**：拓扑排序除了 BFS（Kahn），还可以用 DFS 实现。DFS 的思路是：对每个未访问的节点做 DFS，在所有后继都访问完后（后序），把当前节点加入结果。最终反转结果即为拓扑序。

**环的检测**：DFS 中用三色标记法：
- 白色（0）= 未访问
- 灰色（1）= 正在访问（在当前递归栈中）
- 黑色（2）= 已完成

遇到灰色节点 = 有环！

```
// DFS 拓扑排序示意（同样的图 w→e→r→t→f）:
//
// 从 w 开始 DFS:
// visit(w) → visit(e) → visit(r) → visit(t) → visit(f)
//   f 无后继 → 后序加入 f
//   t 后继处理完 → 后序加入 t
//   r 后继处理完 → 后序加入 r
//   e 后继处理完 → 后序加入 e
//   w 后继处理完 → 后序加入 w
//
// 后序结果: [f, t, r, e, w]
// 反转: [w, e, r, t, f] → "wertf" ✓
```

```cpp
class Solution {
public:
    string alienOrder(vector<string>& words) {
        // Step 1: 建图（同 BFS 版）
        unordered_map<char, unordered_set<char>> graph;
        unordered_set<char> allChars;
        
        for (const string& w : words) {
            for (char c : w) {
                allChars.insert(c);
            }
        }
        
        for (int i = 0; i < (int)words.size() - 1; i++) {
            const string& w1 = words[i];
            const string& w2 = words[i + 1];
            
            if (w1.size() > w2.size() && w1.substr(0, w2.size()) == w2) {
                return "";
            }
            
            int minLen = min(w1.size(), w2.size());
            for (int j = 0; j < minLen; j++) {
                if (w1[j] != w2[j]) {
                    graph[w1[j]].insert(w2[j]);
                    break;
                }
            }
        }
        
        // Step 2: DFS 拓扑排序（三色标记）
        // 0=白(未访问), 1=灰(进行中), 2=黑(完成)
        unordered_map<char, int> color;
        for (char c : allChars) color[c] = 0;
        
        string result;
        bool hasCycle = false;
        
        // DFS 函数：返回 false 表示发现环
        function<void(char)> dfs = [&](char u) {
            if (hasCycle) return;
            color[u] = 1; // 标记为灰色（正在处理）
            
            for (char v : graph[u]) {
                if (color[v] == 1) {
                    // 遇到灰色 → 有环（回到了当前递归栈中的节点）
                    hasCycle = true;
                    return;
                }
                if (color[v] == 0) {
                    dfs(v);
                }
            }
            
            color[u] = 2; // 标记为黑色（处理完成）
            result += u;  // 后序：所有后继都处理完了才加入自己
        };
        
        for (char c : allChars) {
            if (color[c] == 0) {
                dfs(c);
                if (hasCycle) return "";
            }
        }
        
        // 后序的反转就是拓扑序
        reverse(result.begin(), result.end());
        return result;
    }
};
```

**关键点**：
- 后序加入 + 反转 = 拓扑序。为什么？因为后序保证了一个节点的所有后继都在它之前被加入，反转后就变成了所有后继都在它之后。
- 三色标记比简单的 visited 多一个状态，用来区分"正在处理的祖先"和"已经处理完的节点"。

---

## 解法对比

| | BFS (Kahn) | DFS 后序反转 |
|---|---|---|
| 核心思想 | 不断剥离入度为 0 的节点 | 后序遍历 + 反转 |
| 环检测 | 处理完的节点数 < 总数 | 遇到灰色节点 |
| 代码量 | 略多（维护入度表） | 略少（递归自然） |
| 直觉性 | ⭐ 更直观，推荐面试用 | 需要理解后序反转 |
| 结果唯一性 | BFS 用 priority_queue 可得字典序最小 | 不容易控制顺序 |

## 易错点

1. **✗ 忘记 `break`** — 找到第一个不同字符后继续比较后面的字符
   ```cpp
   // 错误：
   for (int j = 0; j < minLen; j++) {
       if (w1[j] != w2[j]) {
           graph[w1[j]].insert(w2[j]);
           // 没有 break！会提取出错误的关系
       }
   }
   ```
   → ✓ 找到第一个不同字符后必须 `break`，后续字符的顺序无法从这对单词推导。

2. **✗ 漏掉前缀非法情况** — `["abc", "ab"]` 应该返回 `""`
   ```cpp
   // 错误：只找不同字符，没有特判前缀
   // 当 w1 比 w2 长且 w2 是 w1 的前缀时，for 循环正常结束不会加任何边
   // 但其实这个排列本身就是非法的！
   ```
   → ✓ 在比较之前先检查：`w1.size() > w2.size() && w1 的前缀 == w2` → return ""

3. **✗ 重复边导致入度多算**
   ```cpp
   // 错误：用 vector 存邻接表
   graph[w1[j]].push_back(w2[j]);
   indegree[w2[j]]++;
   // 如果 "t→f" 这条边被多对单词提取出来，入度会多加
   ```
   → ✓ 用 `unordered_set` 存邻接表，或在加边前检查是否已存在

4. **✗ 遗漏孤立字母** — 只把有边的字母加入图
   ```cpp
   // 错误：只在建边时注册字母
   // 如果某个字母只出现在单词中间但从不参与任何"第一不同字符"比较
   // 它就不在图中 → 结果缺少这个字母
   ```
   → ✓ 初始化时遍历所有单词的所有字符，全部加入入度表（入度为0）

## 面试追问

**Q1: 如何从单词列表中提取字母的顺序关系？**

> 比较每对相邻单词，找第一个不同字符。例如 "wrt" vs "wrf" → t < f。这是因为字典序比较就是这么工作的：共同前缀相同，第一个不同位置决定顺序。

**Q2: 什么情况下无解？如何检测？**

> 两种情况：(1) 顺序矛盾形成环，例如推出 a < b 且 b < a → BFS 中表现为结果长度 < 总字母数；(2) 前缀矛盾，如 ["abc", "ab"]，长的在前面违反字典序。

**Q3: 如果题目要求在多个合法答案中返回字典序最小的，怎么改？**

> 把 BFS 的普通队列换成**最小堆（priority_queue with greater<>）**，每次从入度为 0 的节点中选字母最小的。这保证了在拓扑序的自由度中选择了字典序最小的排列。

**Q4: 复杂度分析 — 为什么说时间是 O(C)？**

> C 是所有单词的总字符数。建图时我们遍历每对相邻单词，总比较次数不超过 C。图中节点最多 26 个（字母），边最多 min(C, 26²) 条。拓扑排序是 O(V+E) = O(26 + E) ⊆ O(C)。

## 相关题型

- **210. 课程表 II (Course Schedule II)** — 完全相同的拓扑排序模板，区别在于本题需要自己建图（从单词列表提取边），210 题边已经直接给出。
- **207. 课程表 (Course Schedule)** — 只需判断有无环（拓扑排序能否完成），不需要输出顺序。复用本题的环检测逻辑。
- **953. 验证外星语词典 (Verifying an Alien Dictionary)** — 反向问题！给你字母顺序，验证单词列表是否有序。本题是从有序列表反推字母顺序。