// ========================================
// 652. Find Duplicate Subtrees
// ========================================
// 解法一：字符串序列化 + 哈希表
// 时间 O(n^2)  空间 O(n^2)
// ========================================
class Solution {
public:
    vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
        vector<TreeNode*> result;
        unordered_map<string, int> count;
        serialize(root, count, result);
        return result;
    }

private:
    // 后序遍历序列化：左子树 + 右子树 + 当前节点
    // 返回以 node 为根的子树的序列化字符串
    string serialize(TreeNode* node,
                     unordered_map<string, int>& count,
                     vector<TreeNode*>& result) {
        if (!node) return "#";

        // 递归序列化左右子树
        string left = serialize(node->left, count, result);
        string right = serialize(node->right, count, result);

        // 构建当前子树的序列化（用逗号分隔防歧义）
        string key = left + "," + right + "," + to_string(node->val);

        // 计数，恰好第二次出现时加入结果
        count[key]++;
        if (count[key] == 2) {
            result.push_back(node);
        }

        return key;
    }
};

// ========================================
// 解法二：三元组 ID 映射（最优解）
// 时间 O(n)  空间 O(n)
// ========================================
// 核心思想：用整数 ID 唯一标识每种子树结构，避免字符串拼接
// 空节点 ID = 0，每种新的 (val, leftId, rightId) 分配一个新 ID
class Solution2 {
public:
    vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
        vector<TreeNode*> result;
        // tripleToId: 三元组 -> 子树唯一 ID
        map<tuple<int,int,int>, int> tripleToId;
        // idCount: 每个子树 ID 出现了多少次
        unordered_map<int, int> idCount;
        int nextId = 1; // 0 留给空节点

        getId(root, tripleToId, idCount, nextId, result);
        return result;
    }

private:
    int getId(TreeNode* node,
              map<tuple<int,int,int>, int>& tripleToId,
              unordered_map<int, int>& idCount,
              int& nextId,
              vector<TreeNode*>& result) {
        if (!node) return 0;

        int leftId = getId(node->left, tripleToId, idCount, nextId, result);
        int rightId = getId(node->right, tripleToId, idCount, nextId, result);

        auto key = make_tuple(node->val, leftId, rightId);

        // 如果这种三元组还没见过，分配新 ID
        if (tripleToId.find(key) == tripleToId.end()) {
            tripleToId[key] = nextId++;
        }

        int id = tripleToId[key];
        idCount[id]++;

        // 恰好第二次出现时加入结果
        if (idCount[id] == 2) {
            result.push_back(node);
        }

        return id;
    }
};

// ========================================
// 解法三：带自定义编码的三元组方案
// 时间 O(n)  空间 O(n)
// ========================================
// 使用 unordered_map 代替 map，通过数值编码加速查找
class Solution3 {
public:
    vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
        vector<TreeNode*> result;
        unordered_map<long long, int> tripleToId;
        unordered_map<int, int> idCount;
        int nextId = 1;

        getId(root, tripleToId, idCount, nextId, result);
        return result;
    }

private:
    // 将三元组编码为 long long，利用值域有限的特点
    // val 范围 [-200, 200]，ID 不超过 n=5000
    long long encode(int val, int leftId, int rightId) {
        // val+200 -> [0, 400]
        // leftId, rightId 各用 16 bit（最多 5000）
        return ((long long)(val + 200) << 32) |
               ((long long)leftId << 16) |
               (long long)rightId;
    }

    int getId(TreeNode* node,
              unordered_map<long long, int>& tripleToId,
              unordered_map<int, int>& idCount,
              int& nextId,
              vector<TreeNode*>& result) {
        if (!node) return 0;

        int leftId = getId(node->left, tripleToId, idCount, nextId, result);
        int rightId = getId(node->right, tripleToId, idCount, nextId, result);

        long long key = encode(node->val, leftId, rightId);

        auto it = tripleToId.find(key);
        if (it == tripleToId.end()) {
            it = tripleToId.emplace(key, nextId++).first;
        }

        int id = it->second;
        idCount[id]++;

        if (idCount[id] == 2) {
            result.push_back(node);
        }

        return id;
    }
};
