/*
 * LeetCode 68: Text Justification (文本左右对齐)
 *
 * 【题目本质】
 * 纯模拟题：贪心地把单词分行，然后按规则分配空格。
 * 没有复杂算法，难点在于把三种情况（普通行/单词行/末行）的空格规则想清楚写对。
 *
 * 【解法总览】
 * 解法1: 贪心模拟（清晰版）— O(n·maxWidth) / O(maxWidth) — 面试首选
 * 解法2: 贪心模拟（紧凑版）— O(n·maxWidth) / O(maxWidth) — 熟练后的简洁写法
 *
 * 两种解法逻辑完全相同，区别仅在代码组织方式。
 */

// ============================================================
// 解法1: 贪心模拟（清晰版）
// 时间: O(n·maxWidth)  空间: O(maxWidth)
//
// 【思路】
// 分两步：
// Step1 - 贪心分行：从左到右，尽可能多地把单词塞进当前行。
//   判断条件：已有单词字符数 + 最少空格数(间隙数) + 新单词长度 <= maxWidth
// Step2 - 构造每行字符串：
//   情况A: 只有1个单词 或 最后一行 → 左对齐，单词间1空格，右侧补空格
//   情况B: 普通行 → 总空格数用整除和取余均匀分配到各间隙
//
// 空格分配示意（以示例1第一行为例）:
//   words = ["This", "is", "an"], maxWidth = 16
//   单词总长 = 4+2+2 = 8, 总空格 = 16-8 = 8, 间隙 = 2
//   每间隙基础 = 8/2 = 4, 余数 = 8%2 = 0
//   拼接: "This" + "    " + "is" + "    " + "an"
//   结果: "This    is    an"
//
// 示例1第二行:
//   words = ["example", "of", "text"], maxWidth = 16
//   单词总长 = 7+2+4 = 13, 总空格 = 3, 间隙 = 2
//   基础 = 3/2 = 1, 余数 = 3%2 = 1 → 第1个间隙多1个空格
//   拼接: "example" + "  " + "of" + " " + "text"
//   结果: "example  of text"
//
// 示例1最后一行:
//   words = ["justification."], 最后一行且只有1个单词
//   拼接: "justification." + "  " (右侧补空格至16)
//   结果: "justification.  "
// ============================================================
class Solution1 {
public:
    vector<string> fullJustify(vector<string>& words, int maxWidth) {
        vector<string> result;
        int n = words.size();
        int i = 0;
        
        while (i < n) {
            // --- Step1: 贪心分行，确定当前行单词范围 [i, j) ---
            int j = i;
            int lineLen = 0; // 当前行所有单词的字符总长（不含空格）
            
            // 尝试加入 words[j]：
            // lineLen = 已选单词的字符总数
            // (j - i) = 已选单词之间的最少空格数（每对单词间至少1个空格）
            // words[j].size() = 候选新单词的长度
            // 三者之和 <= maxWidth 则可以放入
            while (j < n && lineLen + (j - i) + (int)words[j].size() <= maxWidth) {
                lineLen += words[j].size();
                j++;
            }
            // 此时 [i, j) 是当前行的单词
            
            int gaps = j - i - 1;             // 单词间的间隙数
            int totalSpaces = maxWidth - lineLen; // 需要填充的总空格数
            
            string line;
            
            // --- Step2: 根据规则构造当前行 ---
            if (gaps == 0 || j == n) {
                // 情况A: 只有1个单词(gaps==0) 或 最后一行(j==n)
                // 规则: 左对齐，单词间放1个空格，右侧补空格
                for (int k = i; k < j; k++) {
                    if (k > i) line += ' ';
                    line += words[k];
                }
                // 右侧补空格至 maxWidth
                line += string(maxWidth - line.size(), ' ');
            } else {
                // 情况B: 普通行，均匀分配空格
                // 整除 = 每个间隙的基础空格数
                // 取余 = 前几个间隙各多分1个空格（保证左侧 >= 右侧）
                int spacePerGap = totalSpaces / gaps;
                int extraSpaces = totalSpaces % gaps;
                
                for (int k = i; k < j; k++) {
                    line += words[k];
                    if (k < j - 1) { // 最后一个单词后面不加空格
                        // 前 extraSpaces 个间隙多1个空格
                        // 为什么用 (k - i) 而不是 k：因为间隙编号从0开始，第一个间隙是 k=i 时
                        int spaces = spacePerGap + ((k - i) < extraSpaces ? 1 : 0);
                        line += string(spaces, ' ');
                    }
                }
            }
            
            result.push_back(line);
            i = j; // 下一行从 j 开始
        }
        
        return result;
    }
};

