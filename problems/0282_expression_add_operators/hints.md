# 282. Expression Add Operators (给表达式添加运算符)

**难度**: Hard | **标签**: Math, String, Backtracking

## 核心思路

本质是**回溯 + 乘法优先级处理**。在数字串的每个间隔处尝试插入 `+`、`-`、`*` 或不插入（拼接多位数），关键难点在于乘法的优先级——需要额外记录上一个操作数，以便遇到乘法时"撤销"上一步的加减再重新计算。

## 思维链

1. 数字串有 n-1 个间隔，每个间隔有 4 种选择（+, -, *, 拼接）-> 回溯搜索
2. 维护当前计算值 `curVal` 和上一个操作数 `prevOp`
3. 遇到 `+num`: curVal += num, prevOp = num
4. 遇到 `-num`: curVal -= num, prevOp = -num
5. 遇到 `*num`: curVal = curVal - prevOp + prevOp * num, prevOp = prevOp * num
6. 注意前导零：`"05"` 不合法，但单独的 `"0"` 合法
7. 注意溢出：使用 `long long`

```
回溯树示例 (num="123", target=6):

                  ""
         /        |        \
        1         12        123
      / | \      / | \
    1+2 1-2 1*2  12+3 12-3 12*3
    / | \
 1+2+3 1+2-3 1+2*3
  =6     =0    =7

找到: "1+2+3"=6, "1*2*3"=6
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法1: 回溯+prevOp | 记录前一操作数处理乘法优先级 | O(4^n) | O(n) | ⭐⭐⭐ |

## 关键提示

```
乘法优先级处理的核心：

假设当前表达式为 "2+3"，curVal=5, prevOp=3
现在要接 "*4":

错误做法: curVal = 5 * 4 = 20  (把整个表达式乘4)
正确做法: curVal = 5 - 3 + 3*4 = 14  (撤销+3，改为+3*4)

公式: curVal = curVal - prevOp + prevOp * curNum
       newPrevOp = prevOp * curNum
```

## 解法详解

### 解法1: 回溯 + prevOp 追踪 (标准解法)

```cpp
class Solution {
public:
    vector<string> addOperators(string num, int target) {
        vector<string> res;
        string path;
        dfs(num, target, 0, 0, 0, path, res);
        return res;
    }

private:
    void dfs(const string& num, int target, int idx,
             long long curVal, long long prevOp,
             string& path, vector<string>& res) {
        if (idx == num.size()) {
            if (curVal == target) {
                res.push_back(path);
            }
            return;
        }

        for (int i = idx; i < num.size(); i++) {
            // 防止前导零: "05" 不合法
            if (i > idx && num[idx] == '0') break;

            string curStr = num.substr(idx, i - idx + 1);
            long long curNum = stoll(curStr);

            int len = path.size();

            if (idx == 0) {
                // 第一个数字，不需要运算符
                path += curStr;
                dfs(num, target, i + 1, curNum, curNum, path, res);
                path.resize(len);
            } else {
                // 尝试 +
                path += "+" + curStr;
                dfs(num, target, i + 1, curVal + curNum, curNum, path, res);
                path.resize(len);

                // 尝试 -
                path += "-" + curStr;
                dfs(num, target, i + 1, curVal - curNum, -curNum, path, res);
                path.resize(len);

                // 尝试 *
                path += "*" + curStr;
                dfs(num, target, i + 1,
                    curVal - prevOp + prevOp * curNum,
                    prevOp * curNum, path, res);
                path.resize(len);
            }
        }
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|----------|----------|------|
| ✗ 用 `int` 存中间值 | ✓ 用 `long long` | 中间计算可能溢出 int |
| ✗ 不处理前导零 | ✓ `if (i > idx && num[idx]=='0') break` | "05" 非法但 "0" 合法 |
| ✗ 乘法直接 `curVal * curNum` | ✓ `curVal - prevOp + prevOp * curNum` | 乘法只作用于前一操作数 |
| ✗ `-` 时 prevOp = curNum | ✓ prevOp = -curNum | prevOp 应含符号以正确处理连续乘法 |

## 面试追问

**Q1: 时间复杂度如何分析？**
-> 每个间隔有 4 种选择（+, -, *, 拼接），n-1 个间隔，最坏 O(4^n)。实际因为前导零剪枝，远小于此。

**Q2: 如果加入除法运算符怎么办？**
-> 类似乘法处理，但需注意：(1) 除数不能为 0；(2) 整数除法 vs 浮点除法；(3) prevOp 的更新逻辑需要调整。

**Q3: 如果要求加括号呢？**
-> 问题变得极其复杂，需要考虑括号的嵌套组合，搜索空间指数级增长。通常需要 DP + 区间划分。

## 相关题型

- [227. Basic Calculator II](https://leetcode.com/problems/basic-calculator-ii/) - 带优先级的表达式求值
- [224. Basic Calculator](https://leetcode.com/problems/basic-calculator/) - 带括号的表达式求值
- [150. Evaluate Reverse Polish Notation](https://leetcode.com/problems/evaluate-reverse-polish-notation/) - 逆波兰表达式求值
- [494. Target Sum](https://leetcode.com/problems/target-sum/) - 目标和（类似思路但只有 +/-）
