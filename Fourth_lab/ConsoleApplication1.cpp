#include <iostream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

class DirectedAcyclicGraph {
public:
    int nodeCount;
    vector<vector<int>> adjacencyMatrix;

    DirectedAcyclicGraph(int nodes)
        : nodeCount(nodes), adjacencyMatrix(nodes, vector<int>(nodes, 0)) {}

    void connect(int from, int to) {
        adjacencyMatrix[from][to] = 1;
    }

    void disconnect(int from, int to) {
        adjacencyMatrix[from][to] = 0;
    }
};

class WeightedUndirectedGraph {
public:
    int nodeCount;
    vector<vector<int>> adjacencyMatrix;

    WeightedUndirectedGraph(int nodes)
        : nodeCount(nodes), adjacencyMatrix(nodes, vector<int>(nodes, 0)) {}

    void connect(int node1, int node2, int weight) {
        adjacencyMatrix[node1][node2] = weight;
        adjacencyMatrix[node2][node1] = weight;
    }

    void disconnect(int node1, int node2) {
        adjacencyMatrix[node1][node2] = 0;
        adjacencyMatrix[node2][node1] = 0;
    }
};

struct WeightedDirectedEdge {
    int from;
    int to;
    int weight;
};

struct CompareByWeight {
    bool operator()(const WeightedDirectedEdge& a, const WeightedDirectedEdge& b) {
        return a.weight > b.weight;
    }
};

vector<int> topologicalSort(const DirectedAcyclicGraph& graph) {
    int n = graph.nodeCount;
    vector<int> inDegree(n, 0);
    vector<int> sortedOrder;
    stack<int> zeroInDegree;

    for (int col = 0; col < n; ++col)
        for (int row = 0; row < n; ++row)
            if (graph.adjacencyMatrix[row][col])
                ++inDegree[col];

    for (int i = 0; i < n; ++i)
        if (inDegree[i] == 0)
            zeroInDegree.push(i);

    while (!zeroInDegree.empty()) {
        int current = zeroInDegree.top();
        zeroInDegree.pop();
        sortedOrder.push_back(current);

        for (int neighbor = 0; neighbor < n; ++neighbor) {
            if (graph.adjacencyMatrix[current][neighbor]) {
                if (--inDegree[neighbor] == 0)
                    zeroInDegree.push(neighbor);
            }
        }
    }

    if (sortedOrder.size() != static_cast<size_t>(n)) {
        cerr << "Error: Cycle detected in DAG. Topological sort not possible.\n";
        return {};
    }

    return sortedOrder;
}

vector<WeightedDirectedEdge> primMST(const WeightedUndirectedGraph& graph, int startNode) {
    int n = graph.nodeCount;
    if (n < 2) return {};

    vector<bool> visited(n, false);
    vector<WeightedDirectedEdge> mst;
    priority_queue<WeightedDirectedEdge, vector<WeightedDirectedEdge>, CompareByWeight> minHeap;

    minHeap.push({ -1, startNode, 0 });  // Dummy edge to start

    while (!minHeap.empty() && mst.size() < static_cast<size_t>(n - 1)) {
        WeightedDirectedEdge edge = minHeap.top();
        minHeap.pop();

        if (visited[edge.to]) continue;
        visited[edge.to] = true;

        if (edge.from != -1)
            mst.push_back(edge);

        for (int neighbor = 0; neighbor < n; ++neighbor) {
            int weight = graph.adjacencyMatrix[edge.to][neighbor];
            if (!visited[neighbor] && weight > 0)
                minHeap.push({ edge.to, neighbor, weight });
        }
    }

    if (mst.size() != static_cast<size_t>(n - 1)) {
        cerr << "Error: MST could not be formed (disconnected graph).\n";
        return {};
    }

    return mst;
}

int main() {
    
    DirectedAcyclicGraph dag(6);
    dag.connect(0, 2);
    dag.connect(1, 2);
    dag.connect(1, 3);
    dag.connect(2, 4);
    dag.connect(3, 4);
    dag.connect(4, 5);

    vector<int> sortedOrder = topologicalSort(dag);
    if (!sortedOrder.empty()) {
        cout << "Topological Sort: ";
        for (int node : sortedOrder)
            cout << node << " ";
        cout << "\n";
    }

   
    WeightedUndirectedGraph graph(5);
    graph.connect(0, 1, 2);
    graph.connect(0, 3, 6);
    graph.connect(1, 2, 3);
    graph.connect(1, 3, 8);
    graph.connect(1, 4, 5);
    graph.connect(2, 4, 7);
    graph.connect(3, 4, 9);

    
    int startNode;
    cout << "Enter the starting node for Prim's Algorithm (0 to " << graph.nodeCount - 1 << "): ";
    cin >> startNode;

    if (startNode < 0 || startNode >= graph.nodeCount) {
        cout << "Invalid start node! Exiting.\n";
        return 1;
    }

    vector<WeightedDirectedEdge> mst = primMST(graph, startNode);
    if (!mst.empty()) {
        cout << "Minimum Spanning Tree edges:\n";
        for (const auto& edge : mst)
            cout << edge.from << " - " << edge.to << " (weight " << edge.weight << ")\n";
    }

    return 0;
}
