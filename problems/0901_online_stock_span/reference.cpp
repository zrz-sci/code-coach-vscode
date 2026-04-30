/*
 * LeetCode 901: Online Stock Span
 *
 * 解法1: 单调递减栈 (面试首选)
 * 时间: 均摊 O(1) 每次调用  空间: O(n)
 *
 * 思路: 维护一个 (price, span) 的单调递减栈。
 * 每次 next(price) 时，不断弹出栈中 <= price 的元素，
 * 将它们的跨度累加，然后将 (price, 累积跨度) 入栈。
 */

// ==================== 解法1: 单调递减栈 ====================

class StockSpanner {
    // 单调递减栈: 从底到顶价格严格递减
    // 每个元素是 (price, span)
    // span 表示从该天往左连续 <= price 的天数(含自身)
    stack<pair<int,int>> st;

public:
    StockSpanner() {
        // 栈初始为空，无需额外操作
    }

    int next(int price) {
        // 当前天自身的跨度为 1
        int span = 1;

        // 核心: 弹出所有 <= 当前价格的栈顶元素
        // 这些元素被当前价格"支配"，永远不会再被需要
        // 同时把它们的跨度累加到当前元素上
        while (!st.empty() && st.top().first <= price) {
            span += st.top().second;
            st.pop();
        }

        // 当前 (price, span) 入栈
        // 此时栈仍保持单调递减性质
        st.push({price, span});

        return span;
    }
};

/*
 * 解法2: 单调栈 + 哨兵节点优化
 * 时间: 均摊 O(1) 每次调用  空间: O(n)
 *
 * 在栈底放一个 (INT_MAX, 0) 哨兵，省去 empty() 检查
 */

class StockSpanner_V2 {
    stack<pair<int,int>> st;

public:
    StockSpanner_V2() {
        // 哨兵节点: 价格为 INT_MAX，永远不会被弹出
        // span 为 0，因为它不代表真实的交易日
        st.push({INT_MAX, 0});
    }

    int next(int price) {
        int span = 1;

        // 因为哨兵的存在，不需要检查 st.empty()
        // INT_MAX 保证循环一定会在哨兵处停止
        while (st.top().first <= price) {
            span += st.top().second;
            st.pop();
        }

        st.push({price, span});
        return span;
    }
};

/*
 * 解法3: 使用数组模拟栈 + 索引跳跃
 * 时间: 均摊 O(1)  空间: O(n)
 *
 * 不显式弹栈，而是用索引跳跃回溯。
 * 每个位置记录 (price, span)，利用 span 进行跳跃。
 */

class StockSpanner_V3 {
    // 用 vector 模拟"栈"，存储所有历史 (price, span)
    vector<pair<int,int>> history;

public:
    StockSpanner_V3() {}

    int next(int price) {
        int span = 1;

        // 利用已记录的 span 进行跳跃式回溯
        // 不需要逐天检查，可以跳过整个跨度
        int idx = history.size() - 1;
        while (idx >= 0 && history[idx].first <= price) {
            span += history[idx].second;
            idx -= history[idx].second; // 跳跃: 跳过整个跨度
        }

        history.push_back({price, span});
        return span;
    }
};

/*
 * 解法4: 暴力法 (作为基线参考)
 * 时间: O(n) 每次调用  空间: O(n)
 *
 * 存储所有历史价格，每次从后往前线性扫描。
 * 面试中用于说明为什么需要单调栈优化。
 */

class StockSpanner_Brute {
    vector<int> prices;

public:
    StockSpanner_Brute() {}

    int next(int price) {
        prices.push_back(price);
        int span = 1;
        int i = (int)prices.size() - 2;

        // 从前一天开始逐天往回检查
        while (i >= 0 && prices[i] <= price) {
            span++;
            i--;
        }

        return span;
    }
};

/*
 * ==================== 均摊分析 ====================
 *
 * 为什么单调栈解法是均摊 O(1)?
 *
 * 使用"势能法"(Potential Method) 分析:
 * - 令势能函数 Phi = 栈中元素个数
 * - 每次 next(price):
 *     实际操作: 弹出 k 个元素 + 压入 1 个元素
 *     实际代价: O(k + 1)
 *     势能变化: delta_Phi = 1 - k (压入1个，弹出k个)
 *     均摊代价: (k + 1) + (1 - k) = 2 = O(1)
 *
 * 或者更直观地理解:
 * - 每个元素最多被 push 一次、pop 一次
 * - n 次调用总共最多 2n 次栈操作
 * - 均摊每次调用: 2n / n = O(1)
 *
 * ==================== 复杂度总结 ====================
 *
 * | 解法         | 时间(每次)   | 时间(n次) | 空间  |
 * |-------------|-------------|-----------|-------|
 * | 单调栈       | 均摊 O(1)   | O(n)      | O(n)  |
 * | 单调栈+哨兵  | 均摊 O(1)   | O(n)      | O(n)  |
 * | 数组跳跃     | 均摊 O(1)   | O(n)      | O(n)  |
 * | 暴力         | O(n)        | O(n^2)    | O(n)  |
 *
 * 面试策略:
 * 1. 先说暴力法和它的问题 (最坏 O(n^2))
 * 2. 观察到被吞并的元素永远不需要再看 → 单调栈
 * 3. 写出解法1 (最简洁)
 * 4. 能解释均摊分析
 */
