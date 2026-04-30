// LeetCode 683: K Empty Slots - Reference Solution
// 难度: Hard
// 标签: Binary Indexed Tree, Segment Tree, Sliding Window, Ordered Set

// ============================================================
// 解法一: 滑动窗口（在 days 数组上）-- 推荐面试解法
// ============================================================
// 核心思路:
//   1. 构造 days[] 数组: days[x] = 位置 x 的灯在第几天打开
//   2. 在 days[] 上滑动窗口 [left, right], right = left + k + 1
//   3. 窗口内所有 days[i] 必须 > max(days[left], days[right])
//   4. 取所有合法窗口中 max(days[left], days[right]) 的最小值
//
// 时间复杂度: O(n) -- 每个位置最多被扫描两次
// 空间复杂度: O(n)

class Solution {
public:
    int kEmptySlots(vector<int>& bulbs, int k) {
        int n = bulbs.size();

        // 步骤1: 构造 days 数组
        // days[i] 表示位置 i+1 的灯在第几天被打开
        vector<int> days(n);
        for (int i = 0; i < n; i++) {
            days[bulbs[i] - 1] = i + 1;
        }

        // 步骤2: 滑动窗口
        int ans = INT_MAX;
        int left = 0, right = k + 1;
        int i = left + 1;

        while (right < n) {
            // 窗口内部为空 (k == 0)，直接验证
            if (left + 1 == right) {
                ans = min(ans, max(days[left], days[right]));
                left = right;
                right = left + k + 1;
                i = left + 1;
                continue;
            }

            if (i < right) {
                // 验证窗口内部元素
                if (days[i] < days[left] || days[i] < days[right]) {
                    // 窗口失败: i 位置的灯比端点更早打开
                    // 以 i 为新的左端点重新开始
                    left = i;
                    right = i + k + 1;
                    i = left + 1;
                } else {
                    i++;
                }
            } else {
                // i == right, 窗口内所有元素验证通过
                ans = min(ans, max(days[left], days[right]));
                // 移动到下一个窗口
                left = right;
                right = left + k + 1;
                i = left + 1;
            }
        }

        return ans == INT_MAX ? -1 : ans;
    }
};

// ============================================================
// 解法二: 有序集合 std::set（实现更直观）
// ============================================================
// 核心思路:
//   - 逐天打开灯泡，将位置插入 set
//   - 每次插入后检查前驱和后继的距离
//   - 若距离恰好为 k+1，返回当天的天数
//
// 时间复杂度: O(n log n) -- 每次 set 操作 O(log n)
// 空间复杂度: O(n)

class Solution2 {
public:
    int kEmptySlots(vector<int>& bulbs, int k) {
        set<int> opened;

        for (int day = 0; day < (int)bulbs.size(); day++) {
            int pos = bulbs[day];
            auto it = opened.insert(pos).first;

            // 检查右邻居: 是否与右边最近的亮灯距离恰好 k+1
            auto right = next(it);
            if (right != opened.end() && *right - pos == k + 1) {
                return day + 1;  // 天数从 1 开始
            }

            // 检查左邻居: 是否与左边最近的亮灯距离恰好 k+1
            if (it != opened.begin()) {
                auto left = prev(it);
                if (pos - *left == k + 1) {
                    return day + 1;
                }
            }
        }

        return -1;
    }
};

// ============================================================
// 解法三: 树状数组 / Binary Indexed Tree（区间查询版）
// ============================================================
// 核心思路:
//   - 用 BIT 维护每个位置的灯泡状态（0/1）
//   - 每次打开一个灯后，检查左右两侧距离 k+1 处是否有亮灯
//   - 同时用 BIT 区间查询 [pos-k, pos-1] 和 [pos+1, pos+k]
//     是否全为 0（中间没有亮灯）
//
// 时间复杂度: O(n log n)
// 空间复杂度: O(n)

class Solution3 {
    vector<int> bit;
    int sz;

    void update(int i, int delta) {
        for (; i <= sz; i += i & (-i))
            bit[i] += delta;
    }

    int query(int i) {
        int s = 0;
        for (; i > 0; i -= i & (-i))
            s += bit[i];
        return s;
    }

    // 区间和 [l, r]
    int rangeQuery(int l, int r) {
        if (l > r) return 0;
        return query(r) - query(l - 1);
    }

public:
    int kEmptySlots(vector<int>& bulbs, int k) {
        int n = bulbs.size();
        sz = n;
        bit.assign(n + 1, 0);

        for (int day = 0; day < n; day++) {
            int pos = bulbs[day];
            update(pos, 1);  // 打开位置 pos 的灯

            // 检查左边: 位置 pos - k - 1 是否亮着
            // 且 [pos-k, pos-1] 之间全部没亮
            int leftPos = pos - k - 1;
            if (leftPos >= 1) {
                // leftPos 处是否亮着
                int leftOn = rangeQuery(leftPos, leftPos);
                // 中间是否全灭
                int between = rangeQuery(leftPos + 1, pos - 1);
                if (leftOn == 1 && between == 0) {
                    return day + 1;
                }
            }

            // 检查右边: 位置 pos + k + 1 是否亮着
            // 且 [pos+1, pos+k] 之间全部没亮
            int rightPos = pos + k + 1;
            if (rightPos <= n) {
                int rightOn = rangeQuery(rightPos, rightPos);
                int between = rangeQuery(pos + 1, rightPos - 1);
                if (rightOn == 1 && between == 0) {
                    return day + 1;
                }
            }
        }

        return -1;
    }
};
