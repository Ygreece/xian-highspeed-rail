# 西安高铁最短路径查询系统

## 📚 项目简介

这是一个**算法学习项目**，通过实现 Dijkstra 算法来计算从西安出发到全国高铁城市的最少站点数。

**学习目标：**
- 理解图数据结构的基本概念
- 掌握 Dijkstra 最短路径算法的实现
- 学习邻接表的构建和使用
- 实践 C 语言的内存管理和数据结构操作

---

## 🎯 算法原理

### 1. 什么是 Dijkstra 算法？

Dijkstra 算法是一种用于计算**单源最短路径**的经典算法，由荷兰计算机科学家 Edsger W. Dijkstra 于 1956 年提出。

**核心思想：**
- 从起点开始，逐步扩展到其他节点
- 每次选择距离起点最近的未访问节点
- 更新该节点的所有邻居的距离
- 重复直到所有节点都被访问

### 2. 算法步骤

```
1. 初始化：
   - 起点距离 = 0
   - 其他节点距离 = ∞
   - 所有节点标记为未访问

2. 重复以下步骤：
   a. 选择未访问节点中距离最小的节点 u
   b. 标记 u 为已访问
   c. 对于 u 的每个未访问邻居 v：
      - 计算新距离 = dist[u] + weight(u,v)
      - 如果新距离 < dist[v]，更新 dist[v]

3. 当所有节点都被访问时，算法结束
```

### 3. 时间复杂度

- **朴素实现**：O(V²)
- **优先队列优化**：O((V + E) log V)

其中 V 是节点数，E 是边数。

---

## 🏗️ 图的构建

### 1. 什么是图？

图是一种数据结构，由**节点（顶点）**和**边**组成。

**在本项目中：**
- **节点** = 高铁站点
- **边** = 站点之间的直达线路
- **权重** = 1（表示经过一个站点）

### 2. 图的表示方法

#### 邻接矩阵
```
    A  B  C  D
A [ 0  1  0  1 ]
B [ 1  0  1  0 ]
C [ 0  1  0  1 ]
D [ 1  0  1  0 ]
```

#### 邻接表（本项目使用）
```
A -> B -> D
B -> A -> C
C -> B -> D
D -> A -> C
```

### 3. 数据结构设计

```c
// 站点结构体
typedef struct {
    int id;           // 站点ID
    char name[50];    // 站点名称
    char code[10];    // 电报码
} Station;

// 邻接表节点
typedef struct AdjNode {
    int dest;         // 目标站点ID
    int weight;       // 权重（本项目中为1）
    struct AdjNode* next;
} AdjNode;

// 图结构
typedef struct {
    int numVertices;          // 站点数量
    Station stations[MAX];    // 站点数组
    AdjNode* adjList[MAX];    // 邻接表
} Graph;
```

---

## 💻 代码实现详解

### 1. 图的初始化

```c
Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = 0;
    for (int i = 0; i < MAX; i++) {
        graph->adjList[i] = NULL;  // 初始化邻接表为空
    }
    return graph;
}
```

### 2. 添加边（连接站点）

```c
void addEdge(Graph* graph, int src, int dest) {
    // 添加 src -> dest 的边
    AdjNode* newNode = (AdjNode*)malloc(sizeof(AdjNode));
    newNode->dest = dest;
    newNode->weight = 1;
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    // 添加 dest -> src 的边（无向图）
    newNode = (AdjNode*)malloc(sizeof(AdjNode));
    newNode->dest = src;
    newNode->weight = 1;
    newNode->next = graph->adjList[dest];
    graph->adjList[dest] = newNode;
}
```

### 3. Dijkstra 算法实现

```c
DijkstraResult* dijkstra(Graph* graph, int startVertex) {
    // 初始化结果结构
    DijkstraResult* result = malloc(sizeof(DijkstraResult));
    for (int i = 0; i < graph->numVertices; i++) {
        result->dist[i] = INF;    // 初始距离为无穷大
        result->prev[i] = -1;     // 前驱节点为-1
        result->visited[i] = 0;   // 未访问
    }

    // 起点距离为0
    result->dist[startVertex] = 0;

    // 主循环：遍历所有节点
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

        // 步骤3：更新邻居节点的距离
        AdjNode* current = graph->adjList[minVertex];
        while (current) {
            int dest = current->dest;
            int weight = current->weight;

            // 如果找到更短路径，更新距离
            if (!result->visited[dest] &&
                result->dist[minVertex] + weight < result->dist[dest]) {
                result->dist[dest] = result->dist[minVertex] + weight;
                result->prev[dest] = minVertex;  // 记录前驱节点
            }

            current = current->next;
        }
    }

    return result;
}
```

