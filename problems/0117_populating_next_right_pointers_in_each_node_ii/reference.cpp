/*
 * 【题目本质】
 * 对任意二叉树，将每层节点用 next 指针从左到右串联起来。
 * 与 LC 116 的区别：树不一定是完美二叉树，子节点可能缺失。
 *
 * 【解法总览】
 * 1. BFS层序遍历 - 队列逐层连接         O(n) / O(n)
 * 2. 利用next指针+dummy - O(1)空间    O(n) / O(1)
 *
 * 树结构示意：
 *
 *   输入:          连接后:
 *       1              1 -> NULL
 *      / \            / \
 *     2   3          2 -> 3 -> NULL
 *    / \   \        / \    \
 *   4   5   7      4-> 5 -> 7 -> NULL
 *
 *   关键：节点3没有左子节点，5的next要跳过"空位"连到7
 *
 *   利用上层已建好的 next 指针遍历：
 *   curr层: [2] -> [3] -> NULL
 *   对2: 连接 4 -> 5
 *   对3: 连接 -> 7
 *   下层结果: 4 -> 5 -> 7 -> NULL
 */

// ===================== 解法1：BFS 层序遍历 =====================
// 思路：用队列做标准 BFS，每层节点依次连接 next。
// 为什么简单？因为队列自然按层分隔，同层节点顺序取出就是从左到右。
// 缺点：队列最多存一层节点，空间 O(n)。
class Solution1 {
public:
    Node* connect(Node* root) {
        if (!root) return nullptr;

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            int size = q.size();
            Node* prev = nullptr;

            for (int i = 0; i < size; i++) {
                Node* curr = q.front();
                q.pop();

                // 将上一个节点的 next 指向当前节点
                if (prev) prev->next = curr;
                prev = curr;

                if (curr->left)  q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
            // prev(最后一个节点) 的 next 保持为 NULL（默认值）
        }

        return root;
    }
};

// ===================== 解法2：利用 next 指针 + dummy 节点 =====================
// 思路：处理第 k+1 层时，第 k 层的 next 已经建好了。
// 所以可以用 next 指针"横向遍历"第 k 层，同时把第 k+1 层的子节点串成链表。
// 为什么用 dummy？因为下一层的第一个子节点不确定是哪个（可能某些节点没有子节点），
// dummy 避免了判断"下一层第一个节点是谁"的复杂边界逻辑。
class Solution2 {
public:
    Node* connect(Node* root) {
        Node* curr = root;

        while (curr) {
            // 每层创建新的 dummy，作为下一层链表的虚拟头
            Node dummy(0);
            Node* tail = &dummy;

            // 利用 next 指针遍历当前层
            for (Node* p = curr; p; p = p->next) {
                // 先左后右，保证顺序
                if (p->left) {
                    tail->next = p->left;
                    tail = tail->next;
                }
                if (p->right) {
                    tail->next = p->right;
                    tail = tail->next;
                }
            }

            // 下一层的起点
            curr = dummy.next;
        }

        return root;
    }
};

/*
 * 【解法对比】
 * | 解法         | 时间  | 空间  | 关键技巧           |
 * |-------------|------|------|-------------------|
 * | BFS队列      | O(n) | O(n) | 层序遍历           |
 * | next+dummy   | O(n) | O(1) | 利用已建好的next指针 |
 *
 * 【易错点】
 * 1. dummy 必须在每层循环内部创建，否则 dummy.next 残留上一层的值
 * 2. 子节点处理顺序必须先左后右，否则 next 链接方向反了
 * 3. 不要忘记处理某些节点只有右子没有左子的情况
 *    （for 循环中两个独立 if，不是 if-else，所以天然处理了）
 * 4. 与 LC 116 不同：不能假设 parent->next->left 存在
 *
 * 【面试追问】
 * Q1: 为什么 O(1) 空间方案可行？
 *     → 因为上一层的 next 已经连好了，可以当作"链表"遍历，
 *       不需要队列来记住同层节点
 * Q2: LC 116 的 O(1) 方案能直接用在这题吗？
 *     → 不能。116 利用了完美二叉树的性质（每个节点必有两个子节点），
 *       本题必须用更通用的 dummy 链表法
 * Q3: 如果要求从右往左连接 next（每个节点指向左邻居），怎么改？
 *     → 遍历子节点时改为先右后左即可
 */
