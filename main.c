#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
 
typedef struct RankElement {
    unsigned int index;
    unsigned long weight;
} RankElement;
 
typedef struct AdjNode {
    int dest;
    unsigned int weight;
    struct AdjNode *next;
} AdjNode;
 
typedef struct AdjList {
    AdjNode *head;
} AdjList;
 
// Rank
void CheckRank(int k, RankElement rank[], int *size, unsigned int index, unsigned long val);
void TopK(RankElement rank[], int size);
void MaxHeapify(RankElement rank[], int size, int n);
void RankSwap(RankElement *rank, int i, int j);
 
// graph & dijkstra
unsigned long AddGraph(AdjList graph[], int d);
void AddEdge(AdjList graph[], int from, int to, unsigned int weight);
void ClearEdges(AdjList graph[], int d);
int PopMin(int *size, int minHeap[], int posInHeap[], unsigned int dist[]);
void DecreaseDistance(int minHeap[], int posInHeap[], unsigned int dist[], int i, unsigned int newDistance);
void MinHeapify(int size, int minHeap[], int posInHeap[], unsigned int dist[], int n);
 
void DynamicGraph(int d, int k);
void StaticGraph(int d, int k);
unsigned long AddGraphStatic(int d, unsigned int graph[d][d]);
int MinDistance(int d, const unsigned int dist[], const bool mst[]);
 
int main() {
    // number of nodes
    int d = 0;
 
    // rank max length
    int k = 0;
 
    // read d. oss: 10 = 2 + 2^3
    char c = getchar_unlocked();
    while (c != ',' && c != ' ') {
        d = (d << 1) + (d << 3) + (c - '0');
        c = getchar_unlocked();
    }
    c = getchar_unlocked();
 
    // read k
    while (c != '\n') {
        k = (k << 1) + (k << 3) + (c - '0');
        c = getchar_unlocked();
    }
 
    if (d >= 100)
        StaticGraph(d, k);
    else
        DynamicGraph(d, k);
 
 
    // TODO: REMOVE
    //printf("%d nodes --- %d rank", d, k);
 
    return 0;
}
 
void StaticGraph(int d, int k) {
    // current index
    int index = 0;
    char c;
 
    int rankSize = 0;
    RankElement rank[k];
 
    //AdjList graph[d];
    //for (int i = 0; i < d; i++)
    //    graph[i].head = NULL;
    unsigned int graph[d][d];
    /*
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            graph[i][j] = __UINT32_MAX__;
        }
    }*/
 
    // reading command
    c = getchar_unlocked();
    while (c != EOF) {
        if (c == 'A') {
            unsigned long tmp = AddGraphStatic(d, graph);
            CheckRank(k, rank, &rankSize, index, tmp);
            index++;
            // TODO: REMOVE
            //unsigned long tot = AddGraph(graph, d);
            //printf("Index: %d __ Tot: %lu\n", index - 1, tmp);
        }
        else if (c == 'T') {
            TopK(rank, rankSize);
            // TODO: REMOVE
            //printf("TopK\n");
            //while(c != '\n')
            //    c = getchar_unlocked();
 
        }
 
        c = getchar_unlocked();
    }
}
 
unsigned long AddGraphStatic(int d, unsigned int graph[d][d]) {
    // readline AggiungiGrafo\n
    char c = getchar_unlocked();
    while(c != '\n')
        c = getchar_unlocked();
 
    c = getchar_unlocked();
    if (d > 1) {
        // read graph
        for (int i = 0; i < d; i++) {
            // ignore first column
            while (c != ',')
                c = getchar_unlocked();
            graph[i][0] = __UINT32_MAX__;
            for (int j = 1; j < d; j++) {
                c = getchar_unlocked();
                if (i == j) {
                    // ignore diagonal
                    while (c != ',' && c != '\n')
                        c = getchar_unlocked();
                    graph[i][j] = __UINT32_MAX__;
                } else {
                    // read weight
                    unsigned int weight = 0;
                    while (c != ',' && c != '\n') {
                        weight = (weight << 1) + (weight << 3) + (c - '0');
                        c = getchar_unlocked();
                    }
                    // add edge
                    if (weight > 0)
                        graph[i][j] = weight;
                    else
                        graph[i][j] = __UINT32_MAX__;
                }
            }
        }
        // calculate dijkstra
        unsigned int dist[d];
        bool mst[d];
        dist[0] = 0;
        mst[0] = false;
        for (int i = 1; i < d; i++) {
            dist[i] = __UINT32_MAX__;
            mst[i] = false;
        }
 
        for (int n = 0; n < d - 1; n++) {
            int imin = MinDistance(d, dist, mst);
 
            mst[imin] = true;
 
            if (dist[imin] != __UINT32_MAX__) {
                for (int j = 0; j < d; j++) {
                    if (mst[j] == false && graph[imin][j] != __UINT32_MAX__ && dist[imin] + graph[imin][j] < dist[j])
                        dist[j] = dist[imin] + graph[imin][j];
                }
            }
 
        }
 
 
        unsigned long tot = 0;
        for (int i = 0; i < d; i++) {
            if (dist[i] < __UINT32_MAX__)
                tot += dist[i];
        }
 
        return tot;
    }
    c = getchar_unlocked();
    return 0;
}
 
