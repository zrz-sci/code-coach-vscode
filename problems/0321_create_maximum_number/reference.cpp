/*
 * 【题目本质】
 * 从两个数组中共取k个数字（保持各自相对顺序），组成最大数
 * 分解为三步：枚举分配 -> 单调栈取最大子序列 -> 贪心合并
 *
 * 【解法总览】
 * Solution1: 标准三步法 — 单调栈 + 枚举分配 + 字典序归并
 * Solution2: 简化合并（利用 vector 字典序比较）
 */

// ===================== Solution1: 标准三步法 =====================
// 步骤1: maxSubseq — 单调栈从数组中选t个数组成最大子序列
// 步骤2: 枚举 i (0..k)，nums1取i个，nums2取k-i个
// 步骤3: merge — 字典序归并两个子序列
// 时间 O(k*(m+n)), 空间 O(m+n)
class Solution1 {
public:
    vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
        int m = nums1.size(), n = nums2.size();
        vector<int> best;

        for (int i = max(0, k - n); i <= min(k, m); i++) {
            vector<int> sub1 = maxSubseq(nums1, i);
            vector<int> sub2 = maxSubseq(nums2, k - i);
            vector<int> merged = merge(sub1, sub2);
            if (merged > best) best = merged;
        }

        return best;
    }

private:
    // 单调栈：从 nums 中选 t 个数，保持顺序，使子序列最大
    vector<int> maxSubseq(vector<int>& nums, int t) {
        int drop = nums.size() - t;  // 可丢弃的元素数
        vector<int> stk;
        for (int num : nums) {
            // 栈顶较小且还有丢弃配额 => 弹出
            while (drop > 0 && !stk.empty() && stk.back() < num) {
                stk.pop_back();
                drop--;
            }
            stk.push_back(num);
        }
        stk.resize(t);  // 裁剪多余元素
        return stk;
    }

    // 字典序归并：每步取剩余部分字典序较大的
    vector<int> merge(vector<int>& a, vector<int>& b) {
        vector<int> result;
        int i = 0, j = 0;
        while (i < (int)a.size() || j < (int)b.size()) {
            if (greater(a, i, b, j)) {
                result.push_back(a[i++]);
            } else {
                result.push_back(b[j++]);
            }
        }
        return result;
    }

    // 判断 a[i:] 字典序 >= b[j:]
    bool greater(vector<int>& a, int i, vector<int>& b, int j) {
        while (i < (int)a.size() && j < (int)b.size() && a[i] == b[j]) {
            i++; j++;
        }
        if (j >= (int)b.size()) return true;   // b先耗尽，a更大
        if (i >= (int)a.size()) return false;   // a先耗尽，b更大
        return a[i] > b[j];
    }
};

// ===================== Solution2: 利用 vector 比较简化 =====================
// 思路同上，merge 中直接用 vector 切片构造子向量比较
// 代码更简洁但性能稍差（每次比较构造临时向量）
class Solution2 {
public:
    vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
        int m = nums1.size(), n = nums2.size();
        vector<int> best;

        for (int i = max(0, k - n); i <= min(k, m); i++) {
            auto sub1 = maxSubseq(nums1, i);
            auto sub2 = maxSubseq(nums2, k - i);
            auto merged = merge(sub1, sub2);
            best = max(best, merged);
        }
        return best;
    }

private:
    vector<int> maxSubseq(vector<int>& nums, int t) {
        int drop = nums.size() - t;
        vector<int> stk;
        for (int x : nums) {
            while (drop && !stk.empty() && stk.back() < x) {
                stk.pop_back();
                drop--;
            }
            stk.push_back(x);
        }
        stk.resize(t);
        return stk;
    }

    vector<int> merge(vector<int>& a, vector<int>& b) {
        vector<int> res;
        // 用迭代器构造子向量进行字典序比较
        auto ia = a.begin(), ib = b.begin();
        while (ia != a.end() || ib != b.end()) {
            // 直接用 vector 的 > 比较（字典序）
            if (vector<int>(ia, a.end()) >= vector<int>(ib, b.end())) {
                res.push_back(*ia++);
            } else {
                res.push_back(*ib++);
            }
        }
        return res;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | Solution1 手写compare | O(k*(m+n)) | O(k) | 性能最优 |
 * | Solution2 vector比较 | O(k^2*(m+n)) | O(k) | 代码简洁 |
 *
 * 【核心子问题分析】
 * 1. maxSubseq: 单调栈，drop=n-t 控制可弹出次数
 *    关键：弹出条件 stk.back() < num && drop > 0
 * 2. merge: 字典序归并（非数值比较）
 *    关键：相等时必须比较后续，不能随意取
 * 3. 枚举：i 的合法范围 [max(0,k-n), min(k,m)]
 *
 * 【易错点】
 * 1. merge 只比较当前元素会出错：[6,7] vs [6,0,4] 中两个6相等时
 *    必须看后续 7>0，所以先取第一组的6
 * 2. maxSubseq 结尾必须 resize(t)：可能多压入了元素
 * 3. 枚举范围 i: k-n 可能为负，用 max(0, k-n)
 * 4. stk.back() < num 用严格小于（相等时不弹出）
 *
 * 【面试追问】
 * Q1: 单数组版本（LC 402 移掉K位数字）怎么做？
 *     同样的单调栈，drop=k 表示可移除的数字个数
 * Q2: 合并时为什么不能只看当前元素？
 *     [6,7]+[6,0,4] 应出 6,7,6,0,4 而非 6,6,7,0,4
 * Q3: 能否用DP解？
 *     可以但复杂度更高 O(m*n*k)，不如贪心+单调栈
 */
