// LeetCode 677: Map Sum Pairs - Reference Solution
// 难度: Medium
// 标签: Design, Trie, Hash Table, String

// ============================================================
// 解法一: Trie + HashMap + delta 更新（推荐面试解法）
// ============================================================
// 核心思路:
//   - HashMap 记录每个 key 的当前 value（用于处理覆盖）
//   - Trie 每个节点存储经过该前缀的 value 之和
//   - insert 时计算 delta = newVal - oldVal，沿路径更新
//   - sum 时找到 prefix 末尾节点，直接返回累计和
//
// 时间复杂度: insert O(L), sum O(L), L 为 key/prefix 长度
// 空间复杂度: O(所有 key 的总长度 * 26)

class MapSum {
    struct TrieNode {
        TrieNode* children[26] = {};
        int prefixSum = 0;  // 所有经过该节点的 key 的 value 总和
    };

    TrieNode* root;
    unordered_map<string, int> keyMap;  // 记录已有 key 的 value

public:
    MapSum() {
        root = new TrieNode();
    }

    void insert(string key, int val) {
        // 步骤1: 计算 delta（处理覆盖）
        int delta = val;
        if (keyMap.count(key)) {
            delta = val - keyMap[key];
        }
        keyMap[key] = val;

        // 步骤2: 沿 Trie 路径更新每个节点的 prefixSum
        TrieNode* node = root;
        for (char c : key) {
            int idx = c - 'a';
            if (!node->children[idx]) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
            node->prefixSum += delta;
        }
    }

    int sum(string prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!node->children[idx]) return 0;  // 前缀不存在
            node = node->children[idx];
        }
        return node->prefixSum;
    }
};

// ============================================================
// 解法二: HashMap + 暴力遍历（最简实现）
// ============================================================
// 核心思路:
//   - 用 HashMap 存所有键值对
//   - sum 时遍历所有 entry，检查前缀匹配
//
// 时间复杂度: insert O(1), sum O(N*L)
// 空间复杂度: O(N*L)

class MapSum2 {
    unordered_map<string, int> mp;

public:
    MapSum2() {}

    void insert(string key, int val) {
        mp[key] = val;  // 直接覆盖
    }

    int sum(string prefix) {
        int total = 0;
        for (auto& [key, val] : mp) {
            // 检查 key 是否以 prefix 开头
            if (key.size() >= prefix.size() &&
                key.compare(0, prefix.size(), prefix) == 0) {
                total += val;
            }
        }
        return total;
    }
};

// ============================================================
// 解法三: Trie + DFS 子树求和（不需要 delta）
// ============================================================
// 核心思路:
//   - insert 时在叶子节点存 value（覆盖时直接替换）
//   - sum 时找到 prefix 末尾节点，DFS 遍历整棵子树求和
//   - 优点: insert 不需要计算 delta，逻辑更简单
//   - 缺点: sum 时间取决于子树大小，不稳定
//
// 时间复杂度: insert O(L), sum O(子树节点数)
// 空间复杂度: O(所有 key 的总长度 * 26)

class MapSum3 {
    struct TrieNode {
        TrieNode* children[26] = {};
        int val = 0;       // 如果该节点是某个 key 的末尾，存其 value
        bool isEnd = false;
    };

    TrieNode* root;

public:
    MapSum3() {
        root = new TrieNode();
    }

    void insert(string key, int val) {
        TrieNode* node = root;
        for (char c : key) {
            int idx = c - 'a';
            if (!node->children[idx]) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
        }
        node->isEnd = true;
        node->val = val;  // 直接覆盖，无需 delta
    }

    int sum(string prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!node->children[idx]) return 0;
            node = node->children[idx];
        }
        return dfsSum(node);
    }

private:
    // DFS 遍历子树，累加所有叶子节点的 value
    int dfsSum(TrieNode* node) {
        if (!node) return 0;
        int total = 0;
        if (node->isEnd) total += node->val;
        for (int i = 0; i < 26; i++) {
            total += dfsSum(node->children[i]);
        }
        return total;
    }
};
