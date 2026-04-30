# 482. 密钥格式化

## 核心思路
先去掉所有破折号并转大写得到纯净字符串，然后从后向前每 k 个字符分组并插入破折号，最后翻转结果。

## 思维链
1. **预处理**: 遍历字符串，跳过所有 '-'，同时将小写字母转为大写，得到纯净字符串
2. **计算第一组长度**: 纯净长度 % k 得到第一组字符数（可能不足 k 个），若为 0 则第一组满 k 个
3. **反向构建更自然**: 从末尾开始每 k 个字符加一个 '-'，最后 reverse
4. **正向构建也可行**: 先输出第一组（长度 = len % k），然后每 k 个字符加 '-' 前缀
5. **边界处理**: 确保第一组至少有一个字符，且最前面没有多余的 '-'

## 解法概览

| 方法 | 时间 | 空间 | 适用场景 | 推荐 |
|------|------|------|----------|------|
| 反向遍历 + 翻转 | O(n) | O(n) | 逻辑清晰 | ⭐ |
| 正向构建 | O(n) | O(n) | 直观实现 | ⭐ |
| 先拼接再分组 | O(n) | O(n) | 两步法 | |

## 关键提示
1. 从后向前遍历时跳过 '-'，每 k 个字符加一个 '-'，避免第一组长度计算
2. toupper() 可以安全地用在数字和已经是大写的字符上（不会改变它们）
3. 正向构建时，第一组长度 = totalLen % k，若为 0 则取 k
4. 最终结果中不能有首尾多余的 '-'

## 解法详解

### 解法1: 反向遍历 + 翻转 ⭐

**思路**: 从字符串末尾向前遍历，跳过 '-'，每 k 个有效字符后插入一个 '-'。构建完成后翻转得到正确顺序。这种方法自然地让第一组字符数 <= k。

```cpp
class Solution {
public:
    string licenseKeyFormatting(string s, int k) {
        string result;
        int count = 0;
        
        // 从后往前遍历
        for (int i = s.size() - 1; i >= 0; --i) {
            if (s[i] == '-') continue;
            
            // 每 k 个字符加一个分隔符
            if (count > 0 && count % k == 0) {
                result.push_back('-');
            }
            result.push_back(toupper(s[i]));
            ++count;
        }
        
        // 翻转得到正确顺序
        reverse(result.begin(), result.end());
        return result;
    }
};
```

### 解法2: 正向构建（先清洗再分组）

**思路**: 先去掉所有 '-' 并转大写得到纯净字符串，然后计算第一组长度，依次构建。

```cpp
class Solution {
public:
    string licenseKeyFormatting(string s, int k) {
        // 第一步：清洗 — 去掉 '-' 并转大写
        string clean;
        for (char c : s) {
            if (c != '-') {
                clean.push_back(toupper(c));
            }
        }
        
        if (clean.empty()) return "";
        
        // 第二步：计算第一组长度
        int firstGroupLen = clean.size() % k;
        if (firstGroupLen == 0) firstGroupLen = k;
        
        // 第三步：构建结果
        string result;
        int idx = 0;
        
        // 添加第一组
        for (int i = 0; i < firstGroupLen; ++i) {
            result.push_back(clean[idx++]);
        }
        
        // 添加后续每组（每组前加 '-'）
        while (idx < (int)clean.size()) {
            result.push_back('-');
            for (int i = 0; i < k; ++i) {
                result.push_back(clean[idx++]);
            }
        }
        
        return result;
    }
};
```

## 易错点
- ✗ count == k 时插入 '-' 然后重置 count = 0，但忘记先判断 count > 0 → ✓ 防止空字符串时插入多余 '-'
- ✗ 正向构建时第一组长度计算为 len % k，但当 len % k == 0 时取 0 导致空组 → ✓ 此时第一组应取 k
- ✗ 只转换字母为大写，忘记数字字符也要保留 → ✓ toupper() 对数字和已大写字符无影响，直接用即可
- ✗ 反向构建后忘记 reverse → ✓ 最后一步必须翻转

## 面试追问
- **Q1**: 两种方法哪个更好？ → 反向法更优雅，避免了第一组长度的计算；正向法逻辑更直观
- **Q2**: 如果输入全是 '-' 怎么办？ → 纯净字符串为空，返回空字符串。反向法中 count=0，result 自然为空
- **Q3**: 能否原地修改实现 O(1) 额外空间？ → 理论上可以但非常复杂，面试中 O(n) 空间即可

## 相关题型
- LC 6 Z 字形变换（字符串重新分组）
- LC 68 文本左右对齐（字符串格式化）
- LC 1108 IP 地址无效化（字符串格式转换）
- LC 1417 重新格式化字符串（交替排列字母数字）
