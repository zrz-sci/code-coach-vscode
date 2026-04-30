# 388. Longest Absolute File Path - 文件的最长绝对路径

## 核心思路
从文件系统的字符串表示中找到最长的文件绝对路径。核心是用 `\t` 的数量确定层级深度，用**哈希表或栈**记录每层的路径长度前缀和，遇到文件时计算完整路径长度。

## 思维链
1. 按 `\n` 分割得到每个条目（文件/目录名）
2. 数 `\t` 的个数 → 当前层级 depth
3. 去掉 `\t` 后得到文件名/目录名
4. 当前层的路径长度 = 上一层路径长度 + 当前名称长度 + 1（分隔符 `/`）
5. 如果当前是文件（包含 `.`），更新最大路径长度
6. 用 map 记录 `depth → 累计路径长度`

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 哈希表记录层级长度 | O(n) | O(d) | 最简洁直观 |
| 栈模拟 | O(n) | O(d) | 自动处理回退到上层 |

## 关键提示
1. `\t` 的数量就是当前条目的深度（0个tab = 根目录）
2. 文件的判定：名称中包含 `.`
3. 路径长度要加上 `/` 分隔符：但根层级不需要前导 `/`
4. 用 `map[depth] = map[depth-1] + name.length + 1` 递推
5. 根层级（depth=0）的路径长度就是名称长度本身
6. **只有文件需要计算路径长度**，目录不算答案

## 解法详解

### 解法一：哈希表记录层级长度
```cpp
class Solution {
public:
    int lengthLongestPath(string input) {
        unordered_map<int, int> pathLen;  // depth → 累计路径长度
        pathLen[-1] = 0;  // 哨兵：根的父层长度为0（不含分隔符）
        int maxLen = 0;
        
        istringstream ss(input);
        string line;
        while (getline(ss, line, '\n')) {
            int depth = 0;
            while (depth < line.size() && line[depth] == '\t') depth++;
            string name = line.substr(depth);
            
            // 当前层路径长度 = 上层长度 + 1(分隔符/) + 当前名称长度
            pathLen[depth] = pathLen[depth - 1] + 1 + name.size();
            
            // 如果是文件，更新答案（减1因为根层级不需要前导/）
            if (name.find('.') != string::npos) {
                maxLen = max(maxLen, pathLen[depth] - 1);
            }
        }
        return maxLen;
    }
};
```

### 解法二：栈模拟
```cpp
class Solution {
public:
    int lengthLongestPath(string input) {
        stack<int> stk;  // 每层的累计路径长度
        stk.push(0);     // 哨兵
        int maxLen = 0;
        
        istringstream ss(input);
        string line;
        while (getline(ss, line, '\n')) {
            int depth = 0;
            while (depth < line.size() && line[depth] == '\t') depth++;
            
            // 回退到当前层的父层
            while ((int)stk.size() > depth + 1) stk.pop();
            
            int curLen = stk.top() + (int)line.size() - depth + 1;  // +1 for '/'
            stk.push(curLen);
            
            if (line.find('.') != string::npos) {
                maxLen = max(maxLen, curLen - 1);  // -1 去掉根的前导/
            }
        }
        return maxLen;
    }
};
```

## 易错点
| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 用空格数计算深度 | ✓ 用 `\t` 数计算深度 | 题目明确用 tab 表示层级 |
| ✗ 忘记减去 `/` 的多余计算 | ✓ 根层级不需要前导 `/`，最终 -1 | 路径是 "dir/sub/file" 不是 "/dir/sub/file" |
| ✗ 目录也更新 maxLen | ✓ 只有包含 `.` 的文件才更新 | 题目要求"文件"的路径，目录不算 |
| ✗ `\n` 和 `\t` 当成两个字符处理 | ✓ 它们是单个转义字符 | 在 C++ 字符串中 `\n` 是一个字符 |

## 面试追问

**Q1: 如果文件名中没有扩展名（没有 `.`），如何判断是文件还是目录？**
> 本题约定文件名格式是 `name.extension`（包含 `.`），所以用 `.` 判断。实际系统中需要额外标记或 inode 信息。

**Q2: 如果输入可能包含多个根目录呢？**
> 本题的输入可以有多个根级条目（depth=0 的条目可以有多个）。算法已经正确处理：每次遇到 depth=0 时，map[0] 会被覆盖。

**Q3: 能否用 O(1) 空间解决？**
> 不能在严格意义上做到 O(1)，因为需要记录每一层的路径长度。但可以用数组替代哈希表（深度不超过路径分量数），空间为 O(d)，d 是最大深度，通常远小于 n。

## 相关题型
- [71. Simplify Path](https://leetcode.com/problems/simplify-path/) - 路径简化
- [609. Find Duplicate File in System](https://leetcode.com/problems/find-duplicate-file-in-system/) - 文件系统操作
- [1166. Design File System](https://leetcode.com/problems/design-file-system/) - 文件系统设计
