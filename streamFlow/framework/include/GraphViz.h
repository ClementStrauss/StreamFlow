// from https://codereview.stackexchange.com/questions/236073/c-wrapper-for-graphviz-library
#pragma once
// sudo apt-get install libgraphviz-dev
#include <graphviz/cgraph.h>  // these 2 includes are the graphiz cgraph lib
#include <graphviz/gvc.h>

#include <array>
#include <fstream>
#include <iostream>

namespace StreamFlow {

class Graph {
 public:
  Graph() {
    gvc_ = gvContext();

    static const char* fargv[] = {"dot", "-Tpng", "-ograph.png"};  // NOLINT
    gvParseArgs(gvc_, 3, (char**)fargv);                           // NOLINT

    graph_ = agopen((char*)"G", Agdirected, nullptr);  // NOLINT

    // clang-format off
    // set_graph_attr_def("splines",   "none");
    // set_graph_attr_def("ratio",     "1.25");

    // set_node_attr_def("tooltip",    "");
    //set_node_attr_def("fillcolor",  "grey");
    set_node_attr_def("shape",      "box");
    //set_node_attr_def("width",      "0.5");
    // set_node_attr_def("penwidth",   "0");

    // set_edge_attr_def("weight",     "1");
    // clang-format on
  }

  Graph(const Graph& other) = delete;
  Graph& operator=(const Graph& other) = delete;

  Graph(Graph&& other) = delete;
  Graph& operator=(Graph&& other) = delete;

  ~Graph() {
    if (graph_ != nullptr) {
      if (gvc_ != nullptr) gvFreeLayout(gvc_, graph_);
      agclose(graph_);
    }
    if (gvc_ != nullptr) gvFreeContext(gvc_);
  }

  void set_graph_attr_def(std::string name, std::string value) {
    agattr(graph_, AGRAPH, (char*)name.c_str(), (char*)value.c_str());  // NOLINT
  }

  void set_node_attr_def(std::string name, std::string value) {
    agattr(graph_, AGNODE, (char*)name.c_str(), (char*)value.c_str());  // NOLINT
  }

  void set_edge_attr_def(std::string name, std::string value) {
    agattr(graph_, AGEDGE, (char*)name.c_str(), (char*)value.c_str());  // NOLINT
  }

  void set_node_attr(Agnode_t* node, std::string name, std::string value) {  // NOLINT
    agsafeset(node, name.data(), value.data(), (char*)"");                   // NOLINT
  }

  void set_edge_attr(Agedge_t* edge, std::string name, std::string value) {  // NOLINT
    agsafeset(edge, (char*)name.c_str(), (char*)value.c_str(), (char*)"");   // NOLIN
  }

  Agedge_t* add_edge(Agnode_t* src, Agnode_t* dest, std::string edgeName, std::string weight_str) {
    auto edge = agedge(graph_, src, dest, edgeName.data(), 1);
    // set_edge_attr(edge, "weight", weight_str);
    set_edge_attr(edge, "label", edgeName);
    // set_edge_attr(edge, "color", "red");

    return edge;
  }

  Agnode_t* add_node(std::string node_name, std::string doc) {
    auto node = agnode(graph_, (char*)node_name.c_str(), 1);  // NOLINT
    set_node_attr(node, "label", doc);
    return node;
  }

  void layout() {
    std::cout << "layout" << std::endl;

    // Write the graph according to -T and -o options
    gvLayoutJobs(gvc_, graph_);
  }

  void render() {
    // gvRenderJobs(gvc_, graph_);
    std::cout << "render" << std::endl;

    gvRenderFilename(gvc_, graph_, "dot", "test.dot");
    gvFreeLayout(gvc_, graph_);

    // Free graph structures
    agclose(graph_);

    // close output file, free context, and return number of errors
    gvFreeContext(gvc_);
  }

 private:
  Agraph_t* graph_ = nullptr;
  GVC_t* gvc_ = nullptr;
};

static constexpr const size_t max_colours = 30;

static constexpr const std::array<std::string_view, max_colours> colours = {
    "blue",         "green",         "red",       "gold",         "black",          "magenta",    "brown",      "pink",
    "khaki",        "cyan",          "tan",       "blueviolet",   "burlywood",      "cadetblue",  "chartreuse", "chocolate",
    "coral",        "darkgoldenrod", "darkgreen", "darkkhaki",    "darkolivegreen", "darkorange", "darkorchid", "darksalmon",
    "darkseagreen", "dodgerblue",    "lavender",  "mediumpurple", "plum",           "yellow"};
}  // namespace StreamFlow

// int main() {
//   auto graph = Graph{};  // initializes instace of a graphviz graph

//   // build node list by loading data from a mongo database

//   auto node1 = graph.add_node("1");
//   auto node2 = graph.add_node("2");
//   // ...  10,000 + nodes  (that's all neato can handle, we would like more)

//   // 2.3 is the "weight" and it's a double in our code but graphiz wants a string
//   // there is a reason that the Graph::add_edge API takes the string
//   // the double -> string conversion is quite expensive (we use Ryu)
//   // and we need it twice. Once for graphviz and once for the cluster
//   // both as a string
//   graph.add_edge(node1, node2, "2.3");
//   //... 2 - 25 million edges

//   // run clustering algorithm on separate thread

//   graph.layout();  // graphviz neato: slowest part of whole program

//   // clustering has finished by now, update the colours
//   graph.set_node_attr(node1, "fillcolor", colours[0]);  // NOLINT
//   graph.set_node_attr(node1, "fillcolor", colours[1]);  // NOLINT
//   // ...

//   graph.render();  // sends svg to stdout
// }