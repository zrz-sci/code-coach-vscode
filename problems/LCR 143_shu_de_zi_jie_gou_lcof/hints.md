# LCR 143. 子结构判断 (Tree Substructure)

## 核心思路

判断树 B 是否为树 A 的**子结构**。这需要**两层递归**：外层递归遍历 A 的每个节点作为候选起点，内层递归从该起点开始逐节点匹配 B。注意"子结构"和"子树"不同——子结构只要求 B 匹配到的部分和 A 相同，B 的叶子节点对应的 A 节点可以还有子节点。

## 思维链

1. **读完题第一反应**：遍历 A 的每个节点，检查"以这个节点为根，是否和 B 的结构匹配"。

2. **匹配的定义**：从 A 的某个节点开始，B 的每个节点都能在 A 中找到对应位置，且值相等。**B 的叶子处不需要 A 也是叶子**（这是子结构和子树的区别）。

3. **两个递归函数**：
   - `isSubStructure(A, B)`：在 A 中找起点——当前节点试一下，不行就在左子树找，再不行就在右子树找。
   - `match(A, B)`：从当前起点开始匹配——B 空了说明匹配成功，A 空了但 B 没空说明失败，值不等也失败。

4. **空树特判**：题目明确说"空树不是任何树的子结构"，所以 B 为空时直接返回 false。

5. **用 ASCII 树图理解**：

```
// 示例：A = [3,6,7,1,8], B = [6,1]
//
//     A:           B:
//       3            6
//      / \          /
//     6   7        1
//    / \
//   1   8
//
// 遍历 A 的每个节点作为起点：
//   起点 3: match(3, 6) → 3 != 6 → false
//   起点 6: match(6, 6) → 6 == 6 ✓
//           → match(1, 1) → 1 == 1 ✓
//             → match(null, null) → B 为空 → true ✓
//           → match(8, null) → B 为空 → true ✓
//           → 全部匹配 → true!
//
// 注意：A 中的节点 6 有右子节点 8，但 B 中的节点 6 没有右子节点。
// 这没关系！子结构不要求 B 的叶子对应 A 的叶子。
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 双重递归 (DFS) | 外层找起点，内层验证匹配 | O(m * n) | O(max(m, n)) | ⭐ 必须写出 |
| 序列化 + 字符串匹配 | 前序序列化后用 KMP | O(m + n) | O(m + n) | 理论最优但实现复杂 |

> m = A 的节点数，n = B 的节点数

## 关键提示

1. **子结构 vs 子树——最重要的区别**：

```
// 子结构（本题）：B 的叶子节点对应的 A 节点可以有子节点
//
//   A:       B:        子结构匹配？ → YES
//     1        1       B 的 "2" 是叶子，但 A 的 "2" 有子节点 4、5
//    / \      /        这没关系，因为子结构只要求 B 覆盖到的部分匹配
//   2   3    2
//  / \
// 4   5
//
// 子树（LeetCode 572）：B 必须和 A 的某个子树完全一致
//
//   A:       B:        子树匹配？ → NO
//     1        2       因为 A 中以 2 为根的子树是 {2,4,5}
//    / \      / \      而 B 是 {2} 或 {2,4,5}
//   2   3    ?   ?     B 必须包含所有子节点才算子树
//  / \
// 4   5
```

2. **match 函数中 B 为 null 的含义**：如果 B 已经遍历完了（null），说明 B 的所有节点都匹配成功了，返回 true。这正是子结构和子树的区别所在——子树需要同时检查 A 也是 null。

3. **isSubStructure 函数中的短路求值**：`match(A, B) || isSubStructure(A->left, B) || isSubStructure(A->right, B)`。如果 match 返回 true，后面的递归不会执行（短路优化）。

4. **为什么不能在 match 中处理"找起点"的逻辑？** 因为找起点和匹配是两个不同的逻辑：找起点是"A 的某个节点值等于 B 的根值"，匹配是"从这个起点开始逐节点比较"。混在一起会导致逻辑混乱。

5. **时间复杂度的理解**：外层递归遍历 A 的每个节点（m 个），对每个节点调用 match（最多比较 B 的所有 n 个节点）。所以最坏情况 O(m * n)。最好情况下 match 很快失败（值不等），远好于最坏情况。

## 解法详解

### 解法1: 双重递归 (DFS) — O(m*n) / O(max(m,n)) ⭐ 面试首选

**两个函数的分工**：

```
isSubStructure(A, B):  "在 A 中找到一个起点，使得从这个起点开始能匹配 B"
  → 遍历 A 的每个节点，对每个节点调用 match

match(A, B):  "从 A 的当前节点和 B 的当前节点开始，逐节点比较"
  → B 空了 → true（B 匹配完了）
  → A 空了 → false（A 不够用）
  → 值不等 → false
  → 递归比较左右子树
