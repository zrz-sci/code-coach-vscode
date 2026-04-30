// 703. Kth Largest Element in a Stream
// 难度: Easy
// 标签: Tree, Design, Binary Search Tree, Heap (Priority Queue), Data Stream
//
// 思路: 维护大小为 K 的最小堆，堆顶即为第 K 大元素
// 初始化时将所有元素加入堆，超过 K 个时弹出堆顶
// add 时 push 新元素，超过 K 个时弹出堆顶，返回堆顶
//
// 时间复杂度: 初始化 O(n log K), add O(log K)
// 空间复杂度: O(K)

// ============================================================
// 解法一: 最小堆（推荐面试解法）
// ============================================================
class KthLargest {
private:
    int k_;
    // 最小堆: 堆顶是最小值
    // C++ priority_queue 默认是最大堆，加 greater<int> 变最小堆
    priority_queue<int, vector<int>, greater<int>> minHeap_;

public:
    KthLargest(int k, vector<int>& nums) : k_(k) {
        // 将所有元素加入最小堆，但只保留最大的 K 个
        for (int num : nums) {
            minHeap_.push(num);
            if ((int)minHeap_.size() > k_) {
                minHeap_.pop();  // 弹出堆顶(最小值)，保证堆内是最大的 K 个
            }
        }
    }

    int add(int val) {
        minHeap_.push(val);
        if ((int)minHeap_.size() > k_) {
            minHeap_.pop();  // 弹出最小的，保持堆大小为 K
        }
        return minHeap_.top();  // 堆顶 = 第 K 大
    }
};

// ============================================================
// 解法二: 最小堆 + 提前剪枝优化
// ============================================================
// 当堆已满 K 个且新元素 <= 堆顶时，无需 push 再 pop
class KthLargest_V2 {
private:
    int k_;
    priority_queue<int, vector<int>, greater<int>> minHeap_;

public:
    KthLargest_V2(int k, vector<int>& nums) : k_(k) {
        for (int num : nums) {
            addToHeap(num);
        }
    }

    int add(int val) {
        addToHeap(val);
        return minHeap_.top();
    }

private:
    void addToHeap(int val) {
        if ((int)minHeap_.size() < k_) {
            // 堆还没满，直接加入
            minHeap_.push(val);
        } else if (val > minHeap_.top()) {
            // 新元素比堆顶大，替换堆顶
            minHeap_.pop();
            minHeap_.push(val);
        }
        // else: val <= 堆顶，不在前 K 大中，直接忽略
    }
};

// ============================================================
// 解法三: 有序数组（简单但效率低）
// ============================================================
// 用 sorted vector 维护所有元素，add 时二分插入
class KthLargest_V3 {
private:
    int k_;
    vector<int> sorted_;

public:
    KthLargest_V3(int k, vector<int>& nums) : k_(k), sorted_(nums) {
        sort(sorted_.begin(), sorted_.end());
    }

    int add(int val) {
        // 二分查找插入位置
        auto it = lower_bound(sorted_.begin(), sorted_.end(), val);
        sorted_.insert(it, val);

        // 倒数第 K 个就是第 K 大
        return sorted_[sorted_.size() - k_];
    }
};

// ============================================================
// 解法四: multiset（平衡 BST）
// ============================================================
// 使用 multiset 自动排序，支持重复元素
// 保持 multiset 大小为 K，begin() 就是第 K 大
class KthLargest_V4 {
private:
    int k_;
    multiset<int> mset_;

public:
    KthLargest_V4(int k, vector<int>& nums) : k_(k) {
        for (int num : nums) {
            mset_.insert(num);
            if ((int)mset_.size() > k_) {
                mset_.erase(mset_.begin());  // 删除最小的
            }
        }
    }

    int add(int val) {
        mset_.insert(val);
        if ((int)mset_.size() > k_) {
            mset_.erase(mset_.begin());
        }
        return *mset_.begin();  // 最小值 = 第 K 大
    }
};

// ============================================================
// 关键点总结:
// 1. 最小堆大小固定为 K，堆顶就是第 K 大元素
// 2. 解法二的剪枝: val <= top 时无需操作，减少常数开销
// 3. 解法三的 sorted vector 虽简单但插入 O(n)，适合面试备选
// 4. 解法四的 multiset 支持 O(log n) 插入删除，但常数大于堆
// 5. 面试首选解法一或解法二，代码简洁且时间最优
// ============================================================
