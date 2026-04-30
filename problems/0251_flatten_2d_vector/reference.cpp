/*
 * 【题目本质】
 * 设计迭代器将二维向量展开为一维顺序访问。
 * 核心: 双指针(outer行/inner列) + advance()跳过空行。
 * hasNext()中调用advance确保指针有效, next()返回当前元素并推进。
 *
 * 【解法总览】
 * 解法1 (推荐): 双指针 + advance — 构造O(1), next/hasNext均摊O(1), 空间O(1)
 * 解法2: 预展开一维数组 — 构造O(N), next/hasNext O(1), 空间O(N)
 */

// ===================== 解法1: 双指针 + advance (推荐) =====================
// 思路: outer/inner两个索引, advance()负责跳过空行
// 关键: 存储引用避免拷贝, advance保证指针始终有效
class Vector2D_Sol1 {
public:
    Vector2D_Sol1(vector<vector<int>>& vec) : data(vec), outer(0), inner(0) {}

    int next() {
        // 先确保指针有效 (跳过空行)
        hasNext();
        return data[outer][inner++];
    }

    bool hasNext() {
        // advance: 跳过空行, 找到下一个有效元素
        advance();
        return outer < (int)data.size();
    }

private:
    vector<vector<int>>& data;
    int outer, inner;

    // 核心辅助: 当 inner 到达行尾时, 移到下一行
    void advance() {
        while (outer < (int)data.size() && inner >= (int)data[outer].size()) {
            outer++;
            inner = 0;
        }
    }
};

// ===================== 解法2: 预展开一维数组 =====================
// 思路: 构造时展开所有元素, 用单指针遍历
// 优点: 逻辑简单; 缺点: O(N)额外空间, 不符合迭代器设计哲学
class Vector2D_Sol2 {
public:
    Vector2D_Sol2(vector<vector<int>>& vec) : idx(0) {
        for (const auto& row : vec) {
            for (int val : row) {
                flat.push_back(val);
            }
        }
    }

    int next() {
        return flat[idx++];
    }

    bool hasNext() {
        return idx < (int)flat.size();
    }

private:
    vector<int> flat;
    int idx;
};

// ===================== Follow-up: 用 C++ iterator 实现 =====================
// 面试常考进阶版本
class Vector2D_Iterator {
public:
    Vector2D_Iterator(vector<vector<int>>& vec) : data(vec) {
        outer = data.begin();
        if (outer != data.end()) {
            inner = outer->begin();
        }
        advance();
    }

    int next() {
        hasNext();  // 确保有效
        return *inner++;
    }

    bool hasNext() {
        advance();
        return outer != data.end();
    }

private:
    vector<vector<int>>& data;
    vector<vector<int>>::iterator outer;
    vector<int>::iterator inner;

    void advance() {
        while (outer != data.end()) {
            if (inner != outer->end()) return;  // 找到有效元素
            outer++;
            if (outer != data.end()) inner = outer->begin();
        }
    }
};

/*
 * 【解法对比】
 * | 解法 | 构造 | next | hasNext | 空间 | 优势 |
 * |------|------|------|---------|------|------|
 * | 双指针 | O(1) | O(1)* | O(1)* | O(1) | 最优, 惰性求值 |
 * | 预展开 | O(N) | O(1) | O(1) | O(N) | 简单直接 |
 * | Iterator版 | O(1) | O(1)* | O(1)* | O(1) | 面试加分 |
 * (* 均摊复杂度)
 *
 * 【易错点】
 * 1. ✗ 忽略空行 [[],[3]] → ✓ advance()跳过 data[outer].size()==0 的行
 * 2. ✗ next()中不调用hasNext() → ✓ 必须先advance确保指针有效
 *    连续调用next()时, 指针可能停留在空行的末尾
 * 3. ✗ 拷贝vec → ✓ 存储引用 vector<vector<int>>& data
 * 4. ✗ hasNext()有副作用(移动指针) → ✓ advance是幂等的
 *    多次调用hasNext()不改变结果
 *
 * 【面试追问】
 * Q1: 用C++ iterator实现? → 见上方 Vector2D_Iterator
 * Q2: 支持remove()? → next()后标记, remove()执行删除+指针调整
 * Q3: 如何处理嵌套多层? → 用栈存储每层的iterator (LC 341)
 */
