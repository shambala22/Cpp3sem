#ifndef DFS_H_INCLUDED
#define DFS_H_INCLUDED
#include <vector>
#include <functional>
#include <stack>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

template <typename NodePayload>
class Graph {
public:
    typedef unsigned NodeHandle;
    typedef std::pair<NodeHandle, NodeHandle> EdgeHandle;
    typedef std::function<void(NodeHandle const &)> NodeVisitor;
    typedef std::function<void(EdgeHandle const &)> EdgeVisitor;
public:
    Graph();
    ~Graph();
    void loadFromFile(std::string const & filename); /* loads graph from file in format:
                                                        <number of vertexes> <number of edges>
                                                         {payloads for each edge}
                                                         <begin for 1st edge> <end for 1st edge>
                                                         ...
                                                         <begin for last edge> <end for last edge> */
    void saveToFile(std::string const & filename); // saves graph to file in same format
    NodeHandle addNode(NodePayload nodePayload); // adds new vertex to graph
    void addEdge(NodeHandle const & a, NodeHandle const & b); // adds new edge with begin at "a" and end at "b" to graph
    void forEachNode(std::function<void(NodeHandle const &)> const & visitor) const; // executes "visitor" function for each node in graph
    size_t getNodesCount() const; // returns number of vertexes
    void forEachEdge(NodeHandle const & source, EdgeVisitor const & visitor); // executes "visitor" function for each edge, which begins in "source" edge
    NodeHandle move(NodeHandle const & origin, EdgeHandle const & edge); // returns end of "edge" with begin at "origin", if it's possible
    NodePayload & operator[](NodeHandle const & node); // returns payload of vertex "node"
    void dfs(NodeVisitor const & startNode, NodeVisitor const & endNode, NodeVisitor const & discoverNode); // execute depth-first search for graph
private:
    std::vector<std::vector<NodeHandle>> edges;
    std::vector<NodePayload> payload;
    unsigned long edges_counter;
};

template <typename NodePayload>
Graph<NodePayload>::Graph() {
    this->edges_counter = 0;
}

template <typename NodePayload>
Graph<NodePayload>::~Graph() {}

template <typename NodePayload>
void Graph<NodePayload>::saveToFile(std::string const & filename) {
    freopen(filename.data(), "w", stdout);
    cout<< getNodesCount()<<" "<<this->edges_counter<<"\n";
    for (int i = 0; i< getNodesCount(); i++) {
        cout<<payload[i]<<" ";
    }
    cout<<"\n";
    for (int i  = 0; i< getNodesCount(); i++) {
        for (NodeHandle u : edges[i]) {
            if (i<=u) {
                cout << (i + 1) << " " << (u + 1) << "\n";
            }
        }
    }
}
template <typename NodePayload>
void Graph<NodePayload>::loadFromFile(std::string const & filename) {
    freopen(filename.data(), "r", stdin);
    int n = 0;
    int m = 0;
    cin>>n>>m;
    NodePayload p;
    for (int i = 0; i<n; i++) {
        cin>>p;
        addNode(p);
    }
    for (int i = 0; i<m; i++) {
        NodeHandle u;
        NodeHandle v;
        cin>>u>>v;
        addEdge(u, v);
    }
}

template <typename NodePayload>
typename Graph<NodePayload>::NodeHandle Graph<NodePayload>::addNode(NodePayload nodePayload) {
    std::vector<NodeHandle> a;
    this->edges.push_back(a);
    this->payload.push_back(nodePayload);
    return this->edges.size();
}

template <typename NodePayload>
void Graph<NodePayload>::addEdge(NodeHandle const& first, NodeHandle const& second) {
    if (first-1> getNodesCount() || second-1> getNodesCount()) {
        throw new std::exception();
    }
    this->edges[first-1].push_back(second-1);
    edges_counter++;
}

template <typename NodePayload>
size_t Graph<NodePayload>::getNodesCount() const {
    return this->edges.size();
}

template <typename NodePayload>
Graph<int>::NodeHandle Graph<NodePayload>::move(NodeHandle const & origin, EdgeHandle const & edge) {
    if (origin==edge.first) {
        return edge.second;
    } else return origin;
}

template <typename NodePayload>
void Graph<NodePayload>::forEachNode(std::function<void(NodeHandle const &)> const &visitor) const {
    for (size_t i = 0; i<edges.size(); i++) {
        visitor(i);
    }
}

template <typename NodePayload>
void Graph<NodePayload>::forEachEdge(NodeHandle const &source, EdgeVisitor const &visitor) {
    for (size_t i =0; i<edges[source].size(); i++) {
        visitor((EdgeHandle) std::make_pair(source, edges[source][i]));
    }
}

template <typename NodePayload>
NodePayload& Graph<NodePayload>::operator[](NodeHandle const &node) {
    return payload[node];
}

template <typename NodePayload>
void Graph<NodePayload>::dfs(NodeVisitor const &startNode, NodeVisitor const &endNode, NodeVisitor const &discoverNode) {
    bool* visited = new bool[getNodesCount()];
    std::fill_n(visited, getNodesCount(), false);
    int* parent = new int[getNodesCount()];
    std::fill_n(parent, getNodesCount(), -1);
    NodeHandle vertex;
    for (NodeHandle v = 0; v<edges.size(); v++) {
        vertex = v;
        if (!visited[vertex]) {
            std::stack<NodeHandle> stack;
            stack.push(vertex);
            discoverNode(vertex);
            while (!stack.empty()) {
                vertex = stack.top();
                stack.pop();
                startNode(vertex);
                bool first = true;
                std::stack<NodeHandle> extraStack;
                for (auto i = edges[vertex].begin(); i != edges[vertex].end(); i++) {
                    discoverNode(*i);
                    if (!visited[*i]) {
                        visited[*i] = true;
                        extraStack.push(*i);
                    }
                }
                while (!extraStack.empty()) {
                    NodeHandle v = extraStack.top();
                    if (first) {
                        parent[v] = vertex;
                        first = false;
                    }
                    stack.push(v);
                    extraStack.pop();
                }
                if (first) {
                    parent[0] = -1;
                    while (vertex!=-1) {
                        endNode(vertex);
                        vertex = parent[vertex];
                    }
                }
            }
        }
    }

    delete[] visited;

}
#endif // DFS_H_INCLUDED