# Dijkstra 算法详解

## 📌 算法概述

Dijkstra 算法是解决**单源最短路径问题**的经典算法，适用于**非负权重**的图。

---

## 🎯 问题定义

**单源最短路径问题：**
- 给定一个带权有向图 G = (V, E)
- 给定一个源点 s ∈ V
- 求从 s 到图中所有其他节点的最短路径

**在本项目中：**
- 节点 = 高铁站点
- 边 = 站点之间的直达线路
- 权重 = 1（表示经过一个站点）
- 最短路径 = 最少站点数

---

## 🧠 核心思想

Dijkstra 算法是一种**贪心算法**，其核心思想是：

1. **局部最优**：每次选择距离起点最近的未访问节点
2. **全局最优**：通过逐步扩展，最终得到全局最短路径

**为什么贪心有效？**
- 假设从起点 s 到节点 u 的最短路径已经确定
- 那么从 s 到 u 的任何其他路径都不可能更短
- 因此可以安全地将 u 标记为"已确定"

---

## 📝 算法步骤

### 伪代码

```
DIJKSTRA(G, s):
    // 初始化
    for each vertex v in G:
        dist[v] = ∞          // 距离初始化为无穷大
        prev[v] = NULL       // 前驱节点为空
        visited[v] = false   // 未访问
    
    dist[s] = 0              // 起点距离为0
    
    // 主循环
    for i = 1 to |V|-1:
        // 步骤1：找到未访问节点中距离最小的
        u = EXTRACT-MIN(Q)   // Q 是未访问节点集合
        
        // 步骤2：标记为已访问
        visited[u] = true
        
        // 步骤3：更新邻居节点
        for each neighbor v of u:
            if not visited[v]:
                alt = dist[u] + weight(u, v)
                if alt < dist[v]:
                    dist[v] = alt
                    prev[v] = u
    
    return dist, prev
```

### 图示说明

```
初始状态：
    起点 A 距离 = 0
    其他节点距离 = ∞

    A(0) --- B(∞) --- C(∞)
     |         |         |
     D(∞) --- E(∞) --- F(∞)

第1次迭代：
    选择 A（距离最小）
    更新邻居：B = 1, D = 1

    A(0) --- B(1) --- C(∞)
     |         |         |
     D(1) --- E(∞) --- F(∞)

第2次迭代：
    选择 B（距离=1）
    更新邻居：C = 2, E = 2

    A(0) --- B(1) --- C(2)
     |         |         |
     D(1) --- E(2) --- F(∞)

第3次迭代：
    选择 D（距离=1）
    更新邻居：E = 2（不更新，因为 1+1=2 不小于 2）

    A(0) --- B(1) --- C(2)
     |         |         |
     D(1) --- E(2) --- F(∞)

第4次迭代：
    选择 C（距离=2）
    更新邻居：F = 3

    A(0) --- B(1) --- C(2)
     |         |         |
     D(1) --- E(2) --- F(3)

第5次迭代：
    选择 E（距离=2）
    更新邻居：F = 3（不更新）

    A(0) --- B(1) --- C(2)
     |         |         |
     D(1) --- E(2) --- F(3)

算法结束：
    最短路径：
    A → B = 1
    A → D = 1
    A → C = 2 (A → B → C)
    A → E = 2 (A → B → E 或 A → D → E)
    A → F = 3 (A → B → C → F)
```

---

## 💻 C 语言实现

### 1. 数据结构定义

```c
#define MAX_VERTICES 100
#define INF 999999

// 邻接表节点
typedef struct AdjNode {
    int dest;           // 目标节点
    int weight;         // 权重
    struct AdjNode* next;
} AdjNode;

// 图结构
typedef struct {
    int numVertices;
    AdjNode* adjList[MAX_VERTICES];
} Graph;

// Dijkstra 结果
typedef struct {
    int dist[MAX_VERTICES];    // 最短距离
    int prev[MAX_VERTICES];    // 前驱节点
    int visited[MAX_VERTICES]; // 是否已访问
} DijkstraResult;
```

### 2. 算法实现

```c
DijkstraResult* dijkstra(Graph* graph, int startVertex) {
    // 分配结果内存
    DijkstraResult* result = malloc(sizeof(DijkstraResult));
    
    // 初始化
    for (int i = 0; i < graph->numVertices; i++) {
        result->dist[i] = INF;
        result->prev[i] = -1;
        result->visited[i] = 0;
    }
    
    // 起点距离为0
    result->dist[startVertex] = 0;
    
    // 主循环
    for (int count = 0; count < graph->numVertices - 1; count++) {
        // 步骤1：找到未访问节点中距离最小的
        int minDist = INF;
        int minVertex = -1;
        
        for (int v = 0; v < graph->numVertices; v++) {
            if (!result->visited[v] && result->dist[v] < minDist) {
                minDist = result->dist[v];
                minVertex = v;
            }
        }
        
        // 如果没有可达节点，退出
        if (minVertex == -1) break;
        
        // 步骤2：标记为已访问
        result->visited[minVertex] = 1;
        
        // 步骤3：更新邻居节点
        AdjNode* current = graph->adjList[minVertex];
        while (current) {
            int dest = current->dest;
            int weight = current->weight;
            
            // 松弛操作
            if (!result->visited[dest] &&
                result->dist[minVertex] + weight < result->dist[dest]) {
                result->dist[dest] = result->dist[minVertex] + weight;
                result->prev[dest] = minVertex;
            }
            
            current = current->next;
        }
    }
    
    return result;
}
```

