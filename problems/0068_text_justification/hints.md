# 68. 文本左右对齐

## 核心思路

这道题本质上是一个**模拟题**：按贪心策略把单词分组到每一行，然后根据规则分配空格。没有复杂的算法，难点在于把空格分配的逻辑想清楚、写对。

## 思维链

1. **读完题第一反应**：这不是算法题，是"按规则排版"的模拟题。核心任务两步：(a) 决定每行放哪些单词，(b) 决定单词间放多少空格。

2. **第一步怎么分行？** 贪心——从左到右尽可能多塞单词。关键判断：当前行已有的字符总长 + 至少1个空格分隔 + 新单词长度 ≤ maxWidth 吗？如果超了就另起一行。

3. **第二步怎么分配空格？** 一行确定后，总空格数 = maxWidth - 所有单词长度之和。如果有 n 个单词，就有 n-1 个"间隙"。用整除和取余来均匀分配：
   - 每个间隙至少 `totalSpaces / (n-1)` 个空格
   - 前 `totalSpaces % (n-1)` 个间隙各多分 1 个空格（左侧多于右侧）

4. **两个特殊情况**：
   - **一行只有一个单词**：左对齐，右边全填空格
   - **最后一行**：左对齐，单词间只放 1 个空格，右边补空格

5. **整体流程**：遍历 words → 贪心分组 → 对每组按规则拼字符串 → 收集结果。

6. **复杂度**：每个单词恰好被处理一次（分组一次，拼接一次），总时间 O(n·maxWidth)，空间 O(maxWidth) 用于构造每行字符串。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 贪心模拟 | 贪心分行 + 空格整除/取余分配 | O(n·maxWidth) | O(maxWidth) | ⭐ 必须写出 |

> 本题只有一种核心思路（贪心模拟），不存在算法层面的"暴力→优化"递进。区别在于代码组织方式：是写成一大坨 if-else，还是拆成清晰的辅助函数。

## 关键提示

1. **分行判断条件**：当前行的字符总数 + 单词间至少1个空格 + 新单词 > maxWidth 时，就该换行了。具体公式：`curLen + 1 + words[i].size() > maxWidth`（curLen 包含已有单词和最少空格）。

2. **空格分配的数学**：`totalSpaces` 个空格分给 `gaps` 个间隙。每个间隙基础空格 = `totalSpaces / gaps`，前 `totalSpaces % gaps` 个间隙额外 +1。

3. **三种拼接情况分清楚**：
   ```
   普通行(多个单词)：均匀分配空格，左侧间隙可能多1个空格
   只有1个单词的行：单词 + 右侧全空格  
   最后一行：         单词间1空格 + 右侧补空格
   ```

4. **一个容易忽略的 edge case**："只有1个单词的行"和"最后一行只有1个单词"处理方式相同（都是左对齐），但"最后一行有多个单词"不同于"只有1个单词的行"。

5. **ASCII 示意——空格分配过程**：
   ```
   words = ["This", "is", "an"], maxWidth = 16
   单词总长 = 4 + 2 + 2 = 8
   总空格数 = 16 - 8 = 8
   间隙数   = 3 - 1 = 2
   每个间隙基础空格 = 8 / 2 = 4
   余数 = 8 % 2 = 0  → 均匀分配
   结果: "This____is____an"  (下划线代表空格)
   
   words = ["example", "of", "text"], maxWidth = 16
   单词总长 = 7 + 2 + 4 = 13
   总空格数 = 16 - 13 = 3
   间隙数   = 2
   基础 = 3 / 2 = 1, 余数 = 3 % 2 = 1 → 第1个间隙多1个空格
   结果: "example__of_text"  (2空格 + 1空格)
   ```

## 解法详解

### 解法1: 贪心模拟（拆分辅助函数版） — O(n·maxWidth) / O(maxWidth) ⭐ 面试首选

**思考过程**：

