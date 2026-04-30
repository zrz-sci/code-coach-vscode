# 2694. 事件发射器 (Event Emitter)

## 核心思路

实现 JavaScript 的**观察者模式 (Observer Pattern)**。底层数据结构是 `Map<string, Function[]>`——事件名映射到回调函数数组。核心操作三个：`subscribe` 往数组 push 回调并返回 `{unsubscribe}` 对象；`emit` 遍历数组调用所有回调并收集返回值；`unsubscribe` 通过闭包捕获的回调引用，用 `splice` 从数组中移除。看似简单，但 **闭包引用**、**splice vs filter**、**展开运算符** 三个陷阱区分面试水平。

## 思维链

1. **读完题第一反应** → 类似 Node.js 的 `EventEmitter` 或浏览器的 `addEventListener`。需要维护"事件名 → 回调列表"的映射。

2. **选什么数据结构存储？** → `Map` 优于普通对象 `{}`：Map 的键类型灵活、无原型链污染（如 `__proto__`）、有 `.has()` 方便检查。值是 `Function[]` 数组，因为同一事件可以有多个监听器，且按注册顺序调用。

3. **subscribe 的难点在返回值** → 返回一个对象 `{ unsubscribe: () => { ... } }`。这个 unsubscribe 函数必须通过**闭包**"记住"当初注册的那个 callback 引用，以便精确移除。

4. **unsubscribe 怎么从数组中移除？** →
   - `splice`：原地修改数组 ✓（所有引用同步看到变化）
   - `filter`：创建新数组 ✗（旧引用看不到变化，如果 emit 中途 unsubscribe 会出 bug）
   - 标记删除：设为 null ✓（O(1) 删除，但 emit 时需跳过 null）

5. **emit 如何传参？** → `cb(...args)` 用展开运算符，不是 `cb(args)`。前者传多个独立参数，后者传一个数组参数——语义完全不同。

6. **边界**：emit 一个无订阅者的事件 → 返回 `[]`；args 是可选的，默认 `[]`。

## 数据结构图

```js
EventEmitter 内部结构:

this.events = Map {
    "click"   → [cbA, cbB, cbC]      ← 按注册顺序排列
    "hover"   → [cbD]
    "submit"  → [cbE, cbF]
}

subscribe("click", cbG):
  1. events.get("click").push(cbG)
  2. 闭包捕获 cbG 和 listeners 引用
  3. 返回 { unsubscribe: () => splice(indexOf(cbG), 1) }

  events = Map {
      "click" → [cbA, cbB, cbC, cbG]   ← cbG 追加到末尾
      ...
  }

emit("click", [5, 10]):
  遍历 [cbA, cbB, cbC, cbG]，依次调用:
    cbA(5, 10) → retA
    cbB(5, 10) → retB
    cbC(5, 10) → retC
    cbG(5, 10) → retG
  返回 [retA, retB, retC, retG]

unsubscribe cbB:
  indexOf(cbB) → 1
  splice(1, 1) 原地删除
  events = Map {
      "click" → [cbA, cbC, cbG]   ← cbB 被移除，后面的前移
  }
```

## 执行流程追踪

```
Example 4 完整追踪:

const emitter = new EventEmitter();
  → events = Map {}

const sub1 = emitter.subscribe("firstEvent", x => x + 1);
  → events = Map { "firstEvent": [cb1] }      cb1 = x => x + 1
  → sub1 = { unsubscribe: () => splice(indexOf(cb1), 1) }

const sub2 = emitter.subscribe("firstEvent", x => x + 2);
  → events = Map { "firstEvent": [cb1, cb2] } cb2 = x => x + 2
  → sub2 = { unsubscribe: () => splice(indexOf(cb2), 1) }

sub1.unsubscribe();
  → indexOf(cb1) → 0
  → splice(0, 1) → 移除 cb1
  → events = Map { "firstEvent": [cb2] }
  → 返回 undefined

emitter.emit("firstEvent", [5]);
  → 遍历 [cb2]: cb2(5) = 5 + 2 = 7
  → 返回 [7] ✓
```

## 解法概览

| 解法 | subscribe | emit | unsubscribe | 特点 |
|------|-----------|------|-------------|------|
| Map + splice | O(1) | O(k) | O(k) | 标准面试解法 |
| Map + 标记删除 | O(1) | O(k) | O(1) | 高频取消场景优化 |
| Map + 快照 emit | O(1) | O(k) | O(k) | 防 emit 中途修改 |

> k = 某事件的回调数量

## 关键提示

1. **闭包是核心机制**：unsubscribe 通过闭包捕获了三个变量——`listeners`（数组引用）、`callback`（回调引用）、以及隐含的 `this`。正因为闭包"记住了"这些引用，调用 unsubscribe 时才能精确找到并移除目标回调。
   ```javascript
   subscribe(eventName, callback) {
       const listeners = this.events.get(eventName);
       listeners.push(callback);
       return {
           unsubscribe: () => {
               // 闭包捕获了 listeners 和 callback
               const idx = listeners.indexOf(callback);
               if (idx !== -1) listeners.splice(idx, 1);
           }
       };
   }
   ```

2. **splice vs filter 是经典面试陷阱**：
   ```javascript
   // 错误！filter 创建新数组，不修改原数组
   unsubscribe: () => {
       this.events.set(eventName, listeners.filter(cb => cb !== callback));
       // 如果 emit 正在遍历旧数组，它看不到这个新数组！
   }
   // 正确！splice 原地修改
   unsubscribe: () => {
       const idx = listeners.indexOf(callback);
       if (idx !== -1) listeners.splice(idx, 1);
   }
   ```