int MinDistance(int d, const unsigned int dist[], const bool mst[]) {
    unsigned int min;
    int imin;
    for (int i = 0; i < d; i++) {
        if (mst[i] == false) {
            min = dist[i];
            imin = i;
            for (int j = i + 1; j < d; j++) {
                if (mst[j] == false && dist[j] < min) {
                    min = dist[j];
                    imin = j;
                }
            }
            return imin;
        }
    }
    return -1;
}
 
 
 
 
 
 
 
void DynamicGraph(int d, int k) {
    // current index
    int index = 0;
    char c;
 
    int rankSize = 0;
    RankElement rank[k];
 
    AdjList graph[d];
    for (int i = 0; i < d; i++)
        graph[i].head = NULL;
 
    // reading command
    c = getchar_unlocked();
    while (c != EOF) {
        if (c == 'A') {
            unsigned long tmp = AddGraph(graph, d);
            CheckRank(k, rank, &rankSize, index, tmp);
            index++;
            // TODO: REMOVE
            //unsigned long tot = AddGraph(graph, d);
            //printf("Index: %d __ Tot: %lu\n", index - 1, tmp);
        }
        else if (c == 'T') {
            TopK(rank, rankSize);
            // TODO: REMOVE
            //printf("TopK\n");
            //while(c != '\n')
            //    c = getchar_unlocked();
 
        }
 
        c = getchar_unlocked();
    }
}
 
unsigned long AddGraph(AdjList graph[], int d) {
    // readline AggiungiGrafo\n
    char c = getchar_unlocked();
    while(c != '\n')
        c = getchar_unlocked();
 
    c = getchar_unlocked();
    if (d > 1) {
        // read graph
        for (int i = 0; i < d; i++) {
            // ignore first column
            while (c != ',')
                c = getchar_unlocked();
 
            for (int j = 1; j < d; j++) {
                c = getchar_unlocked();
                if (i == j) {
                    // ignore diagonal
                    while (c != ',' && c != '\n')
                        c = getchar_unlocked();
                } else {
                    // read weight
                    unsigned int weight = 0;
                    while (c != ',' && c != '\n') {
                        weight = (weight << 1) + (weight << 3) + (c - '0');
                        c = getchar_unlocked();
                    }
                    // add edge
                    if (weight > 0)
                        AddEdge(graph, i, j, weight);
                }
            }
        }
        // calculate dijkstra
        int size = d;
        unsigned int dist[d];
        int posInHeap[d];
        int minHeap[d];
        dist[0] = 0;
        minHeap[0] = 0;
        posInHeap[0] = 0;
        for (int i = 1; i < size; i++) {
            dist[i] = __UINT32_MAX__;
            minHeap[i] = i;
            posInHeap[i] = i;
        }
 
        while (size > 0) {
            int imin = PopMin(&size, minHeap, posInHeap, dist);
            AdjNode *adj = graph[imin].head;
            while (adj != NULL) {
            //while (adj != NULL && adj->dest != -1) {
                unsigned int newDistance = __UINT32_MAX__;
                if (dist[imin] < __UINT32_MAX__ - adj->weight)
                    newDistance = dist[imin] + adj->weight;
                if (newDistance < dist[adj->dest]) {
                    DecreaseDistance(minHeap, posInHeap, dist, posInHeap[adj->dest], newDistance);
                }
                adj = adj->next;
            }
        }
 
        ClearEdges(graph, d);
        unsigned long tot = 0;
        for (int i = 0; i < d; i++) {
            if (dist[i] < __UINT32_MAX__)
                tot += dist[i];
        }
 
        return tot;
    }
    c = getchar_unlocked();
    return 0;
}
 
void DecreaseDistance(int minHeap[], int posInHeap[], unsigned int dist[], int i, unsigned int newDistance) {
    dist[minHeap[i]] = newDistance;
    int parent = (i - 1) >> 1;
    while (i > 0 && dist[minHeap[parent]] > dist[minHeap[i]]) {
        //SWAP
        minHeap[i] = minHeap[i] ^ minHeap[parent];
        minHeap[parent] = minHeap[i] ^ minHeap[parent];
        minHeap[i] = minHeap[i] ^ minHeap[parent];
        posInHeap[minHeap[i]] = posInHeap[minHeap[i]] ^ posInHeap[minHeap[parent]];
        posInHeap[minHeap[parent]] = posInHeap[minHeap[i]] ^ posInHeap[minHeap[parent]];
        posInHeap[minHeap[i]] = posInHeap[minHeap[i]] ^ posInHeap[minHeap[parent]];
        i = parent;
        parent = (i - 1) >> 1;
    }
}
 
