/*
 * LeetCode 2062: 统计字符串中的元音子字符串
 * 
 * 【题目本质】
 * 找出字符串中所有"仅由元音组成 且 包含全部5种元音"的连续子串的数量。
 * 两个约束：(1) 全是元音 → 辅音是天然隔板  (2) 5种都有 → 种类计数问题
 *
 * 【解法总览】
 * 解法1: 暴力枚举            — O(n²) / O(1) — 最直觉，n≤100足够
 * 解法2: 滑动窗口(atMost技巧) — O(n)  / O(1) — 最优，面试首选
 */

// ============================================================
// 解法1: 暴力枚举(优化版) — 枚举起点，增量维护元音种类
// 时间: O(n²)  空间: O(1)（set最多5个元素，视为常数）
// 
// 【思路】
// 最直觉的做法：固定起点 i，右移终点 j。
// 关键优化：(1) 遇到辅音直接 break（子串连续，辅音后不可能合法）
//          (2) 用 set 增量维护当前窗口内的元音种类
// 
// 例如 "cuaieuouac":
//   i=0: word[0]='c' 是辅音，内层 j 直接不进入
//   i=1: word[1]='u', word[2]='a', ... 一路扩展
//        当 set = {u,a,i,e,o} 时 count++（每次 j 右移都检查）
//   i=9: word[9]='c' 是辅音，跳过
// ============================================================
class Solution1 {
public:
    int countVowelSubstrings(string word) {
        int n = word.size(), count = 0;
        auto isVowel = [](char c) {
            return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
        };
        
        for (int i = 0; i < n; i++) {
            // 起点不是元音，直接跳过（以辅音开头的子串一定不合法）
            if (!isVowel(word[i])) continue;
            
            set<char> seen;
            for (int j = i; j < n; j++) {
                // 遇到辅音 → 以 i 为起点的所有更长子串都包含辅音，不可能合法
                if (!isVowel(word[j])) break;
                
                seen.insert(word[j]);
                // 集合大小为5 → 包含全部5种元音
                if (seen.size() == 5) count++;
            }
        }
        return count;
    }
};

// ============================================================
// 解法2: 滑动窗口(atMost技巧) — 恰好5种 = atMost(5) - atMost(4)
// 时间: O(n)  空间: O(1)（哈希表最多5个key）
//
// 【思路】
// 解法1的瓶颈是 O(n²) 双重枚举。经典的"恰好K种"问题可以转换：
//   恰好K种 = atMost(K) - atMost(K-1)
// 
// atMost(k) 定义：在纯元音子串中，不同元音种类数 ≤ k 的子串总数。
// 
// 为什么等式成立？
//   atMost(5) 包含种类数 0,1,2,3,4,5 的子串
//   atMost(4) 包含种类数 0,1,2,3,4 的子串
//   相减得到恰好5种的子串数
// 
// atMost(k) 用标准滑动窗口：
//   - 右指针扩展，遇到辅音 → 重置（辅音是隔板）
//   - 窗口内种类 > k → 收缩左指针
//   - 每次右指针固定时，合法子串数 = right - left + 1
//
// 滑动窗口计数原理（以 atMost(5) 在 "uaieuoua" 上为例）:
//   right=0: 窗口[u],       种类1≤5, count+=1  (子串:"u")
//   right=1: 窗口[u,a],     种类2≤5, count+=2  ("a","ua")
//   right=2: 窗口[u,a,i],   种类3≤5, count+=3  ("i","ai","uai")
//   right=3: 窗口[u,a,i,e], 种类4≤5, count+=4
//   right=4: 窗口[u,a,i,e,u],种类5≤5,count+=5
//   ...
//   每一步 count += right - left + 1，就是"以right结尾的所有合法子串"
// ============================================================
class Solution2 {
public:
    int countVowelSubstrings(string word) {
        // 恰好5种 = atMost(5) - atMost(4)
        return atMost(word, 5) - atMost(word, 4);
    }
    
private:
    bool isVowel(char c) {
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
    }
    