3. **展开运算符 `...args` 不能忘**：
   ```javascript
   cb(...args)  // 正确: args=[1,2,3] → cb(1, 2, 3)
   cb(args)     // 错误: args=[1,2,3] → cb([1,2,3])  传入的是一个数组参数！
   ```

4. **emit 的 args 要有默认值 `[]`**：题目说 args 是可选的。如果不设默认值，`emit("event")` 时 args 是 `undefined`，`cb(...undefined)` 会报 TypeError。

5. **Map 的 has/get 组合模式**：
   ```javascript
   if (!this.events.has(eventName)) {
       this.events.set(eventName, []);
   }
   const listeners = this.events.get(eventName);
   ```

6. **为什么用 `indexOf` 而非 `findIndex`？** → 我们比较的是函数引用（=== 比较），`indexOf` 用 === 比较就够了。`findIndex` 用于需要自定义比较逻辑的场景。

## 解法详解

### 解法 1: Map + Array + splice — 面试标准解法

**设计思路**：最直接的实现。Map 存事件名 → 回调数组，subscribe 追加，unsubscribe 用 indexOf + splice 原地删除，emit 用 map 遍历调用。

```
subscribe("click", cbA)    → events: { "click": [cbA] }
subscribe("click", cbB)    → events: { "click": [cbA, cbB] }
emit("click", [5])         → [cbA(5), cbB(5)] → 收集返回值
unsubscribe cbA            → splice: events: { "click": [cbB] }
emit("click", [5])         → [cbB(5)]
```

### 解法 2: Map + 标记删除 — 高性能场景

**设计思路**：unsubscribe 不移除元素，而是将回调置为 `null`；emit 遍历时跳过 null。unsubscribe 从 O(k) 降为 O(1)。适合大量订阅者 + 频繁取消的场景（如实时推送系统）。

**代价**：数组中有"空洞"（null），长期运行需定期清理。

### 解法 3: 快照 emit — 工程级方案

**设计思路**：emit 开始时先复制一份监听器数组快照，再遍历快照。这样即使某个回调执行时触发了新的 subscribe/unsubscribe，也不影响当前 emit 的遍历。Node.js 的 EventEmitter 内部就是这样做的。

## 与 Node.js EventEmitter 的对比

| 特性 | 本题 | Node.js EventEmitter |
|------|------|---------------------|
| once 支持 | 无 | `emitter.once(event, cb)` |
| 错误处理 | 无 | 'error' 事件特殊处理，无监听器时抛异常 |
| 最大监听器数 | 无限制 | 默认 10，超过打印警告（防内存泄漏） |
| 返回值收集 | emit 返回 `[result1, ...]` | emit 返回 boolean（有无监听器） |
| unsubscribe 方式 | 返回对象含 unsubscribe 方法 | `removeListener(event, cb)` |
| 参数传递 | 数组展开 `cb(...args)` | 直接传参 `cb(arg1, arg2)` |
| 事件顺序 | 按 subscribe 顺序 | 同，但 `prependListener` 可插队 |

## 易错点

1. **忘记处理事件不存在的情况**：emit 一个从未订阅的事件，`this.events.get(eventName)` 返回 `undefined`，直接 `.map()` 报 TypeError。必须先 `has` 检查或返回 `[]`。

2. **unsubscribe 的幂等性**：虽然题目不要求，但好的实现应保证多次调用 unsubscribe 不报错。可以用 `if (idx !== -1)` 守卫。

3. **this 绑定问题**：如果 unsubscribe 用普通 function 而非箭头函数，`this` 会指向返回的对象而非 EventEmitter 实例。箭头函数继承外层 `this`，避免此问题。

4. **emit 中修改订阅列表**：如果回调 A 执行时调用了 `sub.unsubscribe()` 移除回调 B，splice 修改了数组，可能导致后续遍历跳过某个回调或重复调用。解决方案：emit 前复制快照。

5. **args 默认值遗漏**：`emit(eventName, args = [])` 中的 `= []` 不能省。

## 面试追问

**Q1: 如何实现 `once`（只触发一次）？**
```javascript
once(eventName, callback) {
    const sub = this.subscribe(eventName, (...args) => {
        sub.unsubscribe(); // 调用一次后自动取消
        return callback(...args);
    });
    return sub;
}
```

**Q2: 如何支持通配符事件 `*`？**
> 维护一个特殊的 `*` 监听器列表。emit 任何事件时，除了触发该事件的回调，还同时触发 `*` 的所有回调。

**Q3: 异步回调怎么处理？**
> 如果回调返回 Promise，emit 可以改用 `Promise.all(listeners.map(cb => cb(...args)))` 等待所有异步回调完成。

**Q4: 如何防内存泄漏？**
> 添加 `maxListeners` 属性。subscribe 时检查当前事件的监听器数量，超过则打印警告。Node.js 默认是 10。

## 复杂度分析

| 操作 | 时间 | 空间 |
|------|------|------|
| subscribe | O(1) 均摊 (push) | O(1) |
| unsubscribe (splice) | O(k) (indexOf + splice) | O(1) |
| unsubscribe (标记) | O(1) | O(1) |
| emit | O(k) | O(k) (返回值数组) |
| 总空间 | — | O(E * K) |

> E = 不同事件类型数，K = 平均每事件回调数

## 相关题目

| 题号 | 题目 | 关联 |
|------|------|------|
| 2695 | Array Wrapper | JS 类设计，重写 valueOf/toString |
| 2700 | Differences Between Two Objects | JS 深度对象比较 |
| 2725 | Interval Cancellation | setInterval + 闭包 + 清理 |
| 2637 | Promise Time Limit | Promise + setTimeout + 闭包 |
| 2622 | Cache With Time Limit | Map + setTimeout 设计 |