```

**决策流程图**：
```
// isSubStructure(A, B):
//
//   A 或 B 为空？ ──Yes──→ return false
//        │No
//        ▼
//   match(A, B) 成功？ ──Yes──→ return true
//        │No
//        ▼
//   isSubStructure(A->left, B) 成功？ ──Yes──→ return true
//        │No
//        ▼
//   isSubStructure(A->right, B) 成功？ ──Yes──→ return true
//        │No
//        ▼
//   return false
//
//
// match(A, B):
//
//   B 为空？ ──Yes──→ return true  （B 匹配完了）
//      │No
//      ▼
//   A 为空？ ──Yes──→ return false  （A 不够用了）
//      │No
//      ▼
//   A->val != B->val？ ──Yes──→ return false
//      │No (值相等)
//      ▼
//   return match(A->left, B->left) && match(A->right, B->right)
```

```cpp
class Solution {
public:
    // 外层递归：在 A 中找起点
    bool isSubStructure(TreeNode* A, TreeNode* B) {
        // 空树特判：空树不是任何树的子结构
        if (!A || !B) return false;
        
        // 三种可能（短路求值）：
        // 1. 以 A 的根节点为起点，能匹配 B
        // 2. B 是 A 的左子树的子结构
        // 3. B 是 A 的右子树的子结构
        return match(A, B) 
            || isSubStructure(A->left, B) 
            || isSubStructure(A->right, B);
    }
    
private:
    // 内层递归：从当前起点逐节点匹配
    bool match(TreeNode* A, TreeNode* B) {
        // B 遍历完了 → 匹配成功（子结构的关键！）
        if (!B) return true;
        
        // A 已经到底但 B 还没匹配完 → 失败
        if (!A) return false;
        
        // 当前节点值不等 → 失败
        if (A->val != B->val) return false;
        
        // 递归匹配左右子树（两边都要匹配才算成功）
        return match(A->left, B->left) && match(A->right, B->right);
    }
};
```

**逐步模拟**（A = [3,6,7,1,8], B = [6,1]）：

```
// 树 A:                树 B:
//       3                6
//      / \              /
//     6   7            1
//    / \
//   1   8

isSubStructure(3, 6):
  match(3, 6):
    3 != 6 → false
  isSubStructure(6, 6):        ← 递归到左子树
    match(6, 6):
      6 == 6 ✓
      match(1, 1):             ← 匹配左子树
        1 == 1 ✓
        match(null, null):     ← B 的左子树为空
          B 为空 → true ✓
        match(null, null):     ← B 的右子树为空
          B 为空 → true ✓
        → true
      match(8, null):          ← B 的右子树为空
        B 为空 → true ✓       ← 子结构的关键：B的叶子不要求A也是叶子
      → true && true = true
    → true!
  → return true

