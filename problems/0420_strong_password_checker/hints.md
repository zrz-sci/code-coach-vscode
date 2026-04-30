# 420. Strong Password Checker - 强密码检验器

## 核心思路
将密码修改分为三种情况：长度<6（需插入）、6<=长度<=20（只需替换）、长度>20（需删除+替换），关键在于利用删除操作优化连续重复字符的替换次数。

## 思维链
1. 统计缺失的字符类型数（小写/大写/数字）→ `missing`
2. 扫描连续重复字符段，记录每段长度
3. **长度<6**: 答案 = max(missing, 6-n)
4. **6<=长度<=20**: 替换次数 = sum(每段len/3)，答案 = max(missing, replace)
5. **长度>20**: 需要删除 `del = n-20` 个字符，优先用删除替代部分替换（按 len%3 优先级贪心），答案 = del + max(missing, 剩余replace)

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法1 | 贪心分类讨论 ⭐ | O(n) | O(n) | 首选 |

## 关键提示
1. 连续重复段长度为 `k`，需要 `k/3` 次替换来打断
2. 删除可以减少替换次数：删1个可将 `len%3==0` 的段减少1次替换，删2个可将 `len%3==1` 的段减少1次替换，删3个可将任意段减少1次替换
3. 插入和替换都可以同时解决缺失字符类型问题

## 解法详解

### 解法1: 贪心分类讨论
```cpp
class Solution {
public:
    int strongPasswordChecker(string password) {
        int n = password.size();
        int missing = 0;
        bool hasLower = false, hasUpper = false, hasDigit = false;
        for (char c : password) {
            if (islower(c)) hasLower = true;
            else if (isupper(c)) hasUpper = true;
            else if (isdigit(c)) hasDigit = true;
        }
        missing = (!hasLower) + (!hasUpper) + (!hasDigit);
        
        // 统计连续重复段，每段需要 len/3 次替换
        int replace = 0;
        int one = 0, two = 0; // len%3==0 和 len%3==1 的段数
        int i = 2;
        while (i < n) {
            if (password[i] == password[i-1] && password[i] == password[i-2]) {
                int len = 2;
                while (i < n && password[i] == password[i-1]) { len++; i++; }
                replace += len / 3;
                if (len % 3 == 0) one++;
                else if (len % 3 == 1) two++;
            } else {
                i++;
            }
        }
        
        if (n < 6) return max(missing, 6 - n);
        if (n <= 20) return max(missing, replace);
        
        // n > 20: 需要删除 del 个字符
        int del = n - 20;
        // 优先用删除减少替换：删1个减少 len%3==0 的段
        replace -= min(del, one);
        // 删2个减少 len%3==1 的段
        replace -= min(max(del - one, 0), two * 2) / 2;
        // 删3个减少任意段
        replace -= max(del - one - two * 2, 0) / 3;
        replace = max(replace, 0);
        return del + max(missing, replace);
    }
};
```

## 易错点
| 错误写法 | 正确写法 |
|---------|---------|
| ✗ 只考虑替换不考虑删除优化 | ✓ 按 len%3 优先级用删除减少替换次数 |
| ✗ 忘记 missing 与操作次数取 max | ✓ 插入/替换同时可修复字符类型缺失 |
| ✗ replace 减为负数 | ✓ replace = max(replace, 0) |

## 面试追问
**Q1: 为什么删除操作要按 len%3 的优先级分配？**
> 对于 len%3==0 的段（如长度6），只需删1个就减少1次替换（6→5，替换从2变1）；len%3==1 的段需删2个；其他需删3个。贪心优先处理性价比高的。

**Q2: 为什么 n<6 时答案是 max(missing, 6-n)？**
> 插入字符既增加长度又可以补充缺失类型，两个需求可以共用操作次数，取较大值即可。

**Q3: 这题能用 DP 解吗？**
> 理论上可以但不实用。三种操作的约束关系使得贪心分类讨论更清晰，且 O(n) 复杂度最优。

## 相关题型
- [68. Text Justification](https://leetcode.com/problems/text-justification/) - 复杂模拟分类讨论
- [1180. Count Substrings with Only One Distinct Letter](https://leetcode.com/problems/count-substrings-with-only-one-distinct-letter/) - 连续重复段处理
