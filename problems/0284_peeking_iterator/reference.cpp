/*
 * 【题目本质】
 * 在已有迭代器上增加 peek() 功能——查看下一个元素但不推进指针。
 * 核心是缓存预读模式(Read-Ahead Cache)，装饰器设计模式的经典应用。
 *
 * 【解法总览】
 * Solution1: 缓存标志法 — bool + 缓存值，按需预读 ⭐推荐
 * Solution2: 始终预读法 — 构造时预读，next()中推进预读
 */

// ===================== Solution1: 缓存标志法 =====================
// 思路：用 hasPeeked 标志 + peekedVal 缓存
//       peek(): 未缓存时调底层 next() 缓存起来
//       next(): 有缓存返回缓存并清标志，否则直接调底层
//       hasNext(): 有缓存 || 底层 hasNext()
// 时间：O(1) per call
// 空间：O(1)
class PeekingIterator1 : public Iterator {
    bool hasPeeked;
    int peekedVal;
public:
    PeekingIterator1(const vector<int>& nums) : Iterator(nums) {
        hasPeeked = false;
        peekedVal = 0;
    }

    // 查看下一个元素但不推进
    int peek() {
        if (!hasPeeked) {
            peekedVal = Iterator::next();  // 预读一个
            hasPeeked = true;
        }
        return peekedVal;
    }

    int next() {
        if (hasPeeked) {
            // 有缓存 -> 返回缓存值，清除标志
            hasPeeked = false;
            return peekedVal;
        }
        return Iterator::next();
    }

    bool hasNext() const {
        // 缓存中有值 或 底层还有元素
        return hasPeeked || Iterator::hasNext();
    }
};

// ===================== Solution2: 始终预读法 =====================
// 思路：始终保持 nextVal 为"下一个要返回的值"
//       构造时就预读第一个值
//       每次 next() 返回当前 nextVal 并预读下一个
//       peek() 直接返回 nextVal
// 时间：O(1) per call
// 空间：O(1)
// 特点：逻辑更统一，但构造时就会消费底层迭代器的第一个元素
class PeekingIterator2 : public Iterator {
    int nextVal;
    bool hasNextVal;
public:
    PeekingIterator2(const vector<int>& nums) : Iterator(nums) {
        hasNextVal = Iterator::hasNext();
        if (hasNextVal) {
            nextVal = Iterator::next();
        }
    }

    int peek() {
        return nextVal;  // 总是有预读值，直接返回
    }

    int next() {
        int ret = nextVal;
        // 推进预读：尝试读下一个
        hasNextVal = Iterator::hasNext();
        if (hasNextVal) {
            nextVal = Iterator::next();
        }
        return ret;
    }

    bool hasNext() const {
        return hasNextVal;
    }
};

// ===================== 泛型版本示例 (Follow-up) =====================
// 使用 C++ 模板 + optional 实现通用 PeekingIterator
// template<typename T>
// class GenericPeekingIterator {
//     Iterator<T>& iter;
//     optional<T> cached;
// public:
//     GenericPeekingIterator(Iterator<T>& it) : iter(it) {}
//
//     T peek() {
//         if (!cached.has_value()) {
//             cached = iter.next();
//         }
//         return cached.value();
//     }
//
//     T next() {
//         if (cached.has_value()) {
//             T val = cached.value();
//             cached.reset();
//             return val;
//         }
//         return iter.next();
//     }
//
//     bool hasNext() {
//         return cached.has_value() || iter.hasNext();
//     }
// };

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | 缓存标志法 | O(1) | O(1) | 按需预读，逻辑直观 |
 * | 始终预读法 | O(1) | O(1) | 逻辑统一，构造时预读 |
 *
 * 【易错点】
 * 1. peek()多次调用: 只应在第一次时调底层next()，后续返回缓存
 * 2. hasNext()遗漏缓存: 缓存中有值时即使底层空了也应返回true
 * 3. 不能拷贝nums: 题目明确要求只能用Iterator接口方法
 * 4. const correctness: hasNext()需标记const匹配基类签名
 *
 * 【面试追问】
 * Q1: 泛型扩展? -> 模板类 + optional<T> 替代 bool+int
 * Q2: 设计模式? -> 装饰器模式(Decorator Pattern)
 * Q3: 底层有副作用? -> peek()会提前触发，需延迟执行策略
 */
