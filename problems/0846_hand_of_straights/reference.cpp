// LeetCode 846: Hand of Straights
// 解法一：有序 Map + 贪心（推荐）
// 时间 O(n log n)，空间 O(n)
// 从最小值开始贪心构建连续组

class Solution {
public:
    bool isNStraightHand(vector<int>& hand, int groupSize) {
        int n = hand.size();
        // 必要条件：总数能被 groupSize 整除
        if (n % groupSize != 0) return false;

        // 有序 map 统计频次
        map<int, int> freq;
        for (int card : hand) {
            freq[card]++;
        }

        // 贪心：每次从最小值开始构建一组
        while (!freq.empty()) {
            int start = freq.begin()->first;  // 当前最小值
            int count = freq.begin()->second;  // 需要构建 count 组以 start 开头的

            // 尝试构建 [start, start+1, ..., start+groupSize-1]
            for (int i = 0; i < groupSize; ++i) {
                int card = start + i;
                if (freq.find(card) == freq.end() || freq[card] < count) {
                    return false;
                }
                freq[card] -= count;
                if (freq[card] == 0) {
                    freq.erase(card);
                }
            }
        }
        return true;
    }
};

// ============================================================
// 解法二：排序 + 哈希表贪心
// 时间 O(n log n)，空间 O(n)
// 排序后遍历，每次尝试从当前数构建一组

class Solution2 {
public:
    bool isNStraightHand(vector<int>& hand, int groupSize) {
        int n = hand.size();
        if (n % groupSize != 0) return false;

        sort(hand.begin(), hand.end());
        unordered_map<int, int> freq;
        for (int card : hand) {
            freq[card]++;
        }

        for (int card : hand) {
            // 如果当前牌已经被用完，跳过
            if (freq[card] == 0) continue;

            // 以 card 为起始，构建一组
            for (int i = 0; i < groupSize; ++i) {
                int need = card + i;
                if (freq[need] <= 0) return false;
                freq[need]--;
            }
        }
        return true;
    }
};

// ============================================================
// 解法三：有序 Map 逐一构建（另一种贪心写法）
// 时间 O(n log n)，空间 O(n)
// 每次只构建一组（而非批量），更直观但稍慢

class Solution3 {
public:
    bool isNStraightHand(vector<int>& hand, int groupSize) {
        int n = hand.size();
        if (n % groupSize != 0) return false;

        map<int, int> freq;
        for (int card : hand) {
            freq[card]++;
        }

        int groups = n / groupSize;
        for (int g = 0; g < groups; ++g) {
            // 找当前最小的可用牌
            auto it = freq.begin();
            int start = it->first;

            // 构建 [start, start+groupSize-1]
            for (int i = 0; i < groupSize; ++i) {
                int card = start + i;
                auto found = freq.find(card);
                if (found == freq.end()) return false;
                found->second--;
                if (found->second == 0) {
                    freq.erase(found);
                }
            }
        }
        return true;
    }
};

// ============================================================
// 解法四：使用 multiset（最简洁写法）
// 时间 O(n * groupSize * log n)，空间 O(n)
// 每次取最小值，逐个移除连续元素

class Solution4 {
public:
    bool isNStraightHand(vector<int>& hand, int groupSize) {
        if (hand.size() % groupSize != 0) return false;

        multiset<int> cards(hand.begin(), hand.end());

        while (!cards.empty()) {
            int start = *cards.begin();
            for (int i = 0; i < groupSize; ++i) {
                auto it = cards.find(start + i);
                if (it == cards.end()) return false;
                cards.erase(it);  // 只移除一个
            }
        }
        return true;
    }
};
