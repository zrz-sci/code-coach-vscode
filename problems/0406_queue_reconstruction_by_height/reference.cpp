/*
 * LeetCode 406: 根据身高重建队列 (Queue Reconstruction by Height)
 *
 * 【题目本质】
 * 给定每个人的 [身高h, 前面≥h的人数k]，重建原始队列。
 * 本质是一个排序+贪心问题：精心设计处理顺序，让每次放置都能直接确定位置。
 *
 * 【解法总览】
 * 解法1: 高→矮贪心插入 — O(n²) / O(n) — 面试首选，代码最短
 * 解法2: 矮→高贪心占位 — O(n²) / O(n) — 另一种思考方向
 * 解法3: 树状数组优化  — O(n log²n) / O(n) — 追问优化时给出
 */

// ============================================================
// 解法1: 高→矮贪心插入
// 时间: O(n²)  空间: O(n)
//
// 【思路】
// 核心洞察：矮个子对高个子是"透明"的。
// 身高7的人只关心前面有几个≥7的人，身高4的人不影响他。
// 所以先安排高个子，他们的位置由 k 直接决定。
// 后续插入的矮个子虽然会挤开元素位置，但不会改变
// 高个子前面"≥自己身高的人数"（因为矮个子不被计入）。
//
// 排序规则：
//   - 身高降序（先处理高的）
//   - 同身高时 k 升序（k小的先插，占据靠前位置）
//
// 可视化过程:
//   排序后: [7,0] [7,1] [6,1] [5,0] [5,2] [4,4]
//
//   Step1: insert [7,0] at pos 0
//     → [ [7,0] ]
//
//   Step2: insert [7,1] at pos 1
//     → [ [7,0], [7,1] ]
//
//   Step3: insert [6,1] at pos 1   ← 挤开[7,1]往后移
//     → [ [7,0], [6,1], [7,1] ]
//
//   Step4: insert [5,0] at pos 0   ← 挤开所有人往后移
//     → [ [5,0], [7,0], [6,1], [7,1] ]
//
//   Step5: insert [5,2] at pos 2
//     → [ [5,0], [7,0], [5,2], [6,1], [7,1] ]
//
//   Step6: insert [4,4] at pos 4
//     → [ [5,0], [7,0], [5,2], [6,1], [4,4], [7,1] ]  ✓
//
//   验证 [5,0]: 前面0个≥5的人 ✓
//   验证 [7,0]: 前面0个≥7的人 ✓ (5<7不算)
//   验证 [5,2]: 前面2个≥5的人: [5,0]和[7,0] ✓
//   验证 [6,1]: 前面1个≥6的人: [7,0] ✓ (5<6不算)
//   验证 [4,4]: 前面4个≥4的人: [5,0],[7,0],[5,2],[6,1] ✓
//   验证 [7,1]: 前面1个≥7的人: [7,0] ✓
// ============================================================
class Solution1 {
public:
    vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
        // 身高降序；同身高 k 升序
        // 为什么同身高 k 升序？因为同身高互相"可见"(≥不是>)，
        // k 小的先占位，k 大的后插到更后面的位置
        sort(people.begin(), people.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0] > b[0] || (a[0] == b[0] && a[1] < b[1]);
        });
        
        vector<vector<int>> result;
        for (auto& p : people) {
            // 此时 result 中所有人身高 >= p[0]
            // 所以插入位置 p[1] 就是 "前面有 p[1] 个 >= h 的人"
            result.insert(result.begin() + p[1], p);
        }
        return result;
    }
};


