#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stations.h"
#include "dijkstra.h"

/**
 * 打印使用说明
 *
 * 显示程序的用法和示例
 */
void printUsage() {
    printf("\n西安高铁最短路径查询系统\n");
    printf("=======================\n");
    printf("这是一个 Dijkstra 算法的学习项目\n");
    printf("\n用法: ./railway [选项]\n");
    printf("选项:\n");
    printf("  -a                 显示从西安到所有站点的最短路径\n");
    printf("  -s <站名>          显示从指定站点到所有站点的最短路径\n");
    printf("  -f <起点> <终点>   显示两个站点之间的最短路径\n");
    printf("  -l                 列出所有站点\n");
    printf("  -h                 显示帮助信息\n");
    printf("\n示例:\n");
    printf("  ./railway -a              # 从西安到所有站点\n");
    printf("  ./railway -f 西安 成都     # 西安到成都\n");
    printf("  ./railway -s 北京          # 从北京出发\n");
    printf("\n学习要点:\n");
    printf("  1. 图的邻接表表示\n");
    printf("  2. Dijkstra 算法的实现\n");
    printf("  3. 最短路径的重建\n");
}

/**
 * 列出所有站点
 *
 * 显示图中所有站点的ID、名称和电报码
 *
 * @param graph 图结构指针
 */
void listAllStations(Graph* graph) {
    printf("\n所有站点列表 (%d 个):\n", graph->numVertices);
    printf("=======================\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%3d. %-15s (%s)\n",
               i + 1,
               graph->stations[i].name,
               graph->stations[i].code);
    }
}

/**
 * 根据名称查找站点ID
 *
 * 在图中查找指定名称的站点，返回其ID
 *
 * @param graph 图结构指针
 * @param name  站点名称
 * @return      站点ID，如果未找到返回-1
 */
int findStationByName(Graph* graph, const char* name) {
    for (int i = 0; i < graph->numVertices; i++) {
        if (strcmp(graph->stations[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * 主函数
 *
 * 程序入口，处理命令行参数并调用相应功能
 *
 * 命令行参数：
 *   -a                 显示从西安到所有站点的最短路径
 *   -s <站名>          显示从指定站点到所有站点的最短路径
 *   -f <起点> <终点>   显示两个站点之间的最短路径
 *   -l                 列出所有站点
 *   -h                 显示帮助信息
 *
 * @param argc 参数数量
 * @param argv 参数数组
 * @return     退出码（0表示成功）
 */
int main(int argc, char* argv[]) {
    // 步骤1：创建图结构
    Graph* graph = createGraph();

    // 步骤2：从文件加载站点和线路数据
    printf("正在加载站点数据...\n");
    loadStationsFromFile(graph, "data.txt");

    // 步骤3：检查命令行参数
    if (argc < 2) {
        printUsage();
        freeGraph(graph);
        return 0;
    }

    // 步骤4：设置默认起点（西安）
    int startVertex = findStationByName(graph, "西安");
    if (startVertex == -1) {
        fprintf(stderr, "错误: 找不到起点站 '西安'\n");
        freeGraph(graph);
        return 1;
    }

    // 步骤5：解析命令行参数并执行相应功能
    if (strcmp(argv[1], "-a") == 0) {
        // 选项 -a：显示从西安到所有站点的最短路径
        printf("\n正在计算从西安到所有站点的最短路径...\n");

        // 调用 Dijkstra 算法
        DijkstraResult* result = dijkstra(graph, startVertex);

        // 打印所有路径
        printAllPaths(result, graph, startVertex);

        // 释放结果内存
        freeDijkstraResult(result);

    } else if (strcmp(argv[1], "-s") == 0 && argc >= 3) {
        // 选项 -s：从指定站点出发
        int start = findStationByName(graph, argv[2]);
        if (start == -1) {
            fprintf(stderr, "错误: 找不到站点 '%s'\n", argv[2]);
            freeGraph(graph);
            return 1;
        }

        printf("\n正在计算从 %s 到所有站点的最短路径...\n", argv[2]);

        // 调用 Dijkstra 算法
        DijkstraResult* result = dijkstra(graph, start);

        // 打印所有路径
        printAllPaths(result, graph, start);

        // 释放结果内存
        freeDijkstraResult(result);

    } else if (strcmp(argv[1], "-f") == 0 && argc >= 4) {
        // 选项 -f：显示两个站点之间的最短路径
        int start = findStationByName(graph, argv[2]);
        int end = findStationByName(graph, argv[3]);

        if (start == -1) {
            fprintf(stderr, "错误: 找不到起点站 '%s'\n", argv[2]);
            freeGraph(graph);
            return 1;
        }
        if (end == -1) {
            fprintf(stderr, "错误: 找不到终点站 '%s'\n", argv[3]);
            freeGraph(graph);
            return 1;
        }

        printf("\n正在计算从 %s 到 %s 的最短路径...\n", argv[2], argv[3]);

        // 调用 Dijkstra 算法
        DijkstraResult* result = dijkstra(graph, start);

        // 打印最短路径
        printShortestPath(result, graph, start, end);

        // 释放结果内存
        freeDijkstraResult(result);

    } else if (strcmp(argv[1], "-l") == 0) {
        // 选项 -l：列出所有站点
        listAllStations(graph);

    } else if (strcmp(argv[1], "-h") == 0) {
        // 选项 -h：显示帮助
        printUsage();

    } else {
        // 未知选项
        fprintf(stderr, "错误: 未知选项 '%s'\n", argv[1]);
        printUsage();
        freeGraph(graph);
        return 1;
    }

    // 步骤6：释放图结构内存
    freeGraph(graph);
    return 0;
}
