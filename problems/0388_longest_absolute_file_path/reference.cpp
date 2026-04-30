/*
 * 388. Longest Absolute File Path
 * 难度: Medium
 * 标签: Stack, Depth-First Search, String
 *
 * 【题目本质】
 * 从文件系统的缩进表示中找到指向文件的最长绝对路径长度。
 * 核心是利用 tab 数量确定层级，维护每层路径长度前缀。
 *
 * 【解法总览】
 * 解法1: 哈希表记录层级长度 - 简洁高效
 * 解法2: 栈模拟层级回退 - 自动处理深度变化
 */

// ===================== Solution 1: 哈希表记录层级长度 =====================
// 思路：用 map[depth] 记录每层累计路径长度，遇文件时计算总长
// 时间：O(n)
// 空间：O(d)，d 为最大深度
class Solution1 {
public:
    int lengthLongestPath(string input) {
        unordered_map<int, int> pathLen;
        pathLen[-1] = 0;  // 哨兵：depth=-1 长度为 0
        int maxLen = 0;

        istringstream ss(input);
        string line;
        while (getline(ss, line, '\n')) {
            // 计算深度（tab 数量）
            int depth = 0;
            while (depth < (int)line.size() && line[depth] == '\t') depth++;
            string name = line.substr(depth);

            // 当前层路径长度 = 父层长度 + 分隔符/ + 名称长度
            pathLen[depth] = pathLen[depth - 1] + 1 + (int)name.size();

            // 是文件则更新答案（减1因为根不需要前导/）
            if (name.find('.') != string::npos) {
                maxLen = max(maxLen, pathLen[depth] - 1);
            }
        }
        return maxLen;
    }
};

// ===================== Solution 2: 栈模拟 =====================
// 思路：栈中存每层累计长度，遇到浅层时弹栈回退
// 时间：O(n)
// 空间：O(d)
class Solution2 {
public:
    int lengthLongestPath(string input) {
        stack<int> stk;
        stk.push(0);  // 哨兵
        int maxLen = 0;

        istringstream ss(input);
        string line;
        while (getline(ss, line, '\n')) {
            int depth = 0;
            while (depth < (int)line.size() && line[depth] == '\t') depth++;

            // 弹栈回退到父层（栈大小应 = depth + 1）
            while ((int)stk.size() > depth + 1) stk.pop();

            // 当前累计长度 = 父层长度 + 名称长度 + 1(分隔符)
            int nameLen = (int)line.size() - depth;
            int curLen = stk.top() + nameLen + 1;
            stk.push(curLen);

            if (line.find('.') != string::npos) {
                maxLen = max(maxLen, curLen - 1);
            }
        }
        return maxLen;
    }
};

/*
 * 【解法对比】
 * | 解法 | 数据结构 | 优点 | 缺点 |
 * |------|---------|------|------|
 * | 哈希表 | unordered_map | 直接按 depth 索引 | 旧层级数据残留（不影响正确性） |
 * | 栈 | stack | 自然处理回退 | 代码略多 |
 *
 * 【易错点】
 * 1. 只有文件（含.）才更新答案，目录不算
 * 2. 根层级不需要前导 /，所以最终 -1
 * 3. \n 和 \t 是单个字符，不是两个
 * 4. depth=0 时是根级条目，不要漏算
 *
 * 【面试追问】
 * Q1: 没有扩展名如何判断文件 → 本题规定有 . 就是文件
 * Q2: 多个根目录 → pathLen[0] 会被覆盖，自动处理
 * Q3: 实际文件系统遍历 → DFS + inode 判断文件/目录
 */
