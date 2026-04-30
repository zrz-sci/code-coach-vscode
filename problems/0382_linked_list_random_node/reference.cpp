/*
 * 382. Linked List Random Node
 * 难度: Medium
 * 标签: Reservoir Sampling, Linked List, Math, Randomized
 *
 * 【题目本质】
 * 从单链表中等概率随机选取一个节点值，链表长度可能未知。
 *
 * 【解法总览】
 * 解法1: 蓄水池抽样 - O(1)空间，每次遍历O(n)
 * 解法2: 数组缓存 - O(n)空间，查询O(1)
 */

// ===================== Solution 1: 蓄水池抽样 =====================
// 思路：遍历链表，第i个节点以1/i概率替换当前选中值
// 时间：init O(1), getRandom O(n)
// 空间：O(1)
class Solution1 {
    ListNode* head;
public:
    Solution1(ListNode* head) : head(head) {}

    int getRandom() {
        int result = head->val;
        ListNode* cur = head->next;
        int i = 2;  // 从第2个节点开始考虑替换
        while (cur) {
            // 以 1/i 的概率选择当前节点
            if (rand() % i == 0) {
                result = cur->val;
            }
            i++;
            cur = cur->next;
        }
        return result;
    }
};

// ===================== Solution 2: 数组缓存 =====================
// 思路：初始化时将链表值存入数组，随机下标访问
// 时间：init O(n), getRandom O(1)
// 空间：O(n)
class Solution2 {
    vector<int> vals;
public:
    Solution2(ListNode* head) {
        while (head) {
            vals.push_back(head->val);
            head = head->next;
        }
    }

    int getRandom() {
        return vals[rand() % vals.size()];
    }
};

/*
 * 【解法对比】
 * | 解法 | 初始化 | 查询 | 空间 | 适用场景 |
 * |------|--------|------|------|----------|
 * | 蓄水池抽样 | O(1) | O(n) | O(1) | 链表极大或长度未知 |
 * | 数组缓存 | O(n) | O(1) | O(n) | 查询频繁、链表不变 |
 *
 * 【易错点】
 * 1. 蓄水池抽样 i 必须从 2 开始（第一个元素默认选中）
 * 2. rand() % i 中 i 是当前遍历到的节点序号，不是固定的 n
 * 3. 每次 getRandom 都要从 head 重新遍历
 *
 * 【面试追问】
 * Q1: 推广到选 k 个 → Reservoir Sampling size-k
 * Q2: 证明等概率性 → 数学归纳法：P(第k个被保留) = 1/n
 * Q3: 如何用更好的随机数生成器 → mt19937 + uniform_int_distribution
 */