// ============================================================
// 解法2: 矮→高贪心占位
// 时间: O(n²)  空间: O(n)
//
// 【思路】
// 换个方向思考：先安排矮个子。
// 对于 [h, k]，后续来的人身高都 >= h，他们会填入空位。
// 所以当前人需要前面留 k 个空位（给后来的高个子），
// 自己占据第 (k+1) 个空位。
//
// 排序规则：
//   - 身高升序（先处理矮的）
//   - 同身高时 k 降序（k大的先放到后面的空位，不影响k小的）
//
// 可视化过程 (用 _ 表示空位):
//   排序后: [4,4] [5,2] [5,0] [6,1] [7,1] [7,0]
//   初始:   [ _,  _,  _,  _,  _,  _ ]
//            0   1   2   3   4   5
//
//   Step1: [4,4] 找第5个空位 → pos=4
//     → [ _,  _,  _,  _, [4,4], _ ]
//
//   Step2: [5,2] 找第3个空位 → pos=2
//     → [ _,  _, [5,2], _, [4,4], _ ]
//
//   Step3: [5,0] 找第1个空位 → pos=0
//     → [[5,0], _, [5,2], _, [4,4], _ ]
//
//   Step4: [6,1] 找第2个空位(空位在pos 1,3,5) → pos=3
//     → [[5,0], _, [5,2], [6,1], [4,4], _ ]
//
//   Step5: [7,1] 找第2个空位(空位在pos 1,5) → pos=5
//     → [[5,0], _, [5,2], [6,1], [4,4], [7,1]]
//
//   Step6: [7,0] 找第1个空位(空位在pos 1) → pos=1
//     → [[5,0], [7,0], [5,2], [6,1], [4,4], [7,1]]  ✓
// ============================================================
class Solution2 {
public:
    vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
        // 身高升序；同身高 k 降序
        // 为什么同身高 k 降序？同身高互相可见，
        // k 大的先放到后面的空位，不会占用 k 小的所需空位
        sort(people.begin(), people.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0] < b[0] || (a[0] == b[0] && a[1] > b[1]);
        });
        
        int n = people.size();
        vector<vector<int>> result(n);
        
        for (auto& p : people) {
            int spaces = 0;
            for (int i = 0; i < n; i++) {
                if (result[i].empty()) {
                    spaces++;
                    // 找到第 (k+1) 个空位
                    // 前面的 k 个空位会被后续更高的人填充
                    if (spaces == p[1] + 1) {
                        result[i] = p;
                        break;
                    }
                }
            }
        }
        return result;
    }
};


// ============================================================
// 解法3: 树状数组 + 二分查找优化
// 时间: O(n log²n)  空间: O(n)
//
// 【思路】
// 解法2的瓶颈：每次线性扫描找第(k+1)个空位是 O(n)。
// 优化：用树状数组维护"空位前缀和"。
// tree[i] 的前缀和 = 位置 0~i 中有多少个空位。
// 要找"前缀和 >= k+1 的最小位置"，用二分搜索实现。
//
// 每次放置一个人后，将该位置的空位标记清除（update -1）。
//
// 二分查找空位过程示例:
//   n=6, 初始所有位置为空
//   prefix_sum = [1, 2, 3, 4, 5, 6]
//
//   找第5个空位: 二分 → prefix_sum[4]=5 ≥ 5, prefix_sum[3]=4 < 5
//   → 第5个空位在位置4
//
//   update(4, -1) 后:
//   prefix_sum = [1, 2, 3, 4, 4, 5]
//
//   找第3个空位: 二分 → prefix_sum[2]=3 ≥ 3, prefix_sum[1]=2 < 3
//   → 第3个空位在位置2
// ============================================================
class Solution3 {
public:
    vector<int> tree;
    int n;
    
    // 树状数组：单点更新
    void update(int i, int val) {
        for (i++; i <= n; i += i & (-i))  // +1 转为 1-indexed
            tree[i] += val;
    }
    
    // 树状数组：前缀查询 [0, i]
    int query(int i) {
        int sum = 0;
        for (i++; i > 0; i -= i & (-i))
            sum += tree[i];
        return sum;
    }
    
    // 二分查找：前缀和 >= target 的最小下标
    int findKthEmpty(int target) {
        int lo = 0, hi = n - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            // query(mid) = 位置 0~mid 中空位数量
            if (query(mid) >= target)
                hi = mid;       // mid 可能是答案，继续向左找
            else
                lo = mid + 1;   // 空位不够，必须往右
        }
        return lo;
    }
    
    vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
        n = people.size();
        tree.assign(n + 1, 0);
        
        // 初始化：所有位置为空位（值为1）
        for (int i = 0; i < n; i++)
            update(i, 1);
        
        // 排序：身高升序，同身高 k 降序（与解法2相同）
        sort(people.begin(), people.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0] < b[0] || (a[0] == b[0] && a[1] > b[1]);
        });
        
        vector<vector<int>> result(n);
        for (auto& p : people) {
            // 找第 (k+1) 个空位
            int pos = findKthEmpty(p[1] + 1);
            result[pos] = p;
            // 该位置不再是空位
            update(pos, -1);
        }
        return result;
    }
};


// ============================================================
// 解法4: 树状数组 — 树上二分 (真正的 O(n log n))
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 解法3用了"树状数组查询 + 外层二分"，复杂度 O(n log²n)。
// 可以直接在树状数组的树结构上做二分（类似线段树上二分），
// 从最高位向下逐层确定位置，一次 O(log n) 找到第 k 个空位。
// ============================================================
class Solution4 {
public:
    vector<int> tree;
    int n;
    
