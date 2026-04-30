// 715. Range Module
// 难度: Hard
// 标签: Design, Segment Tree, Ordered Set
//
// 思路: 使用有序Map (std::map) 维护不相交区间集合
// key=左端点, value=右端点, 表示半开区间 [key, value)
// addRange: 合并重叠区间; queryRange: 查找覆盖区间; removeRange: 分裂区间
//
// 时间复杂度: addRange/removeRange O(n) 最坏, queryRange O(log n)
// 空间复杂度: O(n), n为当前区间数量

// ============================================================
// 解法一: 有序 Map（推荐面试解法）
// ============================================================
class RangeModule {
private:
    // intervals_[left] = right 表示区间 [left, right)
    map<int, int> intervals_;

public:
    RangeModule() {}

    // 添加区间 [left, right)，合并所有重叠区间
    void addRange(int left, int right) {
        // 找到第一个左端点 > left 的区间
        auto it = intervals_.upper_bound(left);

        // 检查前一个区间是否与 [left, right) 重叠
        if (it != intervals_.begin()) {
            --it;
            if (it->second < left) {
                // 前一个区间不重叠，跳过
                ++it;
            }
        }

        // 合并所有重叠区间
        while (it != intervals_.end() && it->first <= right) {
            left = min(left, it->first);
            right = max(right, it->second);
            it = intervals_.erase(it);  // 删除并获取下一个迭代器
        }

        // 插入合并后的区间
        intervals_[left] = right;
    }

    // 查询 [left, right) 是否完全被覆盖
    bool queryRange(int left, int right) {
        // 找到第一个左端点 > left 的区间
        auto it = intervals_.upper_bound(left);

        // 没有左端点 <= left 的区间存在
        if (it == intervals_.begin()) return false;

        // 前一个区间的右端点必须 >= right 才能完全覆盖
        --it;
        return it->second >= right;
    }

    // 删除区间 [left, right)
    void removeRange(int left, int right) {
        // 找到第一个左端点 > left 的区间
        auto it = intervals_.upper_bound(left);

        // 检查前一个区间是否与 [left, right) 重叠
        if (it != intervals_.begin()) {
            --it;
            if (it->second < left) {
                ++it;  // 不重叠，跳过
            }
        }

        // 用临时变量保存需要添加的残留区间
        vector<pair<int,int>> toAdd;

        // 删除所有重叠区间，保留残留部分
        while (it != intervals_.end() && it->first < right) {
            // 左残留: [origLeft, left)
            if (it->first < left) {
                toAdd.push_back({it->first, left});
            }
            // 右残留: [right, origRight)
            if (it->second > right) {
                toAdd.push_back({right, it->second});
            }
            it = intervals_.erase(it);
        }

        // 插入残留区间
        for (auto& [l, r] : toAdd) {
            intervals_[l] = r;
        }
    }
};

// ============================================================
// 解法二: 有序 Map 的精简写法（批量删除）
// ============================================================
class RangeModule_V2 {
private:
    map<int, int> intervals_;

public:
    RangeModule_V2() {}

    void addRange(int left, int right) {
        // 获取所有受影响的区间范围
        auto lo = intervals_.upper_bound(left);
        auto hi = intervals_.upper_bound(right);

        if (lo != intervals_.begin()) {
            --lo;
            if (lo->second < left) ++lo;  // 不重叠，跳过
        }

        // 合并范围
        if (lo != hi) {
            left = min(left, lo->first);
            right = max(right, prev(hi)->second);
            intervals_.erase(lo, hi);  // 批量删除
        }

        intervals_[left] = right;
    }

    bool queryRange(int left, int right) {
        auto it = intervals_.upper_bound(left);
        if (it == intervals_.begin()) return false;
        --it;
        return it->second >= right;
    }