答案: true ✓
```

---

### 解法2: 序列化 + 字符串匹配（理论方案）

**另一种思路**：将 A 和 B 都序列化为字符串，然后用 KMP 或其他字符串匹配算法检查 B 的序列化结果是否是 A 的子串。

**问题**：子结构匹配比子字符串匹配更复杂——B 的叶子节点处 A 可以有更多节点。需要特殊的序列化方式（标记 B 的叶子终止符）。实现复杂，面试中不推荐。

```
// 序列化思路（概念性）：
// A 序列化: "3(6(1(#,#),8(#,#)),7(#,#))"
// B 序列化: "6(1,"    ← 需要特殊处理叶子终止
//
// 这种方法在实际中很少使用，因为处理子结构的终止条件非常 tricky。
// 面试中提一嘴"理论上可以用序列化+KMP做到 O(m+n)"即可。
```

## 解法对比

|  | 解法1: 双重递归 | 解法2: 序列化+KMP |
|---|---|---|
| 时间 | O(m * n) | O(m + n) 理论 |
| 空间 | O(max(m, n)) 递归栈 | O(m + n) 字符串 |
| 代码难度 | ⭐ 简单清晰 | 复杂（序列化处理） |
| 面试推荐 | ⭐ 首选且唯一推荐 | 仅理论讨论 |
| 处理子结构 | 天然支持（B=null→true） | 需要特殊终止符 |

## 易错点

1. **空树特判遗漏：B 为空时应返回 false**
   ```cpp
   // ✗ 错误：B 为空时没有特判
   bool isSubStructure(TreeNode* A, TreeNode* B) {
       if (!A) return false;
       // 漏了 B 为空的情况！题目说空树不是子结构
       return match(A, B) || ...;
   }
   
   // ✓ 正确：A 或 B 为空都返回 false
   if (!A || !B) return false;
   ```

2. **子结构和子树的 match 函数搞混**
   ```cpp
   // ✗ 子树的写法（LeetCode 572）：要求 A 和 B 同时到底
   bool match(TreeNode* A, TreeNode* B) {
       if (!A && !B) return true;   // 两个都空才算匹配
       if (!A || !B) return false;  // 一个空一个不空就不匹配
       ...
   }
   
   // ✓ 子结构的写法（本题）：B 空了就算匹配成功
   bool match(TreeNode* A, TreeNode* B) {
       if (!B) return true;   // B 匹配完了就行，不管 A 还有没有
       if (!A) return false;  // A 空了但 B 还没完，不行
       ...
   }
   ```

3. **把 match 和 isSubStructure 的逻辑混在一起**
   ```cpp
   // ✗ 错误：在 match 中尝试换起点
   bool match(TreeNode* A, TreeNode* B) {
       if (A->val != B->val) {
           // 不应该在这里尝试 A 的子节点作为新起点！
           return match(A->left, B) || match(A->right, B);
       }
       ...
   }
   
   // ✓ 正确：match 只负责匹配，isSubStructure 负责换起点
   // 两个函数的职责要清晰分离
   ```

4. **忘记 match 中的 && 写成了 ||**
   ```cpp
   // ✗ 错误：左右子树只要一个匹配就算成功
   return match(A->left, B->left) || match(A->right, B->right);
   
   // ✓ 正确：左右子树都要匹配才算成功
   return match(A->left, B->left) && match(A->right, B->right);
   ```

5. **递归终止条件的顺序搞反**
   ```cpp
   // ✗ 错误：先检查 A 再检查 B
   bool match(TreeNode* A, TreeNode* B) {
       if (!A) return false;  // 如果 B 也为空，这里应该返回 true！
       if (!B) return true;
       ...
   }
   // 当 A=null, B=null 时，先走到 if(!A) 返回 false，但正确答案是 true
   
   // ✓ 正确：先检查 B 再检查 A
   bool match(TreeNode* A, TreeNode* B) {
       if (!B) return true;   // B 空了 → 匹配成功
       if (!A) return false;  // A 空了但 B 还没 → 失败
       ...
   }
   ```

## 面试追问

**Q1（基础理解）**：子结构和子树有什么区别？
> 子结构只要求 B 覆盖到的部分和 A 匹配，B 的叶子对应的 A 节点可以有更多子节点。子树则要求完全一致——B 的叶子对应的 A 节点也必须是叶子。在代码上，区别体现在 match 函数：子结构 `if (!B) return true`，子树 `if (!A && !B) return true`。

**Q2（复杂度分析）**：为什么时间复杂度是 O(m*n) 而不是 O(m+n)？
> 外层 isSubStructure 遍历 A 的所有 m 个节点。对每个节点，match 函数最多访问 B 的所有 n 个节点。最坏情况：A 是一条链（所有值都等于 B 的根值），每个节点都要完整匹配一次 B。所以最坏 O(m*n)。平均情况下远好于此。

**Q3（变体）**：如果改成判断 B 是否是 A 的子树呢？（→ LeetCode 572）
> match 函数改成要求 A 和 B 同时到底：`if (!A && !B) return true; if (!A || !B) return false;`。或者用序列化方式：将两棵树都序列化（包含 null 标记），然后检查 B 的序列化是否是 A 的子串。

**Q4（优化思考）**：能否优化到 O(m+n)？
> 理论上可以用序列化 + KMP 字符串匹配做到 O(m+n)，但对于子结构问题，序列化方式需要特殊处理（B 的叶子终止符），实现非常复杂。实际面试中 O(m*n) 的双重递归解法完全够用。

**Q5（拓展：如果节点值有重复）**：如果 A 中有大量值和 B 根相同的节点，性能如何？
> 每个值相同的节点都会触发一次 match 调用。最坏情况就是 A 的所有节点值都等于 B 的根值——此时 match 被调用 m 次，每次比较 n 个节点，总共 O(m*n)。优化思路：在 match 失败后记忆化（但树结构的记忆化比较复杂，通常不值得）。

**Q6（递归转迭代）**：能否不用递归实现？
> 可以，用 BFS（队列）遍历 A 的每个节点替代外层递归，用栈模拟内层 match 递归。但代码会更长且不易读，面试中不推荐。递归版本更清晰地表达了"找起点 + 逐节点匹配"的两层逻辑。

## 相关题型

- **LeetCode 572 另一棵树的子树** — "子树"版本：B 必须和 A 的某个子树完全一致（包括叶子）。match 函数需修改为同时到底才算匹配。
- **LeetCode 100 相同的树** — match 函数的特例：判断两棵树是否完全相同。是本题 match 逻辑的基础。
- **LeetCode 101 对称二叉树** — 同样的递归匹配思路，但比较的是镜像对称而不是直接匹配。
- **LeetCode 226 翻转二叉树** — 树的基础递归操作，巩固递归思维。
- **LeetCode 104 二叉树的最大深度** — 树递归入门，理解递归终止条件和返回值。
- **剑指 Offer 27 二叉树的镜像** — 和本题同源的剑指 Offer 系列，递归操作树。
