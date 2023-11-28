#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

#include "graph.h"

#include <set>

using namespace std;

Graph<int,char> makeDefaultGraph()
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');
    return graph;
}

set<char> getVertexListElements(const Graph<int,char>::VertexList& vertexList)
{
    set<char> vertexSet;
    for (auto it = vertexList.begin(); it != vertexList.end(); ++it) {
        vertexSet.insert(**it);
	}
    return vertexSet;
}

set<char> getEdgeListElements(const Graph<int,char>::EdgeList& edgeList)
{
    set<char> edgeSet;
    for (auto it = edgeList.begin(); it != edgeList.end(); ++it) {
        edgeSet.insert(**it);
	}
    return edgeSet;
}

TEST_CASE("Insert vertex")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');

    REQUIRE(*v1 == 'a');
    REQUIRE(*v2 == 'b');
    REQUIRE(*v3 == 'c');
}

TEST_CASE("Insert edge")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');
    REQUIRE(*e1 == 'm');
    REQUIRE(*e2 == 'n');
}

TEST_CASE("Erase vertex")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');

    graph.eraseVertex(v2);

	auto vertexSet = getVertexListElements(graph.vertices());
    REQUIRE(vertexSet.size() == 2);
    REQUIRE(vertexSet.count('a') == 1);
    REQUIRE(vertexSet.count('c') == 1);

	auto edgeSet = getEdgeListElements(graph.edges());
    REQUIRE(edgeSet.size() == 1);
    REQUIRE(edgeSet.count('n') == 1);
}

TEST_CASE("Erase edge")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');

    graph.eraseEdge(e1);

	auto vertexSet = getVertexListElements(graph.vertices());
    REQUIRE(vertexSet.size() == 3);
    REQUIRE(vertexSet.count('a') == 1);
    REQUIRE(vertexSet.count('b') == 1);
    REQUIRE(vertexSet.count('c') == 1);

	auto edgeSet = getEdgeListElements(graph.edges());
    REQUIRE(edgeSet.size() == 1);
    REQUIRE(edgeSet.count('n') == 1);
}

TEST_CASE("Vertex iterator")
{
	auto graph = makeDefaultGraph();
    auto vertexSet = getVertexListElements(graph.vertices());
    REQUIRE(vertexSet.size() == 3);
    REQUIRE(vertexSet.count('a') == 1);
    REQUIRE(vertexSet.count('b') == 1);
    REQUIRE(vertexSet.count('c') == 1);
}

TEST_CASE("Edge iterator")
{
	auto graph = makeDefaultGraph();
    auto edgeSet = getEdgeListElements(graph.edges());
    REQUIRE(edgeSet.size() == 2);
    REQUIRE(edgeSet.count('m') == 1);
    REQUIRE(edgeSet.count('n') == 1);
}

TEST_CASE("Vertex is adjacent to")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');
    REQUIRE_FALSE(v1.isAdjacentTo(v1));
    REQUIRE(v1.isAdjacentTo(v2));
    REQUIRE(v1.isAdjacentTo(v3));
    REQUIRE(v2.isAdjacentTo(v1));
    REQUIRE_FALSE(v2.isAdjacentTo(v2));
    REQUIRE_FALSE(v2.isAdjacentTo(v3));
    REQUIRE(v3.isAdjacentTo(v1));
    REQUIRE_FALSE(v3.isAdjacentTo(v2));
    REQUIRE_FALSE(v3.isAdjacentTo(v3));
}

TEST_CASE("Vertex incident edges")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');

	auto edgeSet = getEdgeListElements(v1.incidentEdges());
    REQUIRE(edgeSet.size() == 2);
    REQUIRE(edgeSet.count('m') == 1);
    REQUIRE(edgeSet.count('n') == 1);

	edgeSet = getEdgeListElements(v2.incidentEdges());
    REQUIRE(edgeSet.size() == 1);
    REQUIRE(edgeSet.count('m') == 1);
}

TEST_CASE("Edge end vertices")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');

	auto vertexSet = getVertexListElements(e1.endVertices());
    REQUIRE(vertexSet.size() == 2);
    REQUIRE(vertexSet.count('a') == 1);
    REQUIRE(vertexSet.count('b') == 1);

	vertexSet = getVertexListElements(e2.endVertices());
    REQUIRE(vertexSet.size() == 2);
    REQUIRE(vertexSet.count('a') == 1);
    REQUIRE(vertexSet.count('c') == 1);
}

TEST_CASE("Edge opposite vertex")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');
    REQUIRE(*e1.opposite(v1) == 'b');
    REQUIRE(*e1.opposite(v2) == 'a');
    REQUIRE(*e2.opposite(v1) == 'c');
    REQUIRE(*e2.opposite(v3) == 'a');
}

TEST_CASE("Edge is incident on")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');
    REQUIRE(e1.isIncidentOn(v1));
    REQUIRE(e1.isIncidentOn(v2));
    REQUIRE_FALSE(e1.isIncidentOn(v3));
    REQUIRE(e2.isIncidentOn(v1));
    REQUIRE(e2.isIncidentOn(v3));
    REQUIRE_FALSE(e2.isIncidentOn(v2));
}
