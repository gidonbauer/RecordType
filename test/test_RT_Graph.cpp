#include <gtest/gtest.h>

#include "Graph.hpp"

TEST(test_RT_Graph, InsertNode) {
  RT::Graph<int> graph;
  RT::RecordType<int> rt1(42);

  EXPECT_TRUE(graph.insert_node(rt1)) << "Node should not exist in graph.";
  EXPECT_TRUE(graph.find_index(rt1.id())) << "Node with id " << rt1.id() << " should be in graph.";

  RT::Graph<int>::Node n1;
  EXPECT_NO_THROW(n1 = graph.get(rt1.id());)
      << "Node with id " << rt1.id() << " should be in graph.";

  EXPECT_EQ(rt1.value(), n1.value);
  EXPECT_EQ(rt1.id(), n1.id);
  EXPECT_EQ(rt1.node_type(), n1.node_type);
  EXPECT_EQ(n1.adjacent_nodes.size(), 0ul);
}

TEST(test_RT_Graph, InsertEdge) {
  RT::Graph<int> graph;
  RT::RecordType<int> rt1(42);
  RT::RecordType<int> rt2(12);

  EXPECT_TRUE(graph.insert_node(rt1)) << "Node should not exist in graph.";
  EXPECT_TRUE(graph.insert_node(rt2)) << "Node should not exist in graph.";
  EXPECT_TRUE(graph.insert_edge(rt1.id(), rt2.id())) << "Nodes should existst and edge not.";

  RT::Graph<int>::Node n1 = graph.get(rt1.id());
  RT::Graph<int>::Node n2 = graph.get(rt2.id());
  ASSERT_EQ(n1.adjacent_nodes.size(), 1ul)
      << "Node with id " << n1.id << " should have exactly one adjacent node";
  EXPECT_EQ(n2.adjacent_nodes.size(), 0ul)
      << "Node with id " << n2.id << " should have no adjacent nodes";

  const auto rt2_idx = graph.find_index(rt2.id());
  ASSERT_TRUE(rt2_idx.has_value()) << "Node with id " << rt2.id() << " must be in graph.";
  EXPECT_EQ(n1.adjacent_nodes[0], *rt2_idx)
      << "Node with id " << rt2.id() << " should be the adjacent node to node with id " << n1.id
      << ". Therefore the index of the first node should be in the adjacency list of the latter "
         "node.";
}

