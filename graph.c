#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stations.h"

/**
 * 创建图结构
 *
 * 分配图结构的内存并初始化
 * - numVertices 初始化为 0
 * - adjList 数组初始化为 NULL（空邻接表）
 *
 * @return 图结构指针
 */
Graph* createGraph() {
    // 分配图结构内存
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) {
        fprintf(stderr, "内存分配失败\n");
        exit(1);
    }

    // 初始化站点数量为0
    graph->numVertices = 0;

    // 初始化邻接表数组为空
    for (int i = 0; i < MAX_STATIONS; i++) {
        graph->adjList[i] = NULL;
    }

    return graph;
}

/**
 * 添加站点到图中
 *
 * 将站点信息添加到图的站点数组中
 *
 * @param graph 图结构指针
 * @param id    站点ID
 * @param name  站点名称
 * @param code  电报码
 */
void addStation(Graph* graph, int id, const char* name, const char* code) {
    // 检查ID是否超出范围
    if (id >= MAX_STATIONS) {
        fprintf(stderr, "站点ID超出范围: %d\n", id);
        return;
    }

    // 设置站点信息
    graph->stations[id].id = id;
    strncpy(graph->stations[id].name, name, MAX_NAME_LEN - 1);
    strncpy(graph->stations[id].code, code, 9);

    // 更新站点数量（取最大值）
    if (id >= graph->numVertices) {
        graph->numVertices = id + 1;
    }
}

/**
 * 添加边到图中（无向图）
 *
 * 在两个站点之间添加双向连接
 * 每条边的权重为1（表示经过一个站点）
 *
 * 图的邻接表表示：
 * - 每个节点维护一个链表
 * - 链表中的每个节点表示一条边
 * - 新节点插入到链表头部（O(1) 时间复杂度）
 *
 * @param graph 图结构指针
 * @param src   源站点ID
 * @param dest  目标站点ID
 */
void addEdge(Graph* graph, int src, int dest) {
    // 添加 src -> dest 的边
    AdjNode* newNode = (AdjNode*)malloc(sizeof(AdjNode));
    if (!newNode) {
        fprintf(stderr, "内存分配失败\n");
        exit(1);
    }
    newNode->dest = dest;
    newNode->weight = 1;  // 站点数权重为1
    newNode->next = graph->adjList[src];  // 插入到链表头部
    graph->adjList[src] = newNode;

    // 添加 dest -> src 的边（无向图）
    newNode = (AdjNode*)malloc(sizeof(AdjNode));
    if (!newNode) {
        fprintf(stderr, "内存分配失败\n");
        exit(1);
    }
    newNode->dest = src;
    newNode->weight = 1;
    newNode->next = graph->adjList[dest];  // 插入到链表头部
    graph->adjList[dest] = newNode;
}

/**
 * 从文件加载站点和线路数据
 *
 * 文件格式：
 * - 站点行: S <id> <name> <code>
 * - 线路行: E <src_id> <dest_id>
 * - 注释行: 以 # 开头
 * - 空行: 跳过
 *
 * 示例文件内容：
 * ```
 * # 西安高铁网络
 * S 0 西安 XAY
 * S 1 西安北 XAY
 * E 0 1
 * ```
 *
 * @param graph    图结构指针
 * @param filename 文件名
 */
void loadStationsFromFile(Graph* graph, const char* filename) {
    // 打开文件
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        exit(1);
    }

    char line[256];
    // 逐行读取文件
    while (fgets(line, sizeof(line), file)) {
        // 跳过空行和注释行
        if (line[0] == '\n' || line[0] == '#') continue;

        // 解析站点行
        if (line[0] == 'S') {
            int id;
            char name[MAX_NAME_LEN];
            char code[10];
            // 使用 sscanf 解析格式化的输入
            if (sscanf(line, "S %d %s %s", &id, name, code) == 3) {
                addStation(graph, id, name, code);
            }
        }
        // 解析线路行
        else if (line[0] == 'E') {
            int src, dest;
            // 使用 sscanf 解析格式化的输入
            if (sscanf(line, "E %d %d", &src, &dest) == 2) {
                addEdge(graph, src, dest);
            }
        }
    }

    // 关闭文件
    fclose(file);
    printf("成功加载 %d 个站点\n", graph->numVertices);
}

/**
 * 释放图结构内存
 *
 * 释放图中所有动态分配的内存：
 * 1. 释放邻接表中的所有节点
 * 2. 释放图结构本身
 *
 * 注意：必须按照正确的顺序释放内存，避免内存泄漏
 *
 * @param graph 图结构指针
 */
void freeGraph(Graph* graph) {
    // 释放邻接表中的所有节点
    for (int i = 0; i < graph->numVertices; i++) {
        AdjNode* current = graph->adjList[i];
        // 遍历链表，逐个释放节点
        while (current) {
            AdjNode* temp = current;
            current = current->next;
            free(temp);
        }
    }

    // 释放图结构本身
    free(graph);
}