    // 统计：纯元音子串中，不同元音种类数 ≤ k 的子串个数
    int atMost(const string& word, int k) {
        int n = word.size(), count = 0, left = 0;
        unordered_map<char, int> freq;  // 窗口内每种元音的出现次数
        
        for (int right = 0; right < n; right++) {
            if (!isVowel(word[right])) {
                // 辅音 = 隔板：当前纯元音段结束，必须彻底重置
                // 为什么要 clear 而不是逐个减？因为新段和旧段完全独立
                freq.clear();
                left = right + 1;
                continue;
            }
            
            freq[word[right]]++;
            
            // 种类数超过 k → 收缩左端点直到满足
            while ((int)freq.size() > k) {
                char leftChar = word[left];
                freq[leftChar]--;
                if (freq[leftChar] == 0) {
                    freq.erase(leftChar);  // 种类数减1
                }
                left++;
            }
            
            // 以 right 结尾、起点在 [left, right] 范围内的子串都满足 "种类≤k"
            count += right - left + 1;
        }
        return count;
    }
};

// ============================================================
// 【解法对比】
// 
// 解法1 (O(n²)) vs 解法2 (O(n)):
//   - 解法1 思路直接，代码简单，n≤100 完全够用
//   - 解法2 是通用的"恰好K种"模板，可复用到 LC992 等题
//   - 面试中建议先说解法1展示理解，再给解法2展示优化能力
//
// 核心区别：
//   解法1 枚举起点 → 每个起点都要扫到底 → O(n²)
//   解法2 左右指针各最多走 n 步 → O(n)，且调用两次所以是 O(2n) = O(n)
//
// 【易错点】
// 
// 1. 遇到辅音时只移 left 不清 freq：
//    ✗ if (!isVowel(word[right])) { left = right + 1; continue; }
//    ✓ if (!isVowel(word[right])) { freq.clear(); left = right + 1; continue; }
//    原因：旧段的元音频率不能带到新段，否则种类计数会出错
//
// 2. 解法1中遇辅音用 continue 而不是 break：
//    ✗ if (!isVowel(word[j])) continue;  // 跳过辅音继续扩展
//    ✓ if (!isVowel(word[j])) break;     // 子串连续，辅音后全部非法
//    原因：子串 word[i..j] 必须连续，中间有辅音则整个子串不合法
//
// 3. freq.size() 与 int 比较时的隐患：
//    ✗ while (freq.size() > k)   // size_t 是无符号，当 k<0 时比较异常
//    ✓ while ((int)freq.size() > k)  // 显式转换更安全
//
// 4. atMost(4) 的正确性：
//    有人可能疑惑"纯元音段里种类≤4不就是排除了某种元音吗？"
//    是的，这正是我们想要的——atMost(5)包含所有合法纯元音子串，
//    减去atMost(4)（缺至少一种元音的）= 恰好5种
//
// 【面试追问】
// 
// Q1: 暴力解够用吗？瓶颈在哪？
//     → n≤100 时 O(n²) 甚至 O(n³) 都能过。瓶颈在双重枚举子串。
//
// Q2: 如果 n = 10⁵ 怎么优化到 O(n)？
//     → 用 atMost(5) - atMost(4) 转换，每个 atMost 是标准滑动窗口 O(n)。
//
// Q3: "恰好K种"为什么不能直接用一次滑动窗口？
//     → 因为窗口收缩的条件是"种类>K时收缩"，这只能保证种类≤K。
//       直接要种类==K 的话，左指针不知道该不该收缩（可能收缩后变成K-1种也合法），
//       所以用 atMost(K) - atMost(K-1) 间接得到。
//
// Q4: 如果改成"子串中包含全部5种元音（但允许有辅音）"呢？
//     → 去掉辅音隔板的逻辑。仍然可以用 atMost 技巧，
//       但 atMost 只对元音种类计数，辅音对种类数无影响，只增加子串长度。
// ============================================================
