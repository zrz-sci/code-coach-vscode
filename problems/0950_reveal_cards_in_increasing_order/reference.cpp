// LeetCode 950: Reveal Cards In Increasing Order
// 解法一：队列模拟索引分配（推荐）
// 时间复杂度: O(n log n) - 排序主导
// 空间复杂度: O(n)

class Solution {
public:
    vector<int> deckRevealedIncreasing(vector<int>& deck) {
        int n = deck.size();

        // 步骤1：排序得到目标揭示顺序（递增）
        sort(deck.begin(), deck.end());

        // 步骤2：队列初始化为位置索引 [0, 1, 2, ..., n-1]
        queue<int> indexQueue;
        for (int i = 0; i < n; i++) {
            indexQueue.push(i);
        }

        // 步骤3：模拟揭示过程，将排序后的牌分配到对应位置
        vector<int> result(n);
        for (int i = 0; i < n; i++) {
            // 取出队首索引 —— 这是下一个被揭示的位置
            int idx = indexQueue.front();
            indexQueue.pop();

            // 将第 i 小的牌放在该位置
            result[idx] = deck[i];

            // 模拟"把下一张移到底部"：将队首移到队尾
            if (!indexQueue.empty()) {
                indexQueue.push(indexQueue.front());
                indexQueue.pop();
            }
        }

        return result;
    }
};


// 解法二：逆向模拟（deque，从最大牌倒推）
// 时间复杂度: O(n log n)
// 空间复杂度: O(n)

class Solution2 {
public:
    vector<int> deckRevealedIncreasing(vector<int>& deck) {
        int n = deck.size();

        // 降序排序，从最大的牌开始逆向构建
        sort(deck.begin(), deck.end(), greater<int>());

        deque<int> dq;

        for (int i = 0; i < n; i++) {
            if (!dq.empty()) {
                // 逆操作：将最后一个元素移到最前面
                // （正向是"把顶部移到底部"，逆向是"把底部移到顶部"）
                dq.push_front(dq.back());
                dq.pop_back();
            }
            // 逆操作：将当前牌放到最前面
            // （正向是"从顶部取出"，逆向是"放回顶部"）
            dq.push_front(deck[i]);
        }

        // 将 deque 转为 vector
        return vector<int>(dq.begin(), dq.end());
    }
};


// 验证函数：模拟揭示过程，检查结果是否递增
// 可用于面试中展示测试思维

/*
bool verify(vector<int>& arranged) {
    queue<int> q;
    for (int card : arranged) {
        q.push(card);
    }

    vector<int> revealed;
    while (!q.empty()) {
        // 揭示顶牌
        revealed.push_back(q.front());
        q.pop();

        // 把下一张移到底部
        if (!q.empty()) {
            q.push(q.front());
            q.pop();
        }
    }

    // 检查揭示顺序是否递增
    for (int i = 1; i < (int)revealed.size(); i++) {
        if (revealed[i] <= revealed[i - 1]) return false;
    }
    return true;
}
*/
