/*
 * LeetCode 244: 最短单词距离 II (Shortest Word Distance II)
 *
 * 【题目本质】
 * 设计一个支持多次查询的数据结构：给定单词数组，每次查询两个单词
 * 在数组中的最短距离。核心是"预处理 vs 查询"的权衡。
 *
 * 【解法总览】
 * 解法1: 暴力查询     — 构造 O(1) / 查询 O(n) — 每次查询遍历整个数组
 * 解法2: 哈希表+双指针 — 构造 O(n) / 查询 O(L1+L2) — 预处理下标，双指针找最小差 ⭐
 * 解法3: 哈希表+双指针+缓存 — 重复查询 O(1) — 缓存已查结果
 *
 * 【和 243 的关系】
 * 243 只需查一次，线性扫描 O(n) 即可。
 * 244 要查多次，需要在构造时预处理位置信息，避免重复扫描。
 * 代码区别：243 没有构造函数，244 在构造函数中建哈希表。
 */

// ============================================================
// 解法1: 暴力查询 — 每次 shortest 线性扫描整个数组
// 构造: O(1)    查询: O(n)    空间: O(n)（存原数组）
//
// 【思路】
// 最简单的做法：不做任何预处理，每次查询时像 243 一样
// 遍历数组，记录最近看到的 word1 和 word2 的位置，
// 实时更新最小距离。
//
// 示例: wordsDict = ["practice","makes","perfect","coding","makes"]
// 查询 shortest("coding", "practice"):
//   i=0: "practice" → idx2=0
//   i=1: "makes"    → (无关)
//   i=2: "perfect"  → (无关)
//   i=3: "coding"   → idx1=3, dist=|3-0|=3, minDist=3
//   i=4: "makes"    → (无关)
//   结果: 3
// ============================================================
class WordDistance1 {
    vector<string> words;
public:
    WordDistance1(vector<string>& wordsDict) {
        words = wordsDict;  // 仅保存，不预处理
    }
    
    int shortest(string word1, string word2) {
        int minDist = INT_MAX;
        int idx1 = -1, idx2 = -1;
        for (int i = 0; i < (int)words.size(); i++) {
            if (words[i] == word1) idx1 = i;
            if (words[i] == word2) idx2 = i;
            // 两个都找到过时才更新距离
            if (idx1 != -1 && idx2 != -1) {
                minDist = min(minDist, abs(idx1 - idx2));
            }
        }
        return minDist;
    }
};

// ============================================================
// 解法2: 哈希表 + 双指针 — 预处理下标，查询时双指针找最小差 ⭐
// 构造: O(n)    查询: O(L1 + L2)    空间: O(n)
//
// 【思路】
// 解法1的瓶颈：每次查询都要遍历整个数组找位置。
// 但单词位置是固定的，构造时一次性记录即可。
//
// 预处理：用哈希表 wordIndices[word] = [所有出现下标]
// 下标列表天然有序（从左到右遍历存入）。
//
// 查询：取出两个有序列表，用双指针找最小差值。
// 双指针正确性：谁小移谁，因为：
//   如果 list1[i] < list2[j]，那么 list1[i] 和 list2[j+k] 差更大
//   → 移动 i 才有机会缩小差值
//
// 示例:
//   "makes" → [1, 4]    "coding" → [3]
//
//   双指针过程:
//   i=0, j=0: |1-3|=2, 1<3 → i++
//   i=1, j=0: |4-3|=1, 3<4 → j++
//   j 越界，结束。最小距离 = 1
// ============================================================
class WordDistance {
private:
    unordered_map<string, vector<int>> wordIndices;

public:
    WordDistance(vector<string>& wordsDict) {
        for (int i = 0; i < (int)wordsDict.size(); i++) {
            // 从左到右遍历，push_back 保证下标递增
            wordIndices[wordsDict[i]].push_back(i);
        }
    }
    
    int shortest(string word1, string word2) {
        // 用 const 引用避免拷贝
        const vector<int>& list1 = wordIndices[word1];
        const vector<int>& list2 = wordIndices[word2];
        
        int i = 0, j = 0;
        int minDist = INT_MAX;
        
        while (i < (int)list1.size() && j < (int)list2.size()) {
            int idx1 = list1[i], idx2 = list2[j];
            minDist = min(minDist, abs(idx1 - idx2));
            
            // 谁小移谁：小的往后移才可能缩小差值
            if (idx1 < idx2) {
                i++;
            } else {
                j++;
            }
        }
        
        return minDist;
    }
};

