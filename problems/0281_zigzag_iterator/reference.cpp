/*
 * 【题目本质】
 * 多路迭代器的轮询调度：交替读取多个向量的元素。
 * 核心在于用队列管理"活跃"的迭代器，实现Round-Robin调度。
 *
 * 【解法总览】
 * Solution1: 队列 + 迭代器对 — 通用解法，天然支持 k 个向量 ⭐推荐
 * Solution2: 双指针交替   — 仅适用于 2 个向量的简化版
 */

// ===================== Solution1: 队列 + 迭代器对 =====================
// 思路：将每个非空向量的 (当前位置, 结束位置) 存入队列
//       每次 next() 从队头取出一个迭代器，读取值，若未耗尽则放回队尾
// 时间：O(1) per next/hasNext
// 空间：O(k)，k 为向量个数
class ZigzagIterator1 {
    // 队列中存储 <当前迭代器, 结束迭代器> 对
    queue<pair<vector<int>::iterator, vector<int>::iterator>> q;
public:
    ZigzagIterator1(vector<int>& v1, vector<int>& v2) {
        // 只将非空向量的迭代器加入队列
        if (!v1.empty()) q.push({v1.begin(), v1.end()});
        if (!v2.empty()) q.push({v2.begin(), v2.end()});
    }

    int next() {
        // 取出队头的迭代器对
        auto [cur, end] = q.front();
        q.pop();
        int val = *cur;
        ++cur;
        // 如果该向量还有剩余元素，放回队尾
        if (cur != end) {
            q.push({cur, end});
        }
        return val;
    }

    bool hasNext() {
        return !q.empty();
    }
};

// ===================== Solution2: 双指针交替 =====================
// 思路：维护两个索引和一个布尔标志，交替读取
//       当一方耗尽时，自动切换到另一方
// 时间：O(1) per next/hasNext
// 空间：O(1) 额外空间（不含存储输入）
// 局限：不易扩展到 k 个向量
class ZigzagIterator2 {
    vector<int>& v1_;
    vector<int>& v2_;
    int i1 = 0, i2 = 0;
    bool pickFirst = true;  // true = 下次优先从 v1 取
public:
    ZigzagIterator2(vector<int>& v1, vector<int>& v2)
        : v1_(v1), v2_(v2) {}

    int next() {
        int val;
        // 优先按 pickFirst 指示读取，但若该方已耗尽则读另一方
        if (i1 < (int)v1_.size() && (pickFirst || i2 >= (int)v2_.size())) {
            val = v1_[i1++];
            pickFirst = false;
        } else {
            val = v2_[i2++];
            pickFirst = true;
        }
        return val;
    }

    bool hasNext() {
        return i1 < (int)v1_.size() || i2 < (int)v2_.size();
    }
};

// ============= 扩展: k 个向量版本 (基于 Solution1) =============
class ZigzagIteratorK {
    queue<pair<vector<int>::iterator, vector<int>::iterator>> q;
public:
    ZigzagIteratorK(vector<vector<int>>& vecs) {
        for (auto& v : vecs) {
            if (!v.empty()) q.push({v.begin(), v.end()});
        }
    }
    int next() {
        auto [cur, end] = q.front(); q.pop();
        int val = *cur++;
        if (cur != end) q.push({cur, end});
        return val;
    }
    bool hasNext() { return !q.empty(); }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 可扩展性 | 推荐场景 |
 * |------|------|------|----------|----------|
 * | 队列+迭代器对 | O(1) | O(k) | k个向量 | 面试首选 |
 * | 双指针交替   | O(1) | O(1) | 仅2个   | 快速实现 |
 *
 * 【易错点】
 * 1. 空向量处理：不应将空向量的迭代器加入队列
 * 2. 迭代器失效：如果存的是引用/指针，需确保底层容器生命周期
 * 3. 交替逻辑：当一方耗尽时不应死板交替，应自动切换
 *
 * 【面试追问】
 * Q1: 扩展到 k 个向量? -> 队列解法零改动，构造时 push 所有非空迭代器
 * Q2: 线程安全? -> 互斥锁保护 queue 操作；注意 hasNext+next 的原子性
 * Q3: 超大数据集? -> 使用文件流迭代器，每次只读一个 chunk
 */
