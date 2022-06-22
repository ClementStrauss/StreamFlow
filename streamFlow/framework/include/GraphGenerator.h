#pragma once
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <map>
#include <ostream>

#include "GraphViz.h"
// Meyer singleton
namespace StreamFlow {
class GraphGenerator {
 public:
  static GraphGenerator &instance() {
    static GraphGenerator s;
    return s;
  }  // instance

  GraphGenerator(const GraphGenerator &) = delete;
  GraphGenerator &operator=(const GraphGenerator &) = delete;

  void addNode(std::string nodeName) {
    std::cout << "addNode " + nodeName << std::endl;
    graphvizNodes[nodeName] = graph.add_node(nodeName);
  }

  void addEdge(std::string nodeName1, std::string nodeName2) {
    std::cout << "addEdge " + nodeName1 + " -> " + nodeName2 << std::endl;
    auto n1 = getNode(nodeName1);
    auto n2 = getNode(nodeName2);
    if (n1 != nullptr && n2 != nullptr) graph.add_edge(n1, n2, "1");
  }

  void draw() {
    graph.layout();  // graphviz neato: slowest part of whole program

    //   // clustering has finished by now, update the colours
    //   graph.set_node_attr(node1, "fillcolor", colours[0]);  // NOLINT
    //   graph.set_node_attr(node1, "fillcolor", colours[1]);  // NOLINT
    //   // ...

    graph.render();  // sends svg to stdout
  }

 private:
  GraphGenerator() {}
  ~GraphGenerator() {}

  Agnode_t *getNode(std::string nodeName) {
    if (graphvizNodes.count(nodeName) == 0) throw std::runtime_error(nodeName + " node does not exit");
    return (graphvizNodes[nodeName]);
  }

  std::map<std::string, Agnode_t *> graphvizNodes;
  Graph graph = Graph();

};  // struct singleton_t
}  // namespace StreamFlow