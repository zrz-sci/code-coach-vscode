/*
 * LeetCode 2694: 事件发射器 (Event Emitter)
 *
 * 标签: JavaScript, Design, Observer Pattern
 * 难度: Medium
 *
 * 三种解法:
 * 1. Map + Array + splice  — 标准面试解法
 * 2. Map + 标记删除        — 高性能版 (unsubscribe O(1))
 * 3. 工程级完整版          — 快照 emit + once 扩展
 */


// ============================================================
// 解法1: Map + Array + splice — 标准面试解法 ⭐
//
// 数据结构:
//   this.events = Map<string, Function[]>
//
// 操作:
//   subscribe → push 到数组末尾, 返回 {unsubscribe}
//   emit      → 遍历数组调用所有回调, 收集返回值
//   unsubscribe → indexOf + splice 原地删除
//
// 关键设计决策:
//   1. splice 而非 filter — 原地修改数组保持引用一致
//   2. 箭头函数闭包 — 捕获 listeners 和 callback 引用
//   3. args 默认值 [] — 防止 undefined 展开报错
//
// 时间: subscribe O(1), emit O(k), unsubscribe O(k)
// 空间: O(E*K) 总, E=事件数, K=平均回调数
// ============================================================
class EventEmitter_v1 {
    constructor() {
        // Map<string, Function[]>
        // 键: 事件名, 值: 回调函数数组(按注册顺序)
        this.events = new Map();
    }

    /**
     * 订阅事件
     * @param {string} eventName - 事件名称
     * @param {Function} callback - 回调函数
     * @return {Object} 包含 unsubscribe 方法的对象
     */
    subscribe(eventName, callback) {
        // 懒初始化: 首次订阅某事件时创建空数组
        if (!this.events.has(eventName)) {
            this.events.set(eventName, []);
        }

        // 获取回调数组的引用(不是副本!)
        // 后续 push/splice 都是修改同一个数组
        const listeners = this.events.get(eventName);
        listeners.push(callback);

        // 返回包含 unsubscribe 的对象
        // 箭头函数通过闭包捕获: listeners(数组引用), callback(函数引用)
        return {
            unsubscribe: () => {
                // indexOf 用 === 比较找到回调位置
                const idx = listeners.indexOf(callback);
                if (idx !== -1) {
                    // splice(起始位置, 删除数量)
                    // 原地修改, 所有持有该数组引用的地方同步看到变化
                    listeners.splice(idx, 1);
                }
                // 题目要求返回 undefined (JS 函数默认返回值)
            }
        };
    }

    /**
     * 触发事件
     * @param {string} eventName - 事件名称
     * @param {Array} args - 传给回调的参数数组(可选, 默认空数组)
     * @return {Array} 所有回调的返回值数组
     */
    emit(eventName, args = []) {
        // 事件未注册过 → 没有订阅者 → 返回空数组
        if (!this.events.has(eventName)) return [];

        // map 遍历所有回调, 用 ...args 展开传参
        // args = [1, 2, 3] → cb(1, 2, 3)  不是 cb([1,2,3])!
        return this.events.get(eventName).map(cb => cb(...args));
    }
}


// ============================================================
// 解法2: Map + 标记删除 — 高性能版
//
// 优化点: unsubscribe 不移除元素, 而是标记为 null
//   → unsubscribe 从 O(k) 降为 O(1)
//   → emit 遍历时跳过 null
//
// 适用场景:
//   大量订阅者 + 频繁取消 (如实时数据流、WebSocket)
//
// 代价:
//   数组有 "空洞"(null), 长期运行需定期清理
//   emit 需要 filter 或手动跳过 null
//
// 时间: subscribe O(1), emit O(k), unsubscribe O(1)
// ============================================================
class EventEmitter_v2 {
    constructor() {
        this.events = new Map();
    }

    subscribe(eventName, callback) {
        if (!this.events.has(eventName)) {
            this.events.set(eventName, []);
        }
        const listeners = this.events.get(eventName);
        listeners.push(callback);

        // 幂等标志: 防止重复 unsubscribe 造成误删
        let active = true;

        return {
            unsubscribe: () => {
                if (!active) return; // 已取消过, 直接返回
                active = false;

                // O(1) 标记删除: 将回调置为 null
                // indexOf 仍然是 O(k), 但可以通过额外记录 index 优化
                const idx = listeners.indexOf(callback);
                if (idx !== -1) {
                    listeners[idx] = null; // 标记而非删除
                }
            }
        };
    }

    emit(eventName, args = []) {
        if (!this.events.has(eventName)) return [];

        const results = [];
        for (const cb of this.events.get(eventName)) {
            if (cb !== null) { // 跳过已标记删除的
                results.push(cb(...args));
            }
        }
        return results;
    }

    /**
     * 维护方法: 清理某事件下的 null 空洞
     * 可以在 emit 后或定时调用
     */
    _compact(eventName) {
        if (!this.events.has(eventName)) return;
        const cleaned = this.events.get(eventName).filter(cb => cb !== null);
        this.events.set(eventName, cleaned);
    }
}


// ============================================================
// 解法3: 工程级完整版 — 快照 emit + once 扩展
//
// 工程级改进:
// 1. emit 前复制快照 → 防止回调中 subscribe/unsubscribe 影响遍历
// 2. once() 方法 → 只触发一次后自动取消
// 3. off() 方法 → Node.js 风格的按名移除
// 4. listenerCount() → 查询监听器数量
// 5. removeAllListeners() → 清除某事件所有监听
//
// Node.js 的 EventEmitter 就使用了快照策略:
//   emit 前 arrayClone(listeners), 确保遍历过程稳定
//
// 面试中展示这个版本体现工程思维和对 Node.js 源码的理解
// ============================================================
class EventEmitter {
    constructor() {
        this.events = new Map();
    }

