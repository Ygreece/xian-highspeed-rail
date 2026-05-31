#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stations.h"
#include "dijkstra.h"

/**
 * Dijkstra 算法实现
 *
 * 功能：计算从起点到所有其他节点的最短路径
 *
 * 算法步骤：
 * 1. 初始化所有节点的距离为无穷大，起点距离为0
 * 2. 重复以下步骤 V-1 次：
 *    a. 找到未访问节点中距离最小的节点 u
 *    b. 标记 u 为已访问
 *    c. 对于 u 的每个未访问邻居 v：
 *       - 计算新距离 = dist[u] + weight(u,v)
 *       - 如果新距离 < dist[v]，更新 dist[v] 和 prev[v]
 *
 * 时间复杂度：O(V²)（朴素实现）
 * 空间复杂度：O(V)
 *
 * @param graph      图结构指针
 * @param startVertex 起点节点ID
 * @return           DijkstraResult 结构，包含最短距离和前驱节点
 */
DijkstraResult* dijkstra(Graph* graph, int startVertex) {
    // 步骤0：分配结果结构的内存
    DijkstraResult* result = (DijkstraResult*)malloc(sizeof(DijkstraResult));
    if (!result) {
        fprintf(stderr, "内存分配失败\n");
        exit(1);
    }

    // 步骤1：初始化
    // - dist[i] = INF 表示从起点到节点 i 的距离未知
    // - prev[i] = -1 表示节点 i 的前驱节点未知
    // - visited[i] = 0 表示节点 i 未被访问
    for (int i = 0; i < graph->numVertices; i++) {
        result->dist[i] = INF;      // 初始距离为无穷大
        result->prev[i] = -1;       // 前驱节点为空
        result->visited[i] = 0;     // 未访问
    }

    // 起点距离为0（起点到起点的距离为0）
    result->dist[startVertex] = 0;

    // 步骤2：主循环，重复 V-1 次
    // 为什么是 V-1 次？因为每次迭代确定一个节点的最短路径
    // 最多需要确定 V-1 个节点（起点已确定）
    for (int count = 0; count < graph->numVertices - 1; count++) {

        // 步骤2.1：找到未访问节点中距离最小的节点
        // 这是 Dijkstra 算法的核心：贪心选择
        // 每次选择距离起点最近的未访问节点
        int minDist = INF;
        int minVertex = -1;

        for (int v = 0; v < graph->numVertices; v++) {
            // 如果节点 v 未访问，且距离小于当前最小距离
            if (!result->visited[v] && result->dist[v] < minDist) {
                minDist = result->dist[v];
                minVertex = v;
            }
        }

        // 如果没有找到可达节点（所有剩余节点都不可达），退出
        if (minVertex == -1) break;

        // 步骤2.2：标记为已访问
        // 表示节点 minVertex 的最短路径已经确定
        result->visited[minVertex] = 1;

        // 步骤2.3：更新邻居节点的距离（松弛操作）
        // 遍历 minVertex 的所有邻居
        AdjNode* current = graph->adjList[minVertex];
        while (current) {
            int dest = current->dest;      // 邻居节点ID
            int weight = current->weight;  // 边的权重

            // 松弛操作（Relaxation）：
            // 如果通过 minVertex 到达 dest 的距离更短，则更新
            // 条件：
            // 1. dest 未被访问
            // 2. 通过 minVertex 到达 dest 的距离 < 当前已知距离
            if (!result->visited[dest] &&
                result->dist[minVertex] + weight < result->dist[dest]) {
                // 更新最短距离
                result->dist[dest] = result->dist[minVertex] + weight;
                // 记录前驱节点，用于重建路径
                result->prev[dest] = minVertex;
            }

            // 移动到下一个邻居
            current = current->next;
        }
    }

    // 返回结果
    return result;
}

// 打印从起点到终点的最短路径
void printShortestPath(DijkstraResult* result, Graph* graph, int startVertex, int endVertex) {
    if (result->dist[endVertex] == INF) {
        printf("从 %s 到 %s 不可达\n",
               graph->stations[startVertex].name,
               graph->stations[endVertex].name);
        return;
    }

    // 构建路径
    int path[MAX_STATIONS];
    int pathLen = 0;
    int current = endVertex;

    while (current != -1) {
        path[pathLen++] = current;
        current = result->prev[current];
    }

    // 打印路径信息
    printf("\n=== 最短路径 ===\n");
    printf("起点: %s\n", graph->stations[startVertex].name);
    printf("终点: %s\n", graph->stations[endVertex].name);
    printf("经过站点数: %d\n", result->dist[endVertex]);
    printf("路径: ");

    for (int i = pathLen - 1; i >= 0; i--) {
        printf("%s", graph->stations[path[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

// 打印从起点到所有站点的最短路径
void printAllPaths(DijkstraResult* result, Graph* graph, int startVertex) {
    printf("\n========================================\n");
    printf("从 %s 出发到所有站点的最短路径\n", graph->stations[startVertex].name);
    printf("========================================\n\n");

    for (int i = 0; i < graph->numVertices; i++) {
        if (i == startVertex) continue;

        if (result->dist[i] == INF) {
            printf("%-15s: 不可达\n", graph->stations[i].name);
        } else {
            printf("%-15s: %d 站", graph->stations[i].name, result->dist[i]);

            // 打印简要路径
            int path[MAX_STATIONS];
            int pathLen = 0;
            int current = i;

            while (current != -1) {
                path[pathLen++] = current;
                current = result->prev[current];
            }

            printf(" (");
            for (int j = pathLen - 1; j >= 0; j--) {
                printf("%s", graph->stations[path[j]].name);
                if (j > 0) printf("-");
            }
            printf(")\n");
        }
    }
}

// 释放 Dijkstra 结果内存
void freeDijkstraResult(DijkstraResult* result) {
    free(result);
}
