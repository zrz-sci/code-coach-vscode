/*
 * LeetCode 1656: 设计有序流 (Design an Ordered Stream)
 *
 * 【题目本质】
 * 数据乱序到达，按序输出。用数组缓存 + 指针连续扫描，
 * 每次 insert 后从 ptr 开始尽可能多地输出连续已到达的值。
 *
 * 【解法总览】
 * 解法1: 数组 + 指针 — 均摊 O(1) / O(n) — 唯一解法，直接高效
 */

// ============================================================
// 解法1: 数组 + 指针
// 时间: 均摊 O(1) 每次 insert（n 次总计 O(n)）
// 空间: O(n) 存储 n 个字符串
//
// 【思路】
// id 是 1~n 的连续整数，天然映射到数组下标。
// 维护指针 ptr 表示"下一个应该输出的 id"。
// 每次 insert 把值放到 stream[id]，然后从 ptr 开始
// 连续收集非空值，直到遇到空位。
//
// 关键观察：ptr 只会前进，永远不会后退。
// 所以 n 次 insert 总共移动 ptr 最多 n 步 → 均摊 O(1)。
//
// 示例执行过程（ptr 用 ↓ 标注）:
//
// 初始:    [ _  _  _  _  _ ]     ptr=1
//           ↓
//
// insert(3,"ccccc"):
//          [ _  _  c  _  _ ]     ptr=1, stream[1]为空 → 返回 []
//           ↓
//
// insert(1,"aaaaa"):
//          [ a  _  c  _  _ ]     ptr=1, stream[1]有值 → 输出
//              ↓                  ptr→2, stream[2]为空 → 停
//                                返回 ["aaaaa"]
//
// insert(2,"bbbbb"):
//          [ a  b  c  _  _ ]     ptr=2, stream[2]有值 → 输出
//                    ↓            ptr→3, stream[3]有值 → 输出
//                                 ptr→4, stream[4]为空 → 停
//                                返回 ["bbbbb","ccccc"]
//
// insert(5,"eeeee"):
//          [ a  b  c  _  e ]     ptr=4, stream[4]为空 → 返回 []
//                    ↓
//
// insert(4,"ddddd"):
//          [ a  b  c  d  e ]     ptr=4, stream[4]有值 → 输出
//                                 ptr→5, stream[5]有值 → 输出
//                                 ptr→6, 越界 → 停
//                                返回 ["ddddd","eeeee"]
// ============================================================
class OrderedStream {
public:
    vector<string> stream;
    int ptr;

    OrderedStream(int n) {
        // 开 n+1 大小，下标 1~n 直接对应 id，避免 -1 偏移
        stream.resize(n + 1);
        ptr = 1;
    }

    vector<string> insert(int idKey, string value) {
        // 把值放到对应 id 的位置
        stream[idKey] = value;

        vector<string> result;
        // 从 ptr 开始，连续收集所有非空位置的值
        // 因为默认 string 是 ""，用 empty() 判断该位置是否已被 insert
        while (ptr < (int)stream.size() && !stream[ptr].empty()) {
            result.push_back(stream[ptr]);
            ptr++;  // ptr 只前进不后退，这是均摊 O(1) 的保证
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
// 本题只有一种核心思路（数组+指针），变体是用哈希表替代数组：
// - 数组: 适合 id 是连续整数（本题），O(1) 下标访问
// - 哈希表: 适合 id 范围很大/不连续，O(1) 均摊但常数更大
//
// 【易错点】
// 1. 数组大小: stream.resize(n) 会导致 id=n 时越界，
//    必须 resize(n+1)
// 2. ptr 初始值: 题目 id 从 1 开始，ptr 也要从 1 开始，
//    不是 0
// 3. 空位判断: C++ 的 string 默认构造是 ""，
//    用 !stream[ptr].empty() 判断，不是 nullptr
// 4. while 边界: ptr < stream.size() 中 size() 返回
//    unsigned，ptr 是 int，建议强转 (int)stream.size()
//    避免有符号/无符号比较警告
//
// 【面试追问】
// Q1(基础): 单次 insert 的时间复杂度？
//    → 最坏 O(n)，但 n 次调用总计 O(n)，均摊 O(1)。
//    因为 ptr 只前进不后退，每个元素最多被 ptr 扫过一次。
//
// Q2(变体): 如果 id 范围是 1~10^9 怎么办？
//    → 用 unordered_map<int,string> 代替数组，
//    insert 时 map[idKey] = value，
//    while 循环改为 while(map.count(ptr))。
//
// Q3(并发): 多线程并发 insert 怎么保证正确性？
//    → stream[idKey] = value 各写不同位置可以无锁，
//    但 ptr 的读取和推进是临界区，需要 mutex 或 CAS。
// ============================================================
