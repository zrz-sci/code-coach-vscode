# 331. 验证二叉树的前序序列化 (Medium)

**Tags:** Stack, Tree, String, Binary Tree

## 核心思路

利用**入度/出度平衡**或**栈模拟消除**来验证序列化字符串。核心观察：二叉树中每个非空节点贡献 1 个入度和 2 个出度，每个空节点贡献 1 个入度和 0 个出度。合法序列化在遍历结束时入度总和 = 出度总和，且中间任何时刻出度 >= 入度。

## 思维链

1. 不能建树，需要纯字符串/数值层面的判断方法
2. 观察二叉树的入出度关系：根节点出度 2、入度 0；其他节点入度 1、出度 2（非空）或 0（空）
3. 用 `diff = 出度 - 入度` 追踪可用"槽位"：初始 diff=1（根占一个槽），非空节点消耗 1 个槽位并产生 2 个新槽位(diff += 1)，空节点只消耗 1 个槽位(diff -= 1)
4. 遍历中 diff 不能降到 0 以下（中间不能提前结束），结束时 diff 必须恰好为 0

## 解法概览

| 解法 | 时间 | 空间 | 推荐 |
|------|------|------|------|
| 入度出度差 (diff) | O(n) | O(1) | ⭐ |
| 栈模拟消除 | O(n) | O(n) | ⭐ |

## 关键提示

1. **diff 初始值为 1**：根节点需要消耗 1 个槽位，所以初始有 1 个可用槽位
2. **遇到非空节点**：diff 先 -1（消耗一个槽位进入），再 +2（产生左右两个子槽位），净效果 diff += 1
3. **遇到空节点 `#`**：diff -= 1（消耗一个槽位，不产生新槽位）
4. **中间 diff < 0 立即 false**：说明"悬空"了，多出的节点无处可放
5. **结束时 diff != 0 返回 false**：说明还有未填充的空位

## 解法详解

### 解法一：入度出度差 (推荐，O(1) 空间)

```cpp
class Solution {
public:
    bool isValidSerialization(string preorder) {
        int diff = 1; // 可用槽位数
        
        int i = 0, n = preorder.size();
        while (i < n) {
            if (diff == 0) return false; // 中途槽位用完但还有节点
            
            if (preorder[i] == ',') {
                i++;
            } else if (preorder[i] == '#') {
                diff--; // 空节点消耗一个槽位
                i++;
            } else {
                // 数字节点：可能多位数
                while (i < n && preorder[i] != ',') i++;
                diff++; // 消耗 1 个槽位 + 产生 2 个 = 净 +1
            }
        }
        
        return diff == 0;
    }
};
```

### 解法二：栈模拟消除

```cpp
class Solution {
public:
    bool isValidSerialization(string preorder) {
        // 将序列分割为 tokens
        vector<string> tokens;
        stringstream ss(preorder);
        string token;
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        stack<string> stk;
        for (auto& t : tokens) {
            stk.push(t);
            // 栈顶模式: [数字, #, #] → 消除为 #
            while (stk.size() >= 3) {
                auto top1 = stk.top(); stk.pop();
                auto top2 = stk.top(); stk.pop();
                auto top3 = stk.top(); stk.pop();
                if (top1 == "#" && top2 == "#" && top3 != "#") {
                    stk.push("#"); // 替换为空节点
                } else {
                    stk.push(top3);
                    stk.push(top2);
                    stk.push(top1);
                    break;
                }
            }
        }
        
        return stk.size() == 1 && stk.top() == "#";
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 原因 |
|----------|----------|------|
| ✗ diff 初始为 0 | ✓ diff 初始为 1 | 根节点需要 1 个槽位 |
| ✗ 只在结束时检查 diff==0 | ✓ 中间也要检查 diff >= 0 | "9,#,#,1" 中间 diff 已归零但后面还有节点 |
| ✗ 假设节点值只有一位 | ✓ 处理多位数字 (如 "100") | 题目说值范围 [0,100] |
| ✗ 用 `preorder[i] != '#'` 判断数字 | ✓ 还要跳过逗号 | 逗号不是节点 |

## 面试追问

**Q1: 入度出度方法的数学原理是什么？**
→ 对于任何 n 个节点的二叉树，总出度 = 2n（每个节点两个子指针），总入度 = n-1（除根外每个节点有一条入边），空节点数 = n+1。总共 2n+1 个元素（n 个节点 + n+1 个空），总消耗 2n+1 个槽位，总产生 2n 个槽位。初始给 1 个，正好 1 + 2n - (2n+1) = 0。

**Q2: 如果是后序序列化呢？**
→ 反转序列后变成"镜像前序"，应用相同逻辑。或者修改入出度关系：后序中先遇到叶子/空节点。

**Q3: 能否用递归下降解析？**
→ 可以，定义 `parse()` 消耗一个节点：若为 `#` 返回 true，否则递归解析左子树和右子树。最终检查是否恰好消耗完所有 token。

## 相关题型

- [297. 二叉树的序列化与反序列化](https://leetcode.com/problems/serialize-and-deserialize-binary-tree/) - 前序序列化/反序列化
- [449. 序列化和反序列化二叉搜索树](https://leetcode.com/problems/serialize-and-deserialize-bst/) - BST 序列化
- [606. 根据二叉树创建字符串](https://leetcode.com/problems/construct-string-from-binary-tree/) - 前序表示