把大问题拆成两个子问题：
- **分组**：贪心地把尽可能多的单词放进一行
- **拼接**：根据该行是普通行/单词行/最后一行，按规则填充空格

代码拆成 `fullJustify`（主循环分组）+ 构造每一行字符串的逻辑。

```cpp
class Solution {
public:
    vector<string> fullJustify(vector<string>& words, int maxWidth) {
        vector<string> result;
        int n = words.size();
        int i = 0; // 当前待处理的单词索引
        
        while (i < n) {
            // === 第一步：贪心确定当前行放哪些单词 [i, j) ===
            int j = i;
            int lineLen = 0; // 当前行所有单词的字符总长（不含空格）
            
            // 尝试塞入 words[j]：需要 lineLen + 已有间隙数 + words[j].size() <= maxWidth
            // 已有间隙数 = j - i（因为 [i..j) 已有 j-i 个单词，再加一个就有 j-i 个间隙）
            while (j < n && lineLen + (j - i) + (int)words[j].size() <= maxWidth) {
                lineLen += words[j].size();
                j++;
            }
            // 现在 [i, j) 是当前行的单词
            
            int gaps = j - i - 1;           // 间隙数
            int totalSpaces = maxWidth - lineLen; // 需要填充的空格总数
            
            string line;
            
            // === 第二步：根据规则拼接当前行 ===
            if (gaps == 0 || j == n) {
                // 情况A：只有一个单词 或 最后一行 → 左对齐
                for (int k = i; k < j; k++) {
                    if (k > i) line += ' '; // 单词间放1个空格
                    line += words[k];
                }
                // 右侧补空格至 maxWidth
                line += string(maxWidth - line.size(), ' ');
            } else {
                // 情况B：普通行 → 均匀分配空格
                int spacePerGap = totalSpaces / gaps;  // 每个间隙的基础空格数
                int extraSpaces = totalSpaces % gaps;   // 前 extraSpaces 个间隙各多1
                
                for (int k = i; k < j; k++) {
                    line += words[k];
                    if (k < j - 1) { // 不是最后一个单词才加空格
                        int spaces = spacePerGap + (k - i < extraSpaces ? 1 : 0);
                        line += string(spaces, ' ');
                    }
                }
            }
            
            result.push_back(line);
            i = j; // 移动到下一行的起始单词
        }
        
        return result;
    }
};
```

**关键点**：
- 分行判断 `lineLen + (j - i) + words[j].size() <= maxWidth` 中的 `(j - i)` 代表最少的空格数（每个间隙至少1个空格），这是贪心的核心。
- `k - i < extraSpaces` 确保左侧间隙比右侧多，不要写成 `k < extraSpaces`，因为 k 是从 i 开始的。

### 解法2: 贪心模拟（紧凑写法） — O(n·maxWidth) / O(maxWidth)

**和解法1的区别**：逻辑完全一样，但用更紧凑的方式组织代码，适合熟练后快速写出。

```cpp
class Solution {
public:
    vector<string> fullJustify(vector<string>& words, int maxWidth) {
        vector<string> res;
        int i = 0, n = words.size();
        
        while (i < n) {
            // 贪心分行
            int j = i, width = 0;
            while (j < n && width + words[j].size() + (j - i) <= maxWidth)
                width += words[j++].size();
            
            // 构造当前行
            string line = words[i];
            int gaps = j - i - 1;
            
            if (gaps == 0 || j == n) {
                // 左对齐：单词间1空格，右边补齐
                for (int k = i + 1; k < j; k++)
                    line += ' ' + words[k];
                line.resize(maxWidth, ' ');
            } else {
                // 两端对齐：均匀分配空格
                int space = (maxWidth - width) / gaps;
                int extra = (maxWidth - width) % gaps;
                for (int k = i + 1; k < j; k++) {
                    line += string(space + (k - i - 1 < extra ? 1 : 0), ' ');
                    line += words[k];
                }
            }
            
            res.push_back(line);
            i = j;
        }
        return res;
    }
};
```

