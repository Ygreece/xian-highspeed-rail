#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "stations.h"

/**
 * Dijkstra 算法头文件
 *
 * 本文件定义了 Dijkstra 最短路径算法相关的数据结构和函数
 *
 * Dijkstra 算法是解决单源最短路径问题的经典算法：
 * - 输入：带权有向图 G = (V, E) 和源点 s
 * - 输出：从 s 到所有其他节点的最短路径
 * - 时间复杂度：O(V²)（朴素实现）
 * - 空间复杂度：O(V)
 *
 * 算法特点：
 * 1. 贪心算法：每次选择距离最近的未访问节点
 * 2. 适用于非负权重图
 * 3. 不能处理负权边
 * 4. 可以使用优先队列优化
 */

// 常量定义
#define INF 999999  // 无穷大（表示不可达）

/**
 * Dijkstra 结果结构体
 *
 * 存储算法的计算结果：
 * - dist[]:    从起点到各节点的最短距离
 * - prev[]:    最短路径中各节点的前驱节点（用于重建路径）
 * - visited[]: 各节点是否已被访问（已确定最短路径）
 *
 * 数组索引对应节点ID：
 * dist[i] 表示从起点到节点 i 的最短距离
 * prev[i] 表示在最短路径中，节点 i 的前一个节点
 * visited[i] 表示节点 i 是否已被访问
 */
typedef struct {
    int dist[MAX_STATIONS];      // 最短距离（站点数）
    int prev[MAX_STATIONS];      // 前驱节点（用于重建路径）
    int visited[MAX_STATIONS];   // 是否已访问（已确定最短路径）
} DijkstraResult;

// 函数声明

/**
 * Dijkstra 算法实现
 *
 * 计算从起点到所有其他节点的最短路径
 *
 * 算法步骤：
 * 1. 初始化所有节点的距离为无穷大，起点距离为0
 * 2. 重复 V-1 次：
 *    a. 找到未访问节点中距离最小的节点 u
 *    b. 标记 u 为已访问
 *    c. 更新 u 的所有邻居的距离
 * 3. 返回结果
 *
 * @param graph      图结构指针
 * @param startVertex 起点节点ID
 * @return           DijkstraResult 结构，包含最短距离和前驱节点
 */
DijkstraResult* dijkstra(Graph* graph, int startVertex);

/**
 * 打印从起点到终点的最短路径
 *
 * 通过 prev[] 数组重建路径并打印
 *
 * @param result     Dijkstra 结果指针
 * @param graph      图结构指针
 * @param startVertex 起点节点ID
 * @param endVertex   终点节点ID
 */
void printShortestPath(DijkstraResult* result, Graph* graph, int startVertex, int endVertex);

/**
 * 打印从起点到所有站点的最短路径
 *
 * 遍历所有节点，打印从起点到每个节点的最短路径
 *
 * @param result     Dijkstra 结果指针
 * @param graph      图结构指针
 * @param startVertex 起点节点ID
 */
void printAllPaths(DijkstraResult* result, Graph* graph, int startVertex);

/**
 * 释放 Dijkstra 结果内存
 *
 * 释放 DijkstraResult 结构占用的内存
 *
 * @param result Dijkstra 结果指针
 */
void freeDijkstraResult(DijkstraResult* result);

#endif
