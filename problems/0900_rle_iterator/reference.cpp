/*
 * LeetCode 900: RLE Iterator (RLE 迭代器)
 *
 * 【题目本质】
 * 设计 RLE 编码的迭代器。编码格式：[count, value, count, value, ...]
 * next(n) 消耗 n 个元素，返回第 n 个消耗的元素值。
 * 核心：不展开 RLE，直接在编码数组上用指针跳跃消耗计数。
 *
 * 【解法总览】
 * 解法1: 展开 RLE 数组(MLE)      — O(total) / O(total) — 理解题意用
 * 解法2: 指针跳跃(修改原数组)     — O(均摊) / O(1)额外  — 面试首选
 * 解法3: 指针跳跃(额外remaining) — O(均摊) / O(1)额外  — 不修改原数组
 */

// ============================================================
// 解法1: 展开 RLE 数组 — 仅用于理解题意 (会 MLE)
// 时间: O(total) 初始化  空间: O(total)
//
// 【思路】
// 先把 RLE 完全展开成原始数组，然后用指针逐步前进。
// 这是最直觉的做法，但 encoding[i] 可达 10^9，展开后数组太大。
//
// 示例: encoding = [3, 8, 0, 9, 2, 5]
//   展开: [8, 8, 8, 5, 5]  (3个8, 0个9, 2个5)
//
//   next(2): ptr 从 0 跳到 2, 返回 expanded[1] = 8
//   next(1): ptr 从 2 跳到 3, 返回 expanded[2] = 8
//   next(1): ptr 从 3 跳到 4, 返回 expanded[3] = 5
//   next(2): ptr 从 4 跳到 6, 超出范围, 返回 -1
//
// 为什么会 MLE?
//   encoding = [1000000000, 1, 1000000000, 2]
//   展开后 20 亿个元素，约 8GB 内存
// ============================================================
class Solution1_RLEIterator {
public:
    vector<int> expanded;
    int ptr;

    Solution1_RLEIterator(vector<int>& encoding) {
        ptr = 0;
        // 展开 RLE
        for (int i = 0; i < (int)encoding.size(); i += 2) {
            for (int j = 0; j < encoding[i]; j++) {
                expanded.push_back(encoding[i + 1]);
            }
        }
    }

    int next(int n) {
        ptr += n;
        if (ptr > (int)expanded.size()) {
            return -1;
        }
        // ptr-1 是第 n 个消耗的元素（0-indexed）
        return expanded[ptr - 1];
    }
};

// ============================================================
// 解法2: 指针跳跃 — 直接修改 encoding 记录剩余 — 面试首选 ⭐
// 时间: O(1) 初始化, O(均摊) next  空间: O(1) 额外
//
// 【思路】
// 不展开 RLE！直接在编码数组上操作。
//
// 维护指针 idx 指向当前段（encoding 的偶数索引位置）。
// 直接修改 encoding[idx] 记录当前段的剩余计数。
//
// next(n) 的逻辑：
//   while (idx 没越界):
//     if encoding[idx] >= n:
//       当前段够消耗 → encoding[idx] -= n, 返回 encoding[idx+1]
//     else:
//       当前段不够 → n -= encoding[idx], idx += 2 (跳到下一段)
//   所有段消耗完了 → 返回 -1
//
// 示例详解: encoding = [3, 8, 0, 9, 2, 5]
//
// 初始状态: idx=0, encoding = [3, 8, 0, 9, 2, 5]
//
// next(2):
//   idx=0: enc[0]=3 >= 2? YES → enc[0]=3-2=1, return enc[1]=8
//   状态: idx=0, encoding = [1, 8, 0, 9, 2, 5]
//
// next(1):
//   idx=0: enc[0]=1 >= 1? YES → enc[0]=1-1=0, return enc[1]=8
//   状态: idx=0, encoding = [0, 8, 0, 9, 2, 5]
//
// next(1):
//   idx=0: enc[0]=0 >= 1? NO → n=1-0=1, idx=2
//   idx=2: enc[2]=0 >= 1? NO → n=1-0=1, idx=4
//   idx=4: enc[4]=2 >= 1? YES → enc[4]=2-1=1, return enc[5]=5
//   状态: idx=4, encoding = [0, 8, 0, 9, 1, 5]
//
// next(2):
//   idx=4: enc[4]=1 >= 2? NO → n=2-1=1, idx=6
//   idx=6: 越界! return -1
//
// 均摊分析：
//   每段(count,value对)最多被跳过一次(跳过后idx永不回退)。
//   encoding.length/2 段, 最多1000次next调用。
//   总时间 = O(encoding.length/2 + 1000) = O(encoding.length + Q)
//   其中 Q 是 next 调用次数。
// ============================================================
class Solution2_RLEIterator {
public:
    vector<int> enc;
    int idx;

    Solution2_RLEIterator(vector<int>& encoding) {
        enc = encoding;
        idx = 0;
    }

    int next(int n) {
        while (idx < (int)enc.size()) {
            if (enc[idx] >= n) {
                // 当前段的剩余计数够消耗 n 个
                enc[idx] -= n;
                return enc[idx + 1];  // 返回当前段的值
            } else {
                // 当前段不够，消耗完整段后跳到下一段
                n -= enc[idx];  // 减去当前段所有剩余
                idx += 2;       // 移动到下一段 (每段占2个位置)
            }
        }

        // 所有段都消耗完了，还需要更多元素
        return -1;
    }
};