    void update(int i, int val) {
        for (i++; i <= n; i += i & (-i))
            tree[i] += val;
    }
    
    // 树上二分：在 O(log n) 内找到前缀和 >= k 的最小位置
    int findKth(int k) {
        int pos = 0;
        // 从最高位向下逐层决策
        // LOG = 树状数组的层数
        int LOG = 0;
        while ((1 << (LOG + 1)) <= n) LOG++;
        
        for (int pw = (1 << LOG); pw > 0; pw >>= 1) {
            // 尝试跳到 pos + pw
            if (pos + pw <= n && tree[pos + pw] < k) {
                // 这一段的空位数不够，跳过去
                k -= tree[pos + pw];
                pos += pw;
            }
        }
        // pos 是 0-indexed 的答案（因为 tree 是 1-indexed，pos 需要返回原始下标）
        return pos;  // 返回的是 0-indexed 位置
    }
    
    vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
        n = people.size();
        tree.assign(n + 1, 0);
        
        for (int i = 0; i < n; i++)
            update(i, 1);
        
        sort(people.begin(), people.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0] < b[0] || (a[0] == b[0] && a[1] > b[1]);
        });
        
        vector<vector<int>> result(n);
        for (auto& p : people) {
            int pos = findKth(p[1] + 1);
            result[pos] = p;
            update(pos, -1);
        }
        return result;
    }
};


// ============================================================
// 【解法对比】
//
//  解法1 (高→矮插入):
//    - 优势：代码最短（8行核心），思路最直觉
//    - 劣势：vector::insert 每次 O(n)，总 O(n²)
//    - 适合：面试首选，代码清晰好解释
//
//  解法2 (矮→高占位):
//    - 优势：展示了反向思维能力
//    - 劣势：线性找空位 O(n)，总 O(n²)
//    - 适合：面试中展示多种思路
//
//  解法3 (树状数组+二分):
//    - 优势：O(n log²n)
//    - 劣势：代码较长，面试时间可能不够
//    - 适合：面试追问优化时口述
//
//  解法4 (树上二分):
//    - 优势：真正 O(n log n)
//    - 劣势：实现复杂
//    - 适合：竞赛 / 追问最优复杂度
//
// 【易错点】
//
// 1. 同身高排序方向搞反:
//    ✗ 高→矮策略中同身高按 k 降序: sort by (h desc, k desc)
//      → [7,1]先于[7,0]插入，[7,0]再插入pos 0后，[7,1]在pos 2而不是pos 1
//    ✓ 高→矮策略中同身高按 k 升序: sort by (h desc, k asc)
//
// 2. 两种策略的排序混淆:
//    高→矮: h降序, 同h k升序
//    矮→高: h升序, 同h k降序
//    规律：主排序方向和同身高的 k 排序方向总是相反的
//
// 3. insert 位置理解错误:
//    ✗ result.insert(result.begin() + p[1] - 1, p)  // 以为k是1-indexed
//    ✓ result.insert(result.begin() + p[1], p)       // k是0-indexed的位置
//
// 4. 树状数组下标:
//    树状数组内部 1-indexed，update/query 要 i++ 转换
//    忘记 +1 会导致 tree[0] 永远不被更新（i & (-i) 对 0 为 0，死循环）
//
// 【面试追问】
//
// Q1: 为什么贪心策略是正确的？能否严格证明？
//     → 高→矮插入时，已在数组中的人身高 ≥ 当前人。
//       插入到位置 k 保证前面恰好 k 个 ≥ h 的人。
//       后续矮个子插入虽然改变位置，但不被计入 k（身高 < h），
//       所以不影响已放置的人的正确性。数学归纳可证明。
//
// Q2: O(n²) 能否优化？瓶颈在哪？
//     → 解法1瓶颈在 vector::insert O(n)（内存移动）
//       解法2瓶颈在线性找空位 O(n)
//       用树状数组维护空位前缀和 + 二分查找 → O(n log²n)
//       树上二分 → O(n log n)
//
// Q3: 如果条件改为"前面恰好有 k 个身高严格大于 h 的人"，怎么改？
//     → 高→矮策略：同身高的人互相"不可见"了。
//       排序时同身高按 k 降序（而不是升序），
//       因为同身高间互不影响 k 值，k大的后面插入即可。
//       其余逻辑不变。
// ============================================================