    /**
     * 核心方法: 订阅事件
     */
    subscribe(eventName, callback) {
        if (!this.events.has(eventName)) {
            this.events.set(eventName, []);
        }
        const listeners = this.events.get(eventName);
        listeners.push(callback);

        return {
            unsubscribe: () => {
                const idx = listeners.indexOf(callback);
                if (idx !== -1) {
                    listeners.splice(idx, 1);
                }
            }
        };
    }

    /**
     * 核心方法: 触发事件 (快照版)
     *
     * 关键改进: 遍历前复制数组快照
     * 如果回调 cbA 在执行中调用了 sub.unsubscribe() 移除 cbB,
     * 普通遍历会导致跳过 cbB 之后的回调 (splice 改变了索引)。
     * 快照遍历不受影响。
     */
    emit(eventName, args = []) {
        if (!this.events.has(eventName)) return [];

        // 浅拷贝数组 → 遍历快照, 不受回调中修改的影响
        const snapshot = [...this.events.get(eventName)];
        return snapshot.map(cb => cb(...args));
    }

    /**
     * 扩展: once — 只触发一次的订阅
     *
     * 实现: 包装原始回调, 在首次调用时自动 unsubscribe
     * 这是闭包 + 高阶函数的经典组合
     *
     * 使用示例:
     *   emitter.once("init", () => console.log("只执行一次"));
     *   emitter.emit("init"); // 打印 "只执行一次"
     *   emitter.emit("init"); // 不打印 (已自动取消)
     */
    once(eventName, callback) {
        const sub = this.subscribe(eventName, (...args) => {
            sub.unsubscribe();       // 先取消, 确保只执行一次
            return callback(...args); // 再调用原始回调
        });
        return sub;
    }

    /**
     * 扩展: off — Node.js 风格移除监听器
     */
    off(eventName, callback) {
        if (!this.events.has(eventName)) return;
        const listeners = this.events.get(eventName);
        const idx = listeners.indexOf(callback);
        if (idx !== -1) listeners.splice(idx, 1);
    }

    /**
     * 扩展: listenerCount — 查询某事件监听器数量
     */
    listenerCount(eventName) {
        return this.events.has(eventName)
            ? this.events.get(eventName).length
            : 0;
    }

    /**
     * 扩展: removeAllListeners — 移除某事件或所有事件的监听器
     */
    removeAllListeners(eventName) {
        if (eventName) {
            this.events.delete(eventName);
        } else {
            this.events.clear();
        }
    }
}


// ============================================================
// 测试验证
// ============================================================
function runTests() {
    console.log("=== Test 1: emit 无订阅者 → [] ===");
    const e1 = new EventEmitter();
    console.log(e1.emit("firstEvent")); // []

    console.log("\n=== Test 2: 多个订阅 + emit ===");
    e1.subscribe("firstEvent", () => 5);
    e1.subscribe("firstEvent", () => 6);
    console.log(e1.emit("firstEvent")); // [5, 6]

    console.log("\n=== Test 3: 参数传递 ===");
    const e2 = new EventEmitter();
    e2.subscribe("add", (...args) => args.join(','));
    console.log(e2.emit("add", [1, 2, 3])); // ["1,2,3"]
    console.log(e2.emit("add", [3, 4, 6])); // ["3,4,6"]

    console.log("\n=== Test 4: unsubscribe ===");
    const e3 = new EventEmitter();
    const sub = e3.subscribe("ev", (...args) => args.join(','));
    console.log(e3.emit("ev", [1, 2, 3])); // ["1,2,3"]
    sub.unsubscribe();
    console.log(e3.emit("ev", [4, 5, 6])); // []

    console.log("\n=== Test 5: 部分取消 ===");
    const e4 = new EventEmitter();
    const s1 = e4.subscribe("ev", x => x + 1);
    const s2 = e4.subscribe("ev", x => x + 2);
    s1.unsubscribe();
    console.log(e4.emit("ev", [5])); // [7]

    console.log("\n=== Test 6: once 扩展 ===");
    const e5 = new EventEmitter();
    e5.once("init", x => x * 10);
    console.log(e5.emit("init", [3])); // [30]
    console.log(e5.emit("init", [3])); // [] (已自动取消)

    console.log("\nAll tests passed!");
}

// runTests();


// ============================================================
// 【面试追问】
// ============================================================
// Q1: 如何实现 once（只触发一次）？
//     → 包装回调，首次调用时自动 unsubscribe（见解法3 once 方法）
// Q2: 如何支持通配符事件 "*"？
//     → 维护特殊 "*" 监听器列表，emit 任何事件时同时触发 "*" 的回调
// Q3: 异步回调怎么处理？
//     → emit 改用 Promise.all(listeners.map(cb => cb(...args)))
// Q4: 如何防内存泄漏？
//     → 添加 maxListeners 属性，超过打印警告（Node.js 默认 10）
//
// 【易错点】
// 1. emit 未注册事件时忘记返回 [] → .map() 报 TypeError ✗
// 2. unsubscribe 用 filter 而非 splice → 新数组与旧引用不同步 ✗
// 3. emit 传参用 cb(args) 而非 cb(...args) → 传入数组而非展开参数 ✗
// 4. 忘记 args 默认值 [] → emit("event") 时 ...undefined 报错 ✗
// 5. unsubscribe 用普通 function 而非箭头函数 → this 指向错误 ✗
