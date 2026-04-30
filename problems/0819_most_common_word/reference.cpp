/*
 * LeetCode 819: Most Common Word (最常见的单词)
 *
 * 【题目本质】
 * 从段落中提取单词(忽略标点、大小写不敏感)，排除禁用词后，
 * 找出出现频率最高的单词。
 *
 * 【解法总览】
 * 解法1: 手动分词 + 哈希计数   — O(n+m) / O(n+m) — 面试首选
 * 解法2: 预处理替换 + 流分割   — O(n+m) / O(n+m) — 更简洁的变体
 */

// ============================================================
// 解法1: 手动分词 + 哈希计数
// 时间: O(n + m)  空间: O(n + m)
//   n = paragraph 长度, m = banned 总字符数
//
// 【思路】
// 遍历段落字符：
//   - 字母字符 → 转小写，追加到当前单词
//   - 非字母字符 → 当前单词结束，检查是否被禁用，计入频次
// 最后返回频次最高的非禁用词。
//
// 示例: paragraph = "Bob hit a ball, the hit BALL flew far after it was hit."
//       banned = ["hit"]
//
// 转小写后分词: [bob, hit, a, ball, the, hit, ball, flew, far, after, it, was, hit]
// 排除 "hit" 后计数: bob:1, a:1, ball:2, the:1, flew:1, ...
// 最大频次: ball (2次)
// ============================================================
class Solution1 {
public:
    string mostCommonWord(string paragraph, vector<string>& banned) {
        // 1. 禁用词放入集合，O(1) 查找
        unordered_set<string> ban(banned.begin(), banned.end());

        // 2. 单词频次统计
        unordered_map<string, int> count;

        string word;       // 当前正在构建的单词
        string result;     // 结果
        int maxCount = 0;  // 当前最大频次

        // 3. 遍历段落，逐字符处理
        // 注意: i <= paragraph.size()，多走一步处理末尾单词
        for (int i = 0; i <= (int)paragraph.size(); i++) {
            if (i < (int)paragraph.size() && isalpha(paragraph[i])) {
                // 字母字符: 转小写后追加到当前单词
                word += tolower(paragraph[i]);
            } else {
                // 非字母字符(或越界): 当前单词结束
                if (!word.empty()) {
                    // 检查是否被禁用
                    if (!ban.count(word)) {
                        count[word]++;
                        // 实时更新最大频次和结果
                        if (count[word] > maxCount) {
                            maxCount = count[word];
                            result = word;
                        }
                    }
                    word.clear(); // 重置，准备收集下一个单词
                }
            }
        }

        return result;
    }
};

// ============================================================
// 解法2: 预处理替换 + istringstream 分割
// 时间: O(n + m)  空间: O(n + m)
//
// 【思路】
// 先把段落中所有非字母字符替换为空格，同时转小写。
// 然后用 istringstream 按空格自动分割单词。
// 这样代码更简洁，不需要手动管理单词边界。
//
// 优点: 代码短，利用了 STL 的流式分割
// 缺点: 修改了原始字符串(或需要复制)
// ============================================================
class Solution2 {
public:
    string mostCommonWord(string paragraph, vector<string>& banned) {
        // 禁用词集合
        unordered_set<string> ban(banned.begin(), banned.end());

        // 预处理: 非字母 → 空格，字母 → 小写
        for (char& c : paragraph) {
            if (isalpha(c)) {
                c = tolower(c);
            } else {
                c = ' ';
            }
        }

        // istringstream 自动按空格分割，跳过连续空格
        istringstream iss(paragraph);
        string word, result;
        int maxCount = 0;
        unordered_map<string, int> count;

        while (iss >> word) {
            if (!ban.count(word)) {
                count[word]++;
                if (count[word] > maxCount) {
                    maxCount = count[word];
                    result = word;
                }
            }
        }

        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间    | 空间    | 代码量 | 特点             |
// |-------------|---------|---------|--------|-----------------|
// | 手动分词     | O(n+m)  | O(n+m)  | 中等   | 不修改原字符串   |
// | 预处理+流    | O(n+m)  | O(n+m)  | 更短   | 修改了原字符串   |
//
// 面试选择: 两种都可以。手动分词展示了对字符处理的理解，
// 预处理+流展示了对 STL 工具的熟悉。选择自己更熟练的。
//
// ============================================================
// 【易错点】
//
// 1. 大小写处理:
//    x 直接统计 "Bob" 和 "bob" 为不同单词
//    v 全部转小写后再处理
//
// 2. 忘记处理末尾单词:
//    x 段落 "hello" 以字母结尾，循环不到非字母字符
//    v 循环到 i == paragraph.size() (越界)，触发处理最后一个单词
//    或者在循环结束后额外处理最后一个 word
//
// 3. 标点作为分隔符:
//    x 只按空格分割 → "ball," 不会被识别为 "ball"
//    v 用 isalpha() 判断，非字母字符全部视为分隔符
//
// 4. 空单词:
//    x 连续标点 "a,,b" 可能产生空字符串进入计数
//    v 在计数前检查 !word.empty()
//
// 5. banned 查找效率:
//    x 每次 find(banned.begin(), banned.end(), word) → O(k) per query
//    v 预建 unordered_set → O(1) per query
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 时间空间复杂度?
//     时间 O(n+m): 遍历段落 O(n) + 建禁用集合 O(m)
//     空间 O(n+m): 哈希表 + 集合
//
// Q2: 段落非常大(GB级别)怎么处理?
//     流式处理: 逐字符读取，边分词边计数。
//     内存只需要哈希表(不同单词数, 通常远小于段落长度)。
//     可以用 MapReduce 并行化: Map 阶段各节点分词计数,
//     Reduce 阶段合并计数取最大。
//
// Q3: 如果有多个频次相同的最高频非禁用词?
//     题目保证答案唯一。但如果需要全部返回,
//     先遍历一遍找 maxCount, 再遍历哈希表收集所有
//     频次 == maxCount 的词。
//
// Q4: 能否用 Trie 代替哈希表?
//     可以。Trie 的插入和查找都是 O(L)(L=单词长度),
//     与哈希表平均性能相当。但实现更复杂, 面试不推荐。
//
// ============================================================
// 【相关题型】
//
// - 692. 前K个高频单词:
//   扩展版: 返回前K个高频词, 需要用堆或排序。
//   本题是 K=1 的特例。
//
// - 387. 字符串中的第一个唯一字符:
//   简化版: 字符频次统计, 找频次为1的第一个字符。
//
// - 49. 字母异位词分组:
//   字符串预处理 + 哈希分组。不同的是分组而非计数。
// ============================================================