void MinHeapify(int size, int minHeap[], int posInHeap[], unsigned int dist[], int n) {
    int l = (n << 1) + 1;
    int r = l + 1;
    int imin = n;
    if (l < size && dist[minHeap[l]] < dist[minHeap[imin]])
        imin = l;
    if (r < size && dist[minHeap[r]] < dist[minHeap[imin]])
        imin = r;
    if (imin != n) {
        //SWAP
        minHeap[n] = minHeap[n] ^ minHeap[imin];
        minHeap[imin] = minHeap[n] ^ minHeap[imin];
        minHeap[n] = minHeap[n] ^ minHeap[imin];
        posInHeap[minHeap[n]] = posInHeap[minHeap[n]] ^ posInHeap[minHeap[imin]];
        posInHeap[minHeap[imin]] = posInHeap[minHeap[n]] ^ posInHeap[minHeap[imin]];
        posInHeap[minHeap[n]] = posInHeap[minHeap[n]] ^ posInHeap[minHeap[imin]];
 
        MinHeapify(size, minHeap, posInHeap, dist, imin);
    }
 
}
 
int PopMin(int *size, int minHeap[], int posInHeap[], unsigned int dist[]) {
    if (*size <= 1) {
        (*size)--;
        return 0;
    }
    int min = minHeap[0];
    (*size)--;
    minHeap[0] = minHeap[*size];
    posInHeap[minHeap[0]] = 0;
    MinHeapify(*size, minHeap, posInHeap, dist, 0);
    return min;
}
 
void AddEdge(AdjList graph[], int from, int to, unsigned int weight) {
 
    AdjNode *new = (AdjNode *) malloc(sizeof(AdjNode));
    new->dest = to;
    new->weight = weight;
    new->next = graph[from].head;
    graph[from].head = new;
     /*
    AdjNode *tmp = graph[from].head;
    while (tmp != NULL) {
        if (tmp->dest == -1) {
            tmp->dest = to;
            tmp->weight = weight;
            return;
        }
        tmp = tmp->next;
    }
    AdjNode *new = (AdjNode *) malloc(sizeof(AdjNode));
    new->dest = to;
    new->weight = weight;
    new->next = graph[from].head;
    graph[from].head = new;
     */
}
 
void ClearEdges(AdjList graph[], int d) {
    for (int i = 0; i < d; i++) {
        AdjNode *tmp;
        while (graph[i].head != NULL) {
            tmp = graph[i].head;
            graph[i].head = graph[i].head->next;
            free(tmp);
            //tmp->dest = -1;
        }
    }
}
 
 
 
 
 
void TopK(RankElement rank[], int size) {
    // readline TopK\n
    char c = getchar_unlocked();
    while(c != '\n')
        c = getchar_unlocked();
 
    if (size > 0) {
        printf("%u", rank[0].index);
        for (int i = 1; i < size; i++) {
            printf(" %u", rank[i].index);
        }
    }
    printf("\n");
}
 
void CheckRank(int k, RankElement rank[], int *size, unsigned int index, unsigned long val) {
    if(*size < k) {
        rank[*size].index = index;
        rank[*size].weight = val;
        int i = *size;
        int parent = (i - 1) >> 1;
        while (i > 0 && (rank[parent].weight < rank[i].weight || (rank[parent].weight == rank[i].weight && rank[parent].index < rank[i].index))) {
            RankSwap(rank, parent, i);
            i = parent;
            parent = (i - 1) >> 1;
        }
        (*size)++;
    }
    else if(val < rank[0].weight) {
        rank[0].weight = val;
        rank[0].index = index;
        MaxHeapify(rank, *size, 0);
    }
}
 
void MaxHeapify(RankElement rank[], int size, int n) {
    int l = (n << 1) + 1;
    int r = l + 1;
    int imax = n;
    if (l < size) {
        if (rank[l].weight > rank[imax].weight || (rank[l].weight == rank[imax].weight && rank[l].index > rank[imax].index)) {
            imax = l;
        }
    }
    if (r < size) {
        if (rank[r].weight > rank[imax].weight || (rank[r].weight == rank[imax].weight && rank[r].index > rank[imax].index)) {
            imax = r;
        }
    }
    if (imax != n) {
        RankSwap(rank, n, imax);
        MaxHeapify(rank, size, imax);
    }
}
 
void RankSwap(RankElement *rank, int i, int j) {
    rank[i].index = rank[i].index ^ rank[j].index;
    rank[j].index = rank[i].index ^ rank[j].index;
    rank[i].index = rank[i].index ^ rank[j].index;
 
    rank[i].weight = rank[i].weight ^ rank[j].weight;
    rank[j].weight = rank[i].weight ^ rank[j].weight;
    rank[i].weight = rank[i].weight ^ rank[j].weight;
}