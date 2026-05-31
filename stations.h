#ifndef STATIONS_H
#define STATIONS_H

/**
 * 图数据结构定义
 *
 * 本文件定义了图（Graph）数据结构的基本组件：
 * 1. Station - 站点结构体
 * 2. AdjNode - 邻接表节点
 * 3. Graph   - 图结构
 *
 * 图的表示方法：邻接表
 * - 每个节点维护一个链表
 * - 链表中的每个节点表示一条边
 * - 适用于稀疏图（边数远小于节点数的平方）
 */

// 常量定义
#define MAX_STATIONS 3500   // 最大站点数量（覆盖全国3173个站点）
#define MAX_NAME_LEN 50     // 站点名称最大长度

/**
 * 站点结构体
 *
 * 存储单个站点的信息：
 * - id:   站点的唯一标识符
 * - name: 站点的中文名称
 * - code: 站点的电报码（用于12306接口）
 */
typedef struct {
    int id;                     // 站点ID（唯一标识符）
    char name[MAX_NAME_LEN];    // 中文站名（如"西安"）
    char code[10];              // 电报码（如"XAY"）
} Station;

/**
 * 邻接表节点结构体
 *
 * 表示图中的一条边：
 * - dest:   目标节点的ID
 * - weight: 边的权重（本项目中为1，表示经过一个站点）
 * - next:   指向下一个邻接节点的指针（链表结构）
 *
 * 邻接表的表示方式：
 * 节点0 -> [1] -> [3] -> NULL
 * 节点1 -> [0] -> [2] -> NULL
 * 节点2 -> [1] -> NULL
 * 节点3 -> [0] -> NULL
 */
typedef struct AdjNode {
    int dest;                   // 目标站点ID
    int weight;                 // 权重（站点数=1）
    struct AdjNode* next;       // 下一个邻接节点（链表指针）
} AdjNode;

/**
 * 图结构
 *
 * 存储整个图的信息：
 * - numVertices: 站点数量
 * - stations:    站点数组（存储所有站点信息）
 * - adjList:     邻接表数组（存储边的信息）
 *
 * 邻接表的优点：
 * 1. 空间效率高：O(V + E)，其中V是节点数，E是边数
 * 2. 添加边操作：O(1)
 * 3. 遍历邻居：O(度数)
 *
 * 邻接表的缺点：
 * 1. 查询两点间是否有边：O(度数)
 * 2. 不适合稠密图
 */
typedef struct {
    int numVertices;            // 站点数量
    Station stations[MAX_STATIONS];  // 站点数组
    AdjNode* adjList[MAX_STATIONS];  // 邻接表
} Graph;

// 函数声明

/**
 * 创建图结构
 *
 * 分配图结构的内存并初始化
 * @return 图结构指针
 */
Graph* createGraph();

/**
 * 添加站点到图中
 *
 * @param graph 图结构指针
 * @param id    站点ID
 * @param name  站点名称
 * @param code  电报码
 */
void addStation(Graph* graph, int id, const char* name, const char* code);

/**
 * 添加边到图中（无向图）
 *
 * @param graph 图结构指针
 * @param src   源站点ID
 * @param dest  目标站点ID
 */
void addEdge(Graph* graph, int src, int dest);

/**
 * 从文件加载站点和线路数据
 *
 * @param graph    图结构指针
 * @param filename 文件名
 */
void loadStationsFromFile(Graph* graph, const char* filename);

/**
 * 释放图结构内存
 *
 * @param graph 图结构指针
 */
void freeGraph(Graph* graph);

#endif
