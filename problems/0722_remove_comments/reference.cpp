// ============================================================
// 722. Remove Comments
// ============================================================
// 标签: Array, String
// 难度: Medium
//
// 核心思路:
//   状态机逐字符扫描, 两个状态: 正常代码 / 块注释内
//   行注释 // → break 跳过行末
//   块注释 /* ... */ → 可跨行, 前后内容拼接到同一行
// ============================================================


// ============================================================
// 解法1: 状态机 + 逐字符扫描 (推荐, 面试标准解)
//
// 状态: inBlock (bool)
//   false → 正常代码: 检查 //, /*, 或普通字符
//   true  → 块注释内: 只找 */ 退出
//
// 缓冲区: newLine (string)
//   行末且 !inBlock → 输出 newLine (非空时), 清空
//   行末且  inBlock → 不清空 (跨行拼接)
//
// 时间: O(N), N = 总字符数
// 空间: O(N)
// ============================================================
class Solution {
public:
    vector<string> removeComments(vector<string>& source) {
        vector<string> result;
        bool inBlock = false;    // 是否在块注释中
        string newLine;          // 当前行缓冲区

        for (const string& line : source) {
            int n = line.size();

            for (int i = 0; i < n; i++) {
                if (inBlock) {
                    // ---- 块注释内: 只关心 */ ----
                    if (i + 1 < n && line[i] == '*' && line[i + 1] == '/') {
                        inBlock = false;
                        i++;  // 跳过 '/', 避免下一轮误读
                    }
                    // 其他字符全部忽略 (包括 //)
                } else {
                    // ---- 正常代码 ----
                    if (i + 1 < n && line[i] == '/' && line[i + 1] == '/') {
                        // 行注释: 忽略本行剩余所有字符
                        break;
                    } else if (i + 1 < n && line[i] == '/' && line[i + 1] == '*') {
                        // 块注释开始
                        inBlock = true;
                        i++;  // 跳过 '*', 避免 /*/ 被误判为 */
                    } else {
                        // 普通字符, 加入当前行
                        newLine += line[i];
                    }
                }
            }

            // 行处理完毕:
            // 只有不在块注释中时才考虑输出
            // (块注释中换行不输出, 也不清空 newLine)
            if (!inBlock && !newLine.empty()) {
                result.push_back(newLine);
                newLine.clear();
            }
        }

        return result;
    }
};


// ============================================================
// 解法2: 拼接所有行后用状态机处理 (变体写法)
//
// 先把所有行用 '\n' 拼接成一个大字符串
// 然后状态机扫描, 遇到 '\n' 时处理换行逻辑
// 最后按 '\n' 分割非空行
//
// 优点: 统一处理, 不用每行单独循环
// 缺点: 需要处理行分隔符, 代码反而更长
//
// 时间: O(N)
// 空间: O(N)
// ============================================================
class Solution_Concat {
public:
    vector<string> removeComments(vector<string>& source) {
        // Step 1: 拼接所有行
        string code;
        for (int j = 0; j < (int)source.size(); j++) {
            if (j > 0) code += '\n';
            code += source[j];
        }

        // Step 2: 状态机扫描
        string cleaned;
        bool inBlock = false;
        int n = code.size();

        for (int i = 0; i < n; i++) {
            if (inBlock) {
                if (i + 1 < n && code[i] == '*' && code[i + 1] == '/') {
                    inBlock = false;
                    i++;
                }
                // 块注释内: 连 '\n' 也忽略 (不加入 cleaned)
            } else {
                if (i + 1 < n && code[i] == '/' && code[i + 1] == '/') {
                    // 行注释: 跳到行末
                    while (i < n && code[i] != '\n') {
                        i++;
                    }
                    i--;  // 外层 for 会 i++, 这里退一步让 '\n' 在下一轮处理
                } else if (i + 1 < n && code[i] == '/' && code[i + 1] == '*') {
                    inBlock = true;
                    i++;
                } else {
                    cleaned += code[i];
                }
            }
        }

        // Step 3: 按 '\n' 分割, 过滤空行
        vector<string> result;
        string line;
        for (char c : cleaned) {
            if (c == '\n') {
                if (!line.empty()) {
                    result.push_back(line);
                    line.clear();
                } else {
                    result.push_back("");  // 保留原始空行
                }
            } else {
                line += c;
            }
        }
        // 注意: 按题意, 删除注释后为空的行不输出
        // 但原始就为空的行需要保留? 题目说不输出空行
        // 实际只需: 分割后过滤空行
        vector<string> filtered;
        for (const string& s : result) {
            if (!s.empty()) filtered.push_back(s);
        }
        if (!line.empty()) filtered.push_back(line);

        return filtered;
    }
};


// ============================================================
// 总结
// ============================================================
/*
 * 【解法对比】
 *
 * | 解法              | 时间  | 空间  | 推荐度         |
 * |-------------------|-------|-------|----------------|
 * | 状态机逐行 (推荐) | O(N)  | O(N)  | 面试首选       |
 * | 拼接后扫描        | O(N)  | O(N)  | 逻辑统一但冗长 |
 *
 * 【关键洞察】
 * 1. 两个状态足矣: inBlock=true/false
 * 2. 块注释跨行时 newLine 不清空, 实现前后代码拼接
 * 3. /* 后 i++ 防止 /*/ 被误判为 */
 * 4. 行注释 break 即可, 不需要额外状态
 *
 * 【易错点】
 * 1. 块注释跨行 → newLine 不清空 (最常见错误)
 * 2. i+1 越界检查 → 访问 line[i+1] 前必须检查 i+1 < n
 * 3. /\*\/ 边界 → /* 后 i++ 跳过 *, 否则 /*/ 中的 *
 *    和 / 会被误判为 */
 * 4. 空行不输出 → newLine 为空时不加入 result
 * 5. 注释符在块注释内 → // 在块注释内不生效
 *
 * 【面试追问】
 * Q: 如果代码中有字符串字面量包含 // 或 /* 怎么办?
 * A: 需要增加 inString 状态, 字符串内所有注释符号都是普通字符
 *    还要处理转义 \" 不终止字符串
 *    本题简化了此问题, 但编译器前端需要完整处理
 *
 * Q: 本质是什么?
 * A: 2-状态有限自动机 (FSA), 输入 = ASCII + 行末标记
 *    转移由双字符序列 /\*, \*\/, // 触发
 */