    void removeRange(int left, int right) {
        auto lo = intervals_.upper_bound(left);
        auto hi = intervals_.upper_bound(right);

        if (lo != intervals_.begin()) {
            --lo;
            if (lo->second < left) ++lo;
        }

        // 保存可能的残留
        int newLeft = -1, newRight = -1;
        int tailLeft = -1, tailRight = -1;

        if (lo != hi) {
            // 左残留
            if (lo->first < left) {
                newLeft = lo->first;
                newRight = left;
            }
            // 右残留
            auto last = prev(hi);
            if (last->second > right) {
                tailLeft = right;
                tailRight = last->second;
            }
            intervals_.erase(lo, hi);
        }

        if (newLeft != -1) intervals_[newLeft] = newRight;
        if (tailLeft != -1) intervals_[tailLeft] = tailRight;
    }
};

// ============================================================
// 解法三: 动态开点线段树（竞赛向）
// ============================================================
class RangeModule_V3 {
private:
    // 动态开点线段树节点
    // 值域 [1, 10^9]
    // lazy: 0=无操作, 1=全部覆盖, -1=全部清除
    struct Node {
        int left = 0, right = 0;  // 子节点编号
        bool covered = false;     // 该区间是否完全覆盖
        int lazy = 0;             // 懒标记
    };

    vector<Node> tree;
    int root = 0;

    int newNode() {
        tree.push_back({});
        return tree.size() - 1;
    }

    void pushDown(int node) {
        if (tree[node].lazy == 0) return;

        if (tree[node].left == 0) tree[node].left = newNode();
        if (tree[node].right == 0) tree[node].right = newNode();

        int lz = tree[node].lazy;
        tree[tree[node].left].covered = (lz == 1);
        tree[tree[node].left].lazy = lz;
        tree[tree[node].right].covered = (lz == 1);
        tree[tree[node].right].lazy = lz;
        tree[node].lazy = 0;
    }

    void update(int& node, int lo, int hi, int l, int r, bool val) {
        if (node == 0) node = newNode();
        if (l <= lo && hi <= r) {
            tree[node].covered = val;
            tree[node].lazy = val ? 1 : -1;
            return;
        }
        pushDown(node);
        int mid = lo + (hi - lo) / 2;
        if (l <= mid) update(tree[node].left, lo, mid, l, r, val);
        if (r > mid) update(tree[node].right, mid + 1, hi, l, r, val);
        tree[node].covered = (tree[node].left && tree[tree[node].left].covered) &&
                              (tree[node].right && tree[tree[node].right].covered);
    }

    bool query(int node, int lo, int hi, int l, int r) {
        if (node == 0) return false;
        if (l <= lo && hi <= r) return tree[node].covered;
        pushDown(node);
        int mid = lo + (hi - lo) / 2;
        bool res = true;
        if (l <= mid) res = res && query(tree[node].left, lo, mid, l, r);
        if (r > mid) res = res && query(tree[node].right, mid + 1, hi, l, r);
        return res;
    }

    static const int MAXVAL = 1000000000;

public:
    RangeModule_V3() {
        tree.push_back({});  // 哨兵节点0
        root = newNode();    // 真正的根节点
    }

    void addRange(int left, int right) {
        update(root, 1, MAXVAL, left, right - 1, true);
    }

    bool queryRange(int left, int right) {
        return query(root, 1, MAXVAL, left, right - 1);
    }

    void removeRange(int left, int right) {
        update(root, 1, MAXVAL, left, right - 1, false);
    }
};

// ============================================================
// 关键点总结:
// 1. 解法一/二用有序Map，简洁高效，面试首选
// 2. addRange: 找重叠区间 -> 合并 -> 删旧插新
// 3. queryRange: upper_bound后退一步，检查右端点覆盖
// 4. removeRange: 最复杂，注意左残留和右残留的处理
// 5. 解法二用 erase(lo, hi) 批量删除，更精简
// 6. 解法三线段树适合竞赛，面试中除非被要求否则不用
// ============================================================
