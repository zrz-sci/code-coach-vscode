# 299. 猜数字游戏 (Bulls and Cows)

## 核心思路

本题本质是一个**计数匹配问题**：先统计位置和数字都对的（Bulls），再统计数字对但位置不对的（Cows）。关键在于 Cows 的计算要排除已经是 Bulls 的数字，且处理重复数字时不能多算。

## 思维链

1. **读完题第一反应**：遍历一遍，位置相同且数字相同的就是 Bull。那 Cow 怎么算？Cow 是"数字出现在 secret 中，但不在正确位置"。
2. **Cow 的难点**：如果有重复数字怎么办？比如 secret="1123", guess="0111"，guess 中有三个 1，但 secret 中只有两个 1（其中一个已经是 Bull），所以 Cow 只有 1 个。
3. **朴素想法**：先一遍扫出所有 Bulls，然后对剩下的非 Bull 位置，统计 secret 和 guess 各自的数字频率，取 min 就是 Cow 数。这需要两次遍历。
4. **能不能一次遍历？** 可以！用一个长度为 10 的计数数组，遇到 Bull 跳过，否则对 secret 的数字 +1，对 guess 的数字 -1。利用正负号的含义，可以在过程中直接检测匹配。
5. **一次遍历的核心观察**：当 `count[s] < 0` 时，说明之前 guess 中出现过这个数字但还没匹配上（guess 多出的），现在 secret 出现了，匹配一个 cow；反之亦然。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 两次遍历 + 频率数组 | 第一遍找 Bulls，第二遍用频率统计 Cows | O(n) | O(1) | ⭐ 必须写出 |
| 一次遍历 + 计数数组 | 利用正负计数，一遍同时统计 Bulls 和 Cows | O(n) | O(1) | 加分项 |

## 关键提示

1. **Bulls 很简单**：`secret[i] == guess[i]` 就是一个 Bull。先把 Bulls 处理掉。
2. **Cows 的关键是不能重复计算**：已经算作 Bull 的位置不能再参与 Cow 的统计。
3. **处理重复数字**：对非 Bull 位置，secret 中数字 d 出现了 `a` 次，guess 中数字 d 出现了 `b` 次，那么数字 d 贡献的 Cow 数是 `min(a, b)`。
4. **一次遍历的技巧**：用一个数组 `count[10]`，secret 的数字让它 +1，guess 的数字让它 -1。在操作**之前**检查符号就能知道有没有"等待配对"的对手。
5. 字符 `'0'` 到 `'9'`，只有 10 种，所以空间是 O(1)。

## 解法详解

### 解法1: 两次遍历 + 频率数组 — O(n) / O(1) ⭐ 面试首选

**思考过程**：最直觉的做法——先扫一遍找 Bulls，再对剩余位置分别统计 secret 和 guess 的数字频率，取 min 之和就是 Cows。

```
// 示例: secret = "1123", guess = "0111"
//
// 第一遍找 Bulls:
//   位置0: '1' vs '0' → 不匹配
//   位置1: '1' vs '1' → Bull! bulls=1
//   位置2: '2' vs '1' → 不匹配
//   位置3: '3' vs '1' → 不匹配
//
// 第二遍统计非 Bull 位置的频率:
//   secret 非Bull位: '1','2','3' → freq_s = {1:1, 2:1, 3:1}
//   guess  非Bull位: '0','1','1' → freq_g = {0:1, 1:2}
//
// Cows = min(1,2) [数字1] + min(1,0) [数字2] + ... = 1
// 结果: "1A1B"
```

```cpp
class Solution {
public:
    string getHint(string secret, string guess) {
        int bulls = 0, cows = 0;
        int freq_s[10] = {0};  // secret 中非 Bull 位置的数字频率
        int freq_g[10] = {0};  // guess 中非 Bull 位置的数字频率
        
        for (int i = 0; i < secret.size(); i++) {
            if (secret[i] == guess[i]) {
                bulls++;
            } else {
                // 只统计非 Bull 位置，避免重复计算
                freq_s[secret[i] - '0']++;
                freq_g[guess[i] - '0']++;
            }
        }
        
        // 对每个数字 0-9，Cow 贡献 = min(secret中出现次数, guess中出现次数)
        for (int d = 0; d < 10; d++) {
            cows += min(freq_s[d], freq_g[d]);
        }
        
        return to_string(bulls) + "A" + to_string(cows) + "B";
    }
};
```

**关键点**：必须在 `else` 分支里统计频率，即排除 Bull 位置。如果先统计全部频率再减去 Bulls，处理重复数字时容易出错。

---

### 解法2: 一次遍历 + 正负计数 — O(n) / O(1)

**从解法1优化**：解法1需要两次遍历（一次找 Bull + 统计频率，一次算 Cow）。能不能一次搞定？

**核心思想**：维护一个 `count[10]` 数组。遇到 secret 中的数字就 +1（"供给"），遇到 guess 中的数字就 -1（"需求"）。关键洞察：
- 当我们处理 secret[i] 时，如果 `count[secret[i]] < 0`，说明之前 guess 已经"需求"了这个数字但还没被满足，现在 secret 提供了一个，配对成功，cows++。
- 当我们处理 guess[i] 时，如果 `count[guess[i]] > 0`，说明之前 secret 已经"供给"了这个数字但还没被消耗，现在 guess 需要它，配对成功，cows++。