**关键点**：
- `line.resize(maxWidth, ' ')` 是一个简洁的右侧补空格技巧。
- 紧凑写法中 extra 的判断 `k - i - 1 < extra`：因为 k 从 `i+1` 开始，第一个间隙对应 `k-i-1 = 0`，所以前 extra 个间隙多1。

## 解法对比

| | 解法1（清晰版） | 解法2（紧凑版） |
|--|--|--|
| **可读性** | 高，变量命名清晰 | 中等，需要对索引偏移熟悉 |
| **代码量** | ~35行 | ~25行 |
| **面试推荐** | ✅ 推荐，逻辑清晰不易写错 | 适合熟练选手 |
| **复杂度** | 相同 | 相同 |

面试中推荐解法1，因为模拟题最忌讳写出 bug 后调半天。先把逻辑理清楚比写得短更重要。

## 易错点

1. **分行判断漏掉最少空格数**
   - ✗ `lineLen + words[j].size() <= maxWidth` — 忘了单词间至少要有1个空格
   - ✓ `lineLen + (j - i) + words[j].size() <= maxWidth` — `(j-i)` 就是最少的空格数

2. **extra 空格分配的索引搞错**
   - ✗ `k < extraSpaces` — 当 i ≠ 0 时，k 不是从 0 开始的
   - ✓ `k - i < extraSpaces` — 用相对索引

3. **最后一行误用两端对齐**
   - ✗ 忘记 `j == n` 的判断，导致最后一行也均匀分配空格
   - ✓ `if (gaps == 0 || j == n)` 把最后一行归入左对齐逻辑

4. **只有一个单词时除以零**
   - ✗ `totalSpaces / gaps` 当 `gaps = 0` 时崩溃
   - ✓ 先判断 `gaps == 0` 走左对齐分支，不会执行除法

5. **string 的 size() 返回 unsigned，与 int 混合运算可能出问题**
   - ✓ 转为 int 或确保不会出现负数比较：`(int)words[j].size()`

## 面试追问

**Q1: 这道题的核心难点在哪？**
> 不在算法，在于把规则分清楚并正确实现。三种情况（普通行、单词行、最后一行）各有不同的空格规则，容易混淆。面试中先在白板上列出三种情况再写代码。

**Q2: 如果要求"右侧间隙多于左侧"而不是"左侧多于右侧"，代码改哪里？**
> 只需改 extra 空格的分配方向。把 `k - i < extraSpaces` 改成从右往左数：让后面的间隙获得额外空格。具体：`(gaps - 1 - (k - i)) < extraSpaces` 或者等价地从右侧开始累加。

**Q3: 如果 maxWidth 非常大（比如 10^6），但单词总数很少，有什么性能考虑？**
> 拼接字符串时 `string(spaces, ' ')` 可能创建很长的空格串。可以用 `line.append(spaces, ' ')` 避免临时对象。整体复杂度不变，但常数更优。对于极端情况可以预分配 `line.reserve(maxWidth)`。

**Q4: 如何测试你的实现？**
> 关键测试用例：(1) 所有单词刚好填满一行 (2) 每个单词独占一行 (3) 最后一行只有一个单词 (4) 只有一个单词且长度等于 maxWidth (5) 空格完全均匀分配 vs 不均匀分配。

## 相关题型

- **1592. 重新排列单词间的空格** — 简化版：只有一行，均匀分配空格，可以直接复用本题的空格整除/取余分配逻辑。
- **6. Z 字形变换** — 同为字符串模拟题，核心也是"按规则分组 + 按规则拼接"的思路，练习模拟题的代码组织能力。
- **43. 字符串相乘** — 另一道模拟题，需要把复杂规则拆成小步骤逐个实现，训练"不怕麻烦，逻辑要清楚"的编码习惯。