### 4. 路径重建

```c
void printPath(DijkstraResult* result, Graph* graph, int start, int end) {
    // 如果不可达
    if (result->dist[end] == INF) {
        printf("不可达\n");
        return;
    }

    // 重建路径：从终点回溯到起点
    int path[MAX];
    int pathLen = 0;
    int current = end;

    while (current != -1) {
        path[pathLen++] = current;
        current = result->prev[current];
    }

    // 打印路径
    printf("路径：");
    for (int i = pathLen - 1; i >= 0; i--) {
        printf("%s", graph->stations[path[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
    printf("经过站点数：%d\n", result->dist[end]);
}
```

---

## 📊 数据来源

### 1. 站点数据
- **来源**：12306 官方接口
- **数量**：全国 3338 个高铁站点
- **格式**：站名、电报码、拼音

### 2. 线路数据
- **郑西高铁**：西安 → 郑州（7 站）
- **西成高铁**：西安 → 成都（12 站）
- **大西高铁**：西安 → 太原（13 站）
- **西武高铁**：西安 → 武汉（9 站）
- **西渝高铁**：西安 → 重庆（9 站）
- **西银高铁**：西安 → 银川（9 站）
- **京广高铁**：北京 → 广州（24 站）
- **京沪高铁**：北京 → 上海（23 站）
- **沪昆高铁**：上海 → 昆明（39 站）

---

## 🚀 使用方式

### 方式一：Web 图形界面（推荐）

```bash
# 直接用浏览器打开
open index.html
```

**功能特点：**
- 🎨 可视化展示算法过程
- 📊 实时统计信息
- 🔍 下拉框选择站点
- 📈 显示所有路线

### 方式二：命令行工具

```bash
# 编译
make

# 运行示例
./railway -a              # 从西安到所有站点
./railway -f 西安 成都     # 西安到成都
./railway -s 北京          # 从北京出发
./railway -l              # 列出所有站点
```

---

## 📖 学习指南

### 第一步：理解图的概念
1. 什么是节点和边？
2. 什么是邻接表？
3. 如何用代码表示图？

### 第二步：理解 Dijkstra 算法
1. 算法的核心思想是什么？
2. 为什么选择距离最小的节点？
3. 如何保证找到最短路径？

### 第三步：动手实现
1. 先实现图的构建
2. 再实现 Dijkstra 算法
3. 最后实现路径重建

### 第四步：优化和扩展
1. 如何优化时间复杂度？
2. 如何处理负权边？
3. 如何支持多起点查询？

---

## 🔧 扩展练习

### 初级练习
- [ ] 理解代码中的每个函数
- [ ] 手动模拟算法执行过程
- [ ] 修改权重，观察结果变化

### 中级练习
- [ ] 实现优先队列优化版本
- [ ] 添加 A* 算法实现
- [ ] 支持有向图

### 高级练习
- [ ] 实现 Bellman-Ford 算法（支持负权边）
- [ ] 实现 Floyd-Warshall 算法（所有节点对最短路径）
- [ ] 优化内存使用

---

## 📚 参考资料

### 算法相关
- [Dijkstra 算法 - 维基百科](https://zh.wikipedia.org/wiki/%E6%88%B4%E5%85%8B%E6%96%AF%E7%89%B9%E6%8B%89%E7%AE%97%E6%B3%95)
- [图论基础 - 中国大学 MOOC](https://www.icourse163.org/)
- [数据结构 - 浙江大学](https://www.icourse163.org/course/ZJU-93001)

### C 语言相关
- [C 语言内存管理](https://www.runoob.com/cprogramming/c-memory-management.html)
- [C 语言数据结构](https://www.runoob.com/data-structures/data-structures-tutorial.html)

---

## 🎓 项目收获

通过这个项目，你将掌握：

1. **图数据结构**
   - 节点、边、权重的概念
   - 邻接表的实现
   - 图的遍历方法

2. **Dijkstra 算法**
   - 最短路径的定义
   - 贪心算法的思想
   - 算法的时间复杂度分析

3. **C 语言技能**
   - 结构体的使用
   - 动态内存分配
   - 链表操作

4. **工程实践**
   - 代码模块化设计
   - 文件读取和处理
   - 用户界面设计

---

## 📝 作者

**greece**

这是一个学习项目，欢迎提出改进建议！

---

## 📄 许可证

MIT License