```
// 一次遍历演示: secret = "1807", guess = "7810"
//
// count[0..9] 初始全为 0
//
// i=0: s='1', g='7', 不匹配
//      count[1]=0, 不<0, 不加cow; count[1]++ → count[1]=1  (secret供给了1)
//      count[7]=0, 不>0, 不加cow; count[7]-- → count[7]=-1 (guess需求了7)
//
// i=1: s='8', g='8', 匹配! bulls=1
//
// i=2: s='0', g='1', 不匹配
//      count[0]=0, 不<0;           count[0]++ → count[0]=1
//      count[1]=1, >0, cows++!     count[1]-- → count[1]=0  (guess消耗了之前secret供给的1)
//      cows=1
//
// i=3: s='7', g='0', 不匹配
//      count[7]=-1, <0, cows++!    count[7]++ → count[7]=0  (secret满足了之前guess需求的7)
//      count[0]=1, >0, cows++!     count[0]-- → count[0]=0  (guess消耗了之前secret供给的0)
//      cows=3
//
// 结果: bulls=1, cows=3 → "1A3B" ✓
```

```cpp
class Solution {
public:
    string getHint(string secret, string guess) {
        int bulls = 0, cows = 0;
        int count[10] = {0};  // 正数=secret多出的供给, 负数=guess多出的需求
        
        for (int i = 0; i < secret.size(); i++) {
            int s = secret[i] - '0';
            int g = guess[i] - '0';
            
            if (s == g) {
                bulls++;
            } else {
                // 处理 secret[i]: 如果 count[s] < 0，说明之前 guess 需要过这个数字
                if (count[s] < 0) cows++;
                // 处理 guess[i]: 如果 count[g] > 0，说明之前 secret 供给过这个数字
                if (count[g] > 0) cows++;
                
                count[s]++;  // secret 供给
                count[g]--;  // guess 需求
            }
        }
        
        return to_string(bulls) + "A" + to_string(cows) + "B";
    }
};
```

**关键点**：判断 `count[s] < 0` 和 `count[g] > 0` 必须在修改 count 之前。顺序错了会导致当 s == g 时（虽然这个分支已被 if 排除）或逻辑上的先后依赖出问题。

## 解法对比

| | 解法1: 两次遍历 | 解法2: 一次遍历 |
|---|---|---|
| 遍历次数 | 一次遍历 + 一次 0-9 循环 | 一次遍历 |
| 代码直觉性 | ⭐⭐⭐ 非常直觉 | ⭐⭐ 需要理解正负计数 |
| 实际性能差异 | 极小（第二个循环只有10次） | 常数更优 |
| 面试推荐 | ✅ 先写这个，清晰不易错 | ✅ 追问优化时给出 |

**建议面试策略**：先写解法1，代码清晰且不易出错。面试官追问"能否一次遍历"时，再给出解法2。

## 易错点

1. **Cow 重复计算 Bull 的位置**
   - ✗ 先统计所有位置的频率，再减去 Bulls 数量 → 当一个数字既有 Bull 又有 non-Bull 时容易减错
   - ✓ 只在 `else`（非 Bull）分支中统计频率

2. **一次遍历中判断时机错误**
   - ✗ 先 `count[s]++`，再 `if (count[s] < 0) cows++` → 修改后再判断，逻辑完全错误
   - ✓ 先判断 `count[s] < 0`，再 `count[s]++`

3. **忘记处理重复数字**
   - ✗ 用 set 而非频率计数 → secret="1123", guess="0111" 会算出 Cow=2（因为 set 只记录"有没有"，不记录"还剩几个"）
   - ✓ 用频率数组，取 min 保证不多算

4. **字符串拼接格式**
   - ✗ `return bulls + "A" + cows + "B"` → C++ 中 int 和 string literal 不能直接拼接
   - ✓ `return to_string(bulls) + "A" + to_string(cows) + "B"`

## 面试追问

**Q1: 基础理解** — "为什么 Cow 的计算要用 min(freq_s[d], freq_g[d])？"
> 因为一个 Cow 需要 secret 和 guess 各贡献一个匹配。如果 secret 中 '1' 出现了 2 次（非 Bull），guess 中 '1' 出现了 3 次（非 Bull），只能配对 min(2,3)=2 个 Cow。多出的那个 guess 中的 '1' 在 secret 中找不到对应。

**Q2: 优化** — "能否只用一次遍历解决？"
> 可以。用正负计数数组：secret 出现的数字 +1（供给），guess 出现的数字 -1（需求）。在更新之前检查符号，负值说明 guess 之前需要过这个数字，正值说明 secret 之前供给过这个数字，从而即时配对 Cow。

**Q3: 变体** — "如果 secret 和 guess 不是数字字符串，而是任意字符（a-z），怎么改？"
> 把 `count[10]` 改成 `count[26]`（或用 `unordered_map<char, int>`），索引从 `c - '0'` 改成 `c - 'a'`。算法逻辑完全不变，只是计数数组的大小/类型变了。

## 相关题型

- **[2531. 使字符串总不同字符的数目相等](https://leetcode.com/problems/make-number-of-distinct-characters-equal/)** — 同样需要频率计数 + 位置匹配的思想
- **[890. 查找和替换模式](https://leetcode.com/problems/find-and-replace-pattern/)** — 复用"位置对应关系"的映射思路，区别在于本题是固定位置匹配，890 是模式匹配
- **[Mastermind 游戏变体]** — Bulls and Cows 的泛化版本，颜色更多、长度不同时，核心的"先精确匹配，再频率配对"策略完全复用