TEST(test_RT_Graph, InsertNodeEdge) {
  {
    RT::Graph<int> graph;
    RT::RecordType<int> rt1(42);
    RT::RecordType<int> rt2(12);

    int res = graph.insert_node_edge(rt1, rt2);

    EXPECT_TRUE(static_cast<bool>(res & RT::Graph<int>::INSERT_FROM_NODE))
        << "From node with id " << rt1.id() << " should not have been in the graph.";
    EXPECT_TRUE(static_cast<bool>(res & RT::Graph<int>::INSERT_TO_NODE))
        << "To node with id " << rt2.id() << " should not have been in the graph.";
    EXPECT_TRUE(static_cast<bool>(res & RT::Graph<int>::INSERT_EDGE))
        << "Edge from node with id " << rt1.id() << " to node with id " << rt2.id()
        << " should not have been in the graph.";

    RT::Graph<int>::Node n1 = graph.get(rt1.id());
    RT::Graph<int>::Node n2 = graph.get(rt2.id());
    ASSERT_EQ(n1.adjacent_nodes.size(), 1ul)
        << "Node with id " << n1.id << " should have exactly one adjacent node";
    EXPECT_EQ(n2.adjacent_nodes.size(), 0ul)
        << "Node with id " << n2.id << " should have no adjacent nodes";

    const auto rt2_idx = graph.find_index(rt2.id());
    ASSERT_TRUE(rt2_idx.has_value()) << "Node with id " << rt2.id() << " must be in graph.";
    EXPECT_EQ(n1.adjacent_nodes[0], *rt2_idx)
        << "Node with id " << rt2.id() << " should be the adjacent node to node with id " << n1.id
        << ". Therefore the index of the first node should be in the adjacency list of the latter "
           "node.";
  }

  {
    RT::Graph<int> graph;
    RT::RecordType<int> rt1(42);
    RT::RecordType<int> rt2(12);

    EXPECT_TRUE(graph.insert_node(rt1))
        << "Node with id " << rt1.id() << " should not have been in the graph.";
    int res = graph.insert_node_edge(rt1, rt2);

    EXPECT_FALSE(static_cast<bool>(res & RT::Graph<int>::INSERT_FROM_NODE))
        << "From node with id " << rt1.id() << " should have already been in the graph.";
    EXPECT_TRUE(static_cast<bool>(res & RT::Graph<int>::INSERT_TO_NODE))
        << "To node with id " << rt2.id() << " should not have been in the graph.";
    EXPECT_TRUE(static_cast<bool>(res & RT::Graph<int>::INSERT_EDGE))
        << "Edge from node with id " << rt1.id() << " to node with id " << rt2.id()
        << " should not have been in the graph.";

    RT::Graph<int>::Node n1 = graph.get(rt1.id());
    RT::Graph<int>::Node n2 = graph.get(rt2.id());
    ASSERT_EQ(n1.adjacent_nodes.size(), 1ul)
        << "Node with id " << n1.id << " should have exactly one adjacent node";
    EXPECT_EQ(n2.adjacent_nodes.size(), 0ul)
        << "Node with id " << n2.id << " should have no adjacent nodes";

    const auto rt2_idx = graph.find_index(rt2.id());
    ASSERT_TRUE(rt2_idx.has_value()) << "Node with id " << rt2.id() << " must be in graph.";
    EXPECT_EQ(n1.adjacent_nodes[0], *rt2_idx)
        << "Node with id " << rt2.id() << " should be the adjacent node to node with id " << n1.id
        << ". Therefore the index of the first node should be in the adjacency list of the latter "
           "node.";
  }

  {
    RT::Graph<int> graph;
    RT::RecordType<int> rt1(42);
    RT::RecordType<int> rt2(12);

    EXPECT_TRUE(graph.insert_node(rt2))
        << "Node with id " << rt2.id() << " should not have been in the graph.";
    int res = graph.insert_node_edge(rt1, rt2);

    EXPECT_TRUE(static_cast<bool>(res & RT::Graph<int>::INSERT_FROM_NODE))
        << "From node with id " << rt1.id() << " should not have been in the graph.";
    EXPECT_FALSE(static_cast<bool>(res & RT::Graph<int>::INSERT_TO_NODE))
        << "To node with id " << rt2.id() << " should have already been in the graph.";
    EXPECT_TRUE(static_cast<bool>(res & RT::Graph<int>::INSERT_EDGE))
        << "Edge from node with id " << rt1.id() << " to node with id " << rt2.id()
        << " should not have been in the graph.";

    RT::Graph<int>::Node n1 = graph.get(rt1.id());
    RT::Graph<int>::Node n2 = graph.get(rt2.id());
    ASSERT_EQ(n1.adjacent_nodes.size(), 1ul)
        << "Node with id " << n1.id << " should have exactly one adjacent node";
    EXPECT_EQ(n2.adjacent_nodes.size(), 0ul)
        << "Node with id " << n2.id << " should have no adjacent nodes";

    const auto rt2_idx = graph.find_index(rt2.id());
    ASSERT_TRUE(rt2_idx.has_value()) << "Node with id " << rt2.id() << " must be in graph.";
    EXPECT_EQ(n1.adjacent_nodes[0], *rt2_idx)
        << "Node with id " << rt2.id() << " should be the adjacent node to node with id " << n1.id
        << ". Therefore the index of the first node should be in the adjacency list of the latter "
           "node.";
  }

  {
    RT::Graph<int> graph;
    RT::RecordType<int> rt1(42);
    RT::RecordType<int> rt2(12);

    EXPECT_TRUE(graph.insert_node(rt1))
        << "Node with id " << rt1.id() << " should not have been in the graph.";
    EXPECT_TRUE(graph.insert_node(rt2))
        << "Node with id " << rt2.id() << " should not have been in the graph.";
    EXPECT_TRUE(graph.insert_edge(rt1.id(), rt2.id()))
        << "Edge from node with id " << rt1.id() << " to node with id " << rt2.id()
        << " should not have been in the graph.";

    int res = graph.insert_node_edge(rt1, rt2);

    EXPECT_FALSE(static_cast<bool>(res & RT::Graph<int>::INSERT_FROM_NODE))
        << "From node with id " << rt1.id() << " should have already been in the graph.";
    EXPECT_FALSE(static_cast<bool>(res & RT::Graph<int>::INSERT_TO_NODE))
        << "To node with id " << rt2.id() << " should have already been in the graph.";
    EXPECT_FALSE(static_cast<bool>(res & RT::Graph<int>::INSERT_EDGE))
        << "Edge from node with id " << rt1.id() << " to node with id " << rt2.id()
        << " should have already been in the graph.";

    RT::Graph<int>::Node n1 = graph.get(rt1.id());
    RT::Graph<int>::Node n2 = graph.get(rt2.id());
    ASSERT_EQ(n1.adjacent_nodes.size(), 1ul)
        << "Node with id " << n1.id << " should have exactly one adjacent node";
    EXPECT_EQ(n2.adjacent_nodes.size(), 0ul)
        << "Node with id " << n2.id << " should have no adjacent nodes";

    const auto rt2_idx = graph.find_index(rt2.id());
    ASSERT_TRUE(rt2_idx.has_value()) << "Node with id " << rt2.id() << " must be in graph.";
    EXPECT_EQ(n1.adjacent_nodes[0], *rt2_idx)
        << "Node with id " << rt2.id() << " should be the adjacent node to node with id " << n1.id
        << ". Therefore the index of the first node should be in the adjacency list of the latter "
           "node.";
  }
}