// ============================================================
// 解法3: 哈希表 + 双指针 + 查询缓存
// 构造: O(n)    查询: 首次 O(L1+L2), 重复 O(1)    空间: O(n + Q)
//
// 【思路】
// 在解法2基础上，如果同一对单词被反复查询，缓存结果。
// 注意 (word1,word2) 和 (word2,word1) 是同一个查询，
// 需要统一 key 的顺序（如字典序小的在前）。
// ============================================================
class WordDistance3 {
private:
    unordered_map<string, vector<int>> wordIndices;
    unordered_map<string, int> cache;  // "word1#word2" → 最短距离

public:
    WordDistance3(vector<string>& wordsDict) {
        for (int i = 0; i < (int)wordsDict.size(); i++) {
            wordIndices[wordsDict[i]].push_back(i);
        }
    }
    
    int shortest(string word1, string word2) {
        // 统一 key 顺序，保证 (a,b) 和 (b,a) 共享缓存
        string key = (word1 < word2) ? (word1 + "#" + word2) 
                                     : (word2 + "#" + word1);
        
        auto it = cache.find(key);
        if (it != cache.end()) return it->second;
        
        const vector<int>& list1 = wordIndices[word1];
        const vector<int>& list2 = wordIndices[word2];
        
        int i = 0, j = 0;
        int minDist = INT_MAX;
        
        while (i < (int)list1.size() && j < (int)list2.size()) {
            int idx1 = list1[i], idx2 = list2[j];
            minDist = min(minDist, abs(idx1 - idx2));
            if (idx1 < idx2) i++;
            else j++;
        }
        
        cache[key] = minDist;
        return minDist;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 vs 解法2:
//   解法1 不做预处理，每次查询 O(n) 扫描全数组。
//   解法2 构造时 O(n) 建哈希表，查询只看相关下标 O(L1+L2)。
//   当 L1+L2 << n 时（单词出现次数远小于数组长度），解法2 快很多。
//
// 解法2 vs 解法3:
//   解法3 额外缓存查询结果，适合同一对单词反复被查的场景。
//   面试中先写解法2，面试官追问再优化到解法3。
//
// 【易错点】
// 1. 双指针移动方向写反:
//    ✗ if (idx1 < idx2) j++;  → 移动大的，差值更大，永远找不到最优
//    ✓ if (idx1 < idx2) i++;  → 移动小的，差值可能缩小
//
// 2. 用拷贝而非引用获取下标列表:
//    ✗ vector<int> list1 = wordIndices[word1];  → 每次查询拷贝O(L1)
//    ✓ const vector<int>& list1 = wordIndices[word1];  → 零开销引用
//
// 3. 缓存时忘记统一 key 顺序:
//    ✗ cache[word1+"#"+word2]  → ("a","b") 和 ("b","a") 各算一次
//    ✓ 先按字典序排列，保证同一对单词的 key 唯一
//
// 4. 误以为下标列表需要排序:
//    下标是从左到右遍历时 push_back 的，天然递增，不需要 sort()
//
// 【面试追问】
// Q1: 和 243 有什么区别？为什么不能直接用 243 的方案？
//   → 243 只查一次 O(n) 够了；244 查多次，每次 O(n) 太慢，需要预处理。
//
// Q2: 双指针为什么正确？为什么 O(L1+L2) 不会漏解？
//   → 两个列表有序。idx1 < idx2 时，idx1 与 list2 后面的差更大，
//     所以跳过 idx1 不会错过最优。每个指针最多走到头，总共 O(L1+L2)。
//
// Q3: 如果同一对单词被查 1000 次怎么优化？
//   → 加查询缓存（解法3），首次 O(L1+L2)，后续 O(1)。
//
// Q4: 如果单词可以动态添加到数组末尾呢？
//   → 新单词 append 到原数组末尾，同时往哈希表对应列表 push_back 新下标。
//     列表仍然有序。如果有缓存，需要清除涉及该单词的缓存条目。
// ============================================================
