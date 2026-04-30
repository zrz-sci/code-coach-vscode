// ============================================================
// 725. Split Linked List in Parts
// ============================================================
// 标签: Linked List
// 难度: Medium
//
// 核心思路:
//   1. 遍历链表获取总长度 n
//   2. partSize = n/k, extra = n%k
//   3. 前 extra 段长 partSize+1, 其余长 partSize
//   4. 逐段切割: 走到段尾 → 断开 → 移到下一段
// ============================================================


// ============================================================
// 解法1: 计算长度 + 逐段切割 (推荐, 面试标准解)
//
// 数学:
//   partSize = n / k (每段基础长度)
//   extra = n % k    (多出的节点数)
//   前 extra 段: partSize + 1
//   后 k-extra 段: partSize
//
// 示例: n=10, k=3
//   partSize=3, extra=1
//   段长度: [4, 3, 3]
//
// 示例: n=2, k=5
//   partSize=0, extra=2
//   段长度: [1, 1, 0, 0, 0]
//
// 时间: O(n + k)
// 空间: O(k) 结果数组
// ============================================================
class Solution {
public:
    vector<ListNode*> splitListToParts(ListNode* head, int k) {
        // Step 1: 计算链表总长度
        int n = 0;
        ListNode* curr = head;
        while (curr) {
            n++;
            curr = curr->next;
        }

        // Step 2: 计算每段长度
        int partSize = n / k;    // 每段基础长度
        int extra = n % k;       // 前 extra 段多一个节点

        // Step 3: 逐段切割
        vector<ListNode*> result(k, nullptr);
        curr = head;

        for (int i = 0; i < k && curr; i++) {
            result[i] = curr;  // 记录当前段的头节点

            // 当前段的实际长度
            int curLen = partSize + (i < extra ? 1 : 0);

            // 走到当前段的最后一个节点 (走 curLen-1 步)
            for (int j = 1; j < curLen; j++) {
                curr = curr->next;
            }

            // 断开: 保存下一段的头, 然后切断
            ListNode* nextPart = curr->next;
            curr->next = nullptr;
            curr = nextPart;
        }

        return result;
    }
};


// ============================================================
// 解法2: 转数组再分割 (更直观但空间 O(n))
//
// 将链表所有值存入数组
// 按段长度分割数组, 每段重建链表
//
// 优点: 逻辑清晰, 不涉及指针断开操作
// 缺点: O(n) 额外空间, 重建链表增加开销
//
// 时间: O(n + k)
// 空间: O(n)
// ============================================================
class Solution_Array {
public:
    vector<ListNode*> splitListToParts(ListNode* head, int k) {
        // Step 1: 链表转数组
        vector<int> vals;
        ListNode* curr = head;
        while (curr) {
            vals.push_back(curr->val);
            curr = curr->next;
        }

        int n = vals.size();
        int partSize = n / k;
        int extra = n % k;

        // Step 2: 按段重建链表
        vector<ListNode*> result(k, nullptr);
        int idx = 0;

        for (int i = 0; i < k; i++) {
            int curLen = partSize + (i < extra ? 1 : 0);
            if (curLen == 0) continue;

            // 建立当前段的链表
            ListNode dummy(0);
            ListNode* tail = &dummy;
            for (int j = 0; j < curLen; j++) {
                tail->next = new ListNode(vals[idx++]);
                tail = tail->next;
            }
            result[i] = dummy.next;
        }

        return result;
    }
};


// ============================================================
// 解法3: 递归分割 (展示递归思维)
//
// 递归思路:
//   每次从链表头部切下一段 (长度 partSize 或 partSize+1)
//   递归处理剩余链表
//
// 递归参数: 当前 head, 剩余段数 k, 剩余节点数 n
// 基础情况: k == 0 或 head == nullptr
//
// 时间: O(n + k) — 递归深度 k
// 空间: O(k) 递归栈 + O(k) 结果数组
// ============================================================
class Solution_Recursive {
public:
    vector<ListNode*> splitListToParts(ListNode* head, int k) {
        // 计算总长度
        int n = 0;
        ListNode* curr = head;
        while (curr) { n++; curr = curr->next; }

        vector<ListNode*> result(k, nullptr);
        splitHelper(head, k, n, 0, result);
        return result;
    }

private:
    void splitHelper(ListNode* head, int k, int remaining,
                     int idx, vector<ListNode*>& result) {
        if (idx >= k || !head) return;

        int partSize = remaining / (k - idx);
        int extra = remaining % (k - idx);
        int curLen = partSize + (extra > 0 ? 1 : 0);

        result[idx] = head;
        ListNode* curr = head;

        for (int j = 1; j < curLen; j++) {
            curr = curr->next;
        }

        ListNode* nextPart = curr->next;
        curr->next = nullptr;

        splitHelper(nextPart, k, remaining - curLen, idx + 1, result);
    }
};


// ============================================================
// 总结
// ============================================================
/*
 * 【解法对比】
 *
 * | 解法              | 时间    | 空间    | 推荐度       |
 * |-------------------|---------|---------|--------------|
 * | 逐段切割 (推荐)   | O(n+k)  | O(k)   | 面试首选     |
 * | 转数组分割        | O(n+k)  | O(n)   | 简单但浪费   |
 * | 递归分割          | O(n+k)  | O(k)   | 展示递归     |
 *
 * 【关键公式】
 * partSize = n / k
 * extra = n % k
 * 第 i 段长度 = partSize + (i < extra ? 1 : 0)
 *
 * 【易错点】
 * 1. 走步数: curLen-1 步到达段尾, 不是 curLen 步
 * 2. 断开操作: 必须 curr->next = nullptr, 否则段之间仍然相连
 * 3. result 初始化: 全部填 nullptr, 处理 n < k 的情况
 * 4. extra 分配方向: 前 extra 段多一个, 不是后 extra 段
 * 5. 空链表: head 为 nullptr 时所有段都是空
 * 6. 内层循环 j 从 1 开始: 因为 curr 已经在段头上
 *
 * 【面试追问】
 * Q: 能否不计算长度直接切割?
 * A: 很难。不知道 n 就无法确定 partSize 和 extra。
 *    可以用快慢指针找中点后递归, 但更复杂。
 *
 * Q: k 远大于 n 时的性能?
 * A: O(n+k), 主要开销在初始化 k 个 nullptr。
 *    如果 k 极大, 空间 O(k) 可能是瓶颈。
 *
 * Q: 如何验证分割正确性?
 * A: 检查: (1) 段长度之和=n (2) 任意两段差<=1
 *         (3) 段长度单调不增
 */