// ============================================================
// 解法2: 贪心模拟（紧凑版）
// 时间: O(n·maxWidth)  空间: O(maxWidth)
//
// 【思路】
// 逻辑与解法1完全相同，但用更紧凑的写法：
// - 用 line.resize(maxWidth, ' ') 替代手动拼接右侧空格
// - 循环从 i+1 开始（第一个单词先放入 line）
// - 适合对逻辑已经很熟悉、追求代码简洁的情况
// ============================================================
class Solution2 {
public:
    vector<string> fullJustify(vector<string>& words, int maxWidth) {
        vector<string> res;
        int i = 0, n = words.size();
        
        while (i < n) {
            // 贪心确定当前行 [i, j)
            int j = i, width = 0;
            while (j < n && width + (int)words[j].size() + (j - i) <= maxWidth)
                width += words[j++].size();
            
            string line = words[i]; // 先放第一个单词
            int gaps = j - i - 1;
            
            if (gaps == 0 || j == n) {
                // 左对齐：单词间1空格，右侧用 resize 补齐
                for (int k = i + 1; k < j; k++)
                    line += ' ' + words[k];
                line.resize(maxWidth, ' ');
            } else {
                // 两端对齐：均匀分配
                int space = (maxWidth - width) / gaps;
                int extra = (maxWidth - width) % gaps;
                for (int k = i + 1; k < j; k++) {
                    // (k - i - 1) 是当前间隙的编号（从0开始）
                    // 前 extra 个间隙多1个空格
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

// ============================================================
// 【解法对比】
// 两种解法逻辑完全一致，区别仅在代码风格：
// - 解法1: 变量命名清晰(lineLen, spacePerGap, extraSpaces)，
//          适合面试时白板编码，不容易出bug
// - 解法2: 更紧凑(width, space, extra)，用 resize 简化右侧补空格，
//          适合竞赛或已经很熟练的场景
// 
// 面试推荐解法1：模拟题最怕写出bug调半天，清晰比简洁更重要。
//
// 【易错点】
// 1. 分行判断漏掉最少空格数:
//    ✗ lineLen + words[j].size() <= maxWidth  (忘了单词间至少1个空格)
//    ✓ lineLen + (j - i) + words[j].size() <= maxWidth
//
// 2. extra 空格索引偏移:
//    ✗ k < extraSpaces  (当 i≠0 时 k 不从0开始)
//    ✓ (k - i) < extraSpaces  (用相对索引)
//    紧凑版中循环从 i+1 开始，对应写法是 (k - i - 1) < extra
//
// 3. 最后一行遗漏:
//    ✗ 只判断 gaps == 0 而忘了 j == n
//    ✓ if (gaps == 0 || j == n) 两个条件都要检查
//
// 4. 除以零:
//    ✗ 先计算 totalSpaces / gaps 再判断 gaps==0
//    ✓ 先判断 gaps==0 走左对齐分支，避免除法
//
// 5. size() 返回 unsigned 的隐患:
//    当 maxWidth - line.size() 中 line.size() > maxWidth 时会下溢
//    ✓ 由于我们的逻辑保证每行不超过 maxWidth，不会出现此情况，
//      但保险起见可以 cast 为 int
//
// 【面试追问】
// Q1: 这道题有算法优化空间吗？
// A1: 没有。每个单词只被处理一次（分组+拼接），已经是最优。
//     这道题考的是代码实现能力和边界条件处理，不是算法设计。
//
// Q2: 如果要求"右侧间隙比左侧多"，怎么改？
// A2: 只改 extra 的分配方向。当前是 (k-i) < extra 让左侧多，
//     改成 (gaps-1-(k-i)) < extra 即可让右侧多。
//     或者等价地：倒序拼接单词再翻转每个间隙的分配。
//
// Q3: 如果输入是数据流（单词一个一个来），怎么处理？
// A3: 维护一个缓冲区存当前行的单词。每来一个新单词判断能否放入：
//     - 能放入 → 加入缓冲区
//     - 放不下 → 把缓冲区中的单词排版输出，清空缓冲区，新单词进入
//     最后调用一次 flush 输出最后一行（左对齐）。
//
// Q4: 这道题最容易出 bug 的地方在哪？
// A4: 空格数的索引偏移。建议面试时先手动模拟一个例子（如3个单词、
//     总空格7、间隙2 → 基础3余1 → 第一个间隙4空格，第二个3空格），
//     确认索引逻辑正确后再写代码。
// ============================================================
