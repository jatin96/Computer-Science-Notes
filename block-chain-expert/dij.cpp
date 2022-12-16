#include <bits/stdc++.h>
using namespace std;
int N, E, src;
const int INF = 1000000007;
// step 1: represent the graph in the form of node and its neighbours(do 2 way mapping for undirected)
vector<pair<int, int>> G[100007];
int dist[];


void dijkstra() {
    // step 2: initialize the dist array to INF.
    for (int i = 0; i < N; i++) dist[i] = INF;

    // step 3: make a priority queue having a pair <current least weight, node> minHeap
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> pq;

    // step 4: push the <0, src> and initialize the dist[src] = 0
    pq.push({0, src});
    dist[src] = 0;

    while (!pq.empty()) {
        int currNode = pq.top().second;
        int currWt = pq.top().first;
        pq.pop();

        // step 5: check dist[currNode] < currWt
        if (dist[currNode] < currWt) continue;

        for (int i = 0; i < G[currNode].size(); i++) {
            int v = G[currNode][i].first;
            int w = G[currNode][i].second;
            // step 6: check if dist[currNode] + w < dist[v]
            if (dist[currNode] + w < dist[v]) {
                dist[v] = dist[currNode] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

int main() {
    cin >> N >> E;
    cin >> src;
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        G[u].push_back({v, w});
        // if this is undirected, then only do below
        G[v].push_back({u, w});
    }

    dijkstra();
}