### 3. 路径重建

```c
void printPath(DijkstraResult* result, Graph* graph, int start, int end) {
    // 检查是否可达
    if (result->dist[end] == INF) {
        printf("从 %d 到 %d 不可达\n", start, end);
        return;
    }
    
    // 重建路径
    int path[MAX_VERTICES];
    int pathLen = 0;
    int current = end;
    
    // 从终点回溯到起点
    while (current != -1) {
        path[pathLen++] = current;
        current = result->prev[current];
    }
    
    // 打印路径
    printf("最短路径：");
    for (int i = pathLen - 1; i >= 0; i--) {
        printf("%d", path[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
    
    printf("距离：%d\n", result->dist[end]);
}
```

---

## ⏱️ 复杂度分析

### 时间复杂度

**朴素实现（本项目使用）：**
- 外层循环：O(V)
- 内层找最小值：O(V)
- 更新邻居：O(E)
- **总复杂度：O(V² + E) = O(V²)**

**优先队列优化：**
- 使用最小堆找最小值：O(log V)
- 更新邻居：O(E log V)
- **总复杂度：O((V + E) log V)**

### 空间复杂度
- 距离数组：O(V)
- 前驱数组：O(V)
- 访问标记：O(V)
- 邻接表：O(V + E)
- **总复杂度：O(V + E)**

---

## 🔍 算法正确性证明

### 定理
Dijkstra 算法能够正确计算从源点到所有其他节点的最短路径。

### 证明（数学归纳法）

**基础情况：**
- 初始时，源点 s 的距离为 0，这是正确的
- 其他节点的距离为 ∞，表示尚未发现路径

**归纳假设：**
- 假设前 k 次迭代中，所有已访问节点的最短路径都已正确计算

**归纳步骤：**
- 第 k+1 次迭代选择节点 u（距离最小）
- 假设存在一条更短路径 P 从 s 到 u
- 路径 P 必须经过某个未访问节点 v
- 但 dist[v] > dist[u]（因为 u 是距离最小的未访问节点）
- 因此路径 P 的长度 > dist[u]
- 矛盾！所以不存在更短路径

**结论：**
- 每次选择的节点 u 的最短路径都是正确的
- 因此算法最终能够正确计算所有节点的最短路径

---

## ⚠️ 算法限制

### 1. 不能处理负权边
- 原因：贪心选择依赖于"已确定的最短路径不会被更新"
- 解决方案：使用 Bellman-Ford 算法

### 2. 只能计算单源最短路径
- 原因：算法从单个源点开始扩展
- 解决方案：使用 Floyd-Warshall 算法

### 3. 需要非负权重
- 原因：负权边会破坏贪心选择的正确性
- 解决方案：使用 Bellman-Ford 算法

---

## 🚀 优化技巧

### 1. 优先队列优化
```c
// 使用最小堆找最小值
int extractMin(DijkstraResult* result) {
    // 实现最小堆
    // ...
}
```

### 2. 提前终止
```c
// 如果只需要到某个终点的最短路径
if (minVertex == endVertex) {
    break;  // 提前终止
}
```

### 3. 双向 Dijkstra
- 从起点和终点同时开始搜索
- 当两个搜索相遇时停止
- 可以显著减少搜索空间

---

## 📚 相关算法

### 1. Bellman-Ford 算法
- 支持负权边
- 时间复杂度：O(VE)
- 可以检测负权环

### 2. Floyd-Warshall 算法
- 计算所有节点对的最短路径
- 时间复杂度：O(V³)
- 空间复杂度：O(V²)

### 3. A* 算法
- Dijkstra 的启发式版本
- 使用启发函数引导搜索
- 在有良好启发函数时更高效

---

## 🎓 学习建议

### 初学者
1. 先理解算法的伪代码
2. 手动模拟小规模图的执行过程
3. 阅读并理解 C 语言实现

### 进阶者
1. 实现优先队列优化版本
2. 比较不同实现的性能
3. 尝试处理特殊情况（如不可达节点）

### 高级者
1. 实现其他最短路径算法
2. 研究算法的并行化
3. 优化内存使用和缓存效率

---

## 📖 参考文献

1. Dijkstra, E. W. (1959). "A note on two problems in connexion with graphs". Numerische Mathematik. 1: 269–271.
2. Cormen, T. H., et al. (2009). Introduction to Algorithms (3rd ed.). MIT Press.
3. 严蔚敏, 吴伟民. 数据结构 (C 语言版). 清华大学出版社.

---

*作者: greece*
*最后更新: 2025-05-31*