// ============================================================
// 解法3: 指针跳跃 — 用额外变量 remaining，不修改原数组
// 时间: O(1) 初始化, O(均摊) next  空间: O(1) 额外
//
// 【思路】
// 和解法2 逻辑完全一样，但不修改原数组。
// 用 remaining 变量记录当前段的剩余计数。
//
// 这种写法更"纯净"——不会破坏输入数据。
// 如果后续需要重置迭代器或多次遍历，用这种写法更方便。
//
// 注意：用 long long 防止连续减法中间值溢出。
// 虽然单个 encoding[i] 和 n 都不超过 10^9 (int 范围)，
// 但如果要做累加判断或某些中间计算，long long 更安全。
// ============================================================
class Solution3_RLEIterator {
public:
    vector<int> enc;
    int idx;
    long long remaining;

    Solution3_RLEIterator(vector<int>& encoding) {
        enc = encoding;
        idx = 0;
        // 初始化 remaining 为第一段的计数
        remaining = (idx < (int)enc.size()) ? enc[0] : 0;
    }

    int next(int n) {
        long long need = n;

        while (idx < (int)enc.size()) {
            if (remaining >= need) {
                // 当前段够消耗
                remaining -= need;
                return enc[idx + 1];
            } else {
                // 当前段不够，消耗完后移到下一段
                need -= remaining;
                idx += 2;
                // 更新 remaining 为新段的计数
                remaining = (idx < (int)enc.size()) ? enc[idx] : 0;
            }
        }

        return -1;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 初始化 | next 均摊 | 额外空间 | 优势                    | 劣势                |
// |--------------|--------|----------|---------|------------------------|---------------------|
// | 展开 RLE      | O(total)| O(1)    | O(total) | 逻辑最简单             | MLE (10^9 元素)     |
// | 修改原数组    | O(n)   | O(段/调用) | O(1)    | 代码最简洁              | 修改了输入           |
// | 额外remaining | O(n)   | O(段/调用) | O(1)    | 不修改输入，易重置       | 代码稍长             |
//
// n = encoding.length (拷贝数组的开销)
// 面试选择：解法2，代码最短逻辑最清晰。
//
// ============================================================
// 【易错点】
//
// 1. 展开 RLE 数组:
//    ✗ for (j < encoding[i]) expanded.push_back(...) → 10^9 次循环，MLE
//    ✓ 直接在编码数组上操作计数，不展开
//
// 2. idx 步进 +1 而不是 +2:
//    ✗ idx++ → 跳到了 value 位置而不是下一段的 count 位置
//    ✓ idx += 2，因为每段是 (count, value) 占两个位置
//
// 3. 忘记 >= 边界:
//    ✗ if (enc[idx] > n) → 恰好等于时漏处理
//    ✓ if (enc[idx] >= n) → 等于时消耗完当前段（剩余变0）
//
// 4. count=0 的段没处理:
//    ✗ 假设每段 count > 0 → encoding 允许 count=0
//    ✓ 代码自然处理：enc[idx]=0 < n，直接跳过该段
//
// 5. 返回值搞混:
//    ✗ return enc[idx] → 返回的是 count 而不是 value
//    ✓ return enc[idx + 1] → 值在奇数索引位置
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 所有 next 调用的总时间复杂度是多少？
//     O(encoding.length/2 + Q)，其中 Q 是调用次数。
//     每段最多被跳过一次（idx 单调递增不回退），
//     总跳段次数 ≤ encoding.length/2。
//     加上每次 next 调用的 O(1) 基础操作。
//
// Q2: 如果要支持 prev(n)（回退 n 个元素），怎么改？
//     需要 idx 能回退。不能修改原数组了（需要恢复计数），
//     用解法3 的 remaining 方案。回退时 idx -= 2 并恢复
//     remaining = enc[idx] - (之前消耗的)。
//     最简单的做法：保存原始 encoding + 当前全局偏移量。
//
// Q3: 如果 encoding 非常长但大部分 count=0，怎么优化？
//     预处理：构建一个"非零段"的索引数组，跳过所有 count=0 的段。
//     或者在初始化时过滤掉 count=0 的段。
//
// Q4: 如何实现 reset()？
//     解法2 修改了原数组，需要保存副本。
//     解法3 只需 idx=0, remaining=enc[0] 即可，原数组未修改。
//
// ============================================================
// 【相关题型】
//
// - 284. 窥探迭代器 (Peeking Iterator):
//   迭代器设计题。实现 peek() 预读下一个元素。
//   核心：用缓存变量存预读值。本题的 next 更复杂（跳跃消耗）。
//
// - 341. 扁平化嵌套列表迭代器:
//   处理嵌套结构的迭代器。用栈/递归展平。
//   本题是一维压缩，341 是多维嵌套。
//
// - 604. 迭代压缩字符串:
//   和本题最相似！也是压缩编码迭代器。
//   区别在编码格式（字母+数字 vs 纯数字对）。
//
// - 443. 压缩字符串:
//   RLE 编码的"反方向"——把原始字符串压缩为 RLE 格式。
//   理解 RLE 编解码有助于解这两道题。
// ============================================================
