#include <iostream>
#include <fstream>

#include "dfs.h"


using namespace std;

int main() {
    Graph<string> a;
    a.loadFromFile("aaa.txt");
    a.addNode("string");
    a.addEdge(1, 4);
    a.saveToFile("vvv.txt");

    return 0;
}