TEST(test_RT_Graph, CountOps) {
  {
    using Type = RT::RecordType<int8_t>;

    Type rt1 = 1;
    Type rt2 = 2;
    Type rt3 = 3;
    Type rt4 = 4;

    [[maybe_unused]] Type rt5 = (rt1 + rt2) * (rt3 + rt4);
    EXPECT_EQ(Type::graph().count_ops(), 3ul)
        << "The code should have 3 operations (2x ADD, 1x MUL).";
    EXPECT_EQ(Type::graph().count_op(RT::NodeType::ADD), 2ul)
        << "The code should have 2 ADD operations.";
    EXPECT_EQ(Type::graph().count_op(RT::NodeType::MUL), 1ul)
        << "The code should have 1 MUL operation.";
    EXPECT_EQ(Type::graph().count_op(RT::NodeType::SQRT), 0ul)
        << "The code should have 0 SQRT operation.";
  }

  {
    using Type = RT::RecordType<int16_t>;

    Type rt1 = 3;
    Type rt2 = 2;
    Type res = 0;

    constexpr size_t n = 100ul;
    for (size_t i = 0ul; i < n; ++i) {
      res += rt1 * rt2;
    }
    EXPECT_EQ(Type::graph().count_ops(), 2 * n)
        << "The code should have " << 2 * n << " operations: for-loop with " << n
        << " iterations and 2 operations (1x ADD, 1x MUL) per iteration.";
    EXPECT_EQ(Type::graph().count_op(RT::NodeType::ADD), n)
        << "The code should have " << n << " ADD operations: for-loop with " << n
        << " iterations and 1 ADD per iteration.";
    EXPECT_EQ(Type::graph().count_op(RT::NodeType::MUL), n)
        << "The code should have " << n << " MUL operations: for-loop with " << n
        << " iterations and 1 MUL per iteration.";
    EXPECT_EQ(Type::graph().count_op(RT::NodeType::SQRT), 0ul)
        << "The code should have 0 SQRT operation.";
  }
}
