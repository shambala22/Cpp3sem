#include <iostream>
#include <fstream>

#include "dfs.h"


using namespace std;

void start(Graph<string>::NodeHandle const& v) {
    cout<<"start "<<v+1<<"\n";
}

void discover(Graph<string>::NodeHandle const& v) {
    cout<<"discover "<<v+1<<"\n";
}

void endv(Graph<string>::NodeHandle const& v) {
    cout<<"end "<<v+1<<"\n";
}

int main() {
    Graph<string> a;
    a.loadFromFile("aaa.txt");

    Graph<string>::NodeVisitor s;
    s = start;
    Graph<string>::NodeVisitor d;
    d = discover;
    Graph<string>::NodeVisitor e;
    e = endv;
    a.dfs(s, e, d);
    a.saveToFile("vvv.txt");
    return 0;
}

