#pragma once

#include <unordered_map>
#include <vector>

template <typename VertexElement, typename EdgeElement>
class Graph {
public:

    class Vertex;
    class VertexItor;
    class Edge;
    class EdgeItor;
    typedef std::unordered_map<int,std::vector<int>> AdjacencyList;
    typedef std::unordered_map<int,Vertex> VertexMap;
    typedef std::unordered_map<int,Edge> EdgeMap;

    class VertexList {
    public:
        VertexItor begin() const {
            return VertexItor(vertexMap.begin());
        }

        VertexItor end() const {
            return VertexItor(vertexMap.end());
        }

    private:
        const VertexMap vertexMap;
        friend Graph;

        VertexList(const VertexMap& _vertexMap) : vertexMap(_vertexMap) {}
    };

    class EdgeList {
    public:
        EdgeItor begin() const {
            return EdgeItor(edgeMap.begin());
        }

        EdgeItor end() const {
            return EdgeItor(edgeMap.end());
        }

    private:
        const EdgeMap edgeMap;
        friend Graph;

        EdgeList(const EdgeMap& _edgeMap) : edgeMap(_edgeMap) {}
    };

    class VertexItor {
    public:
        const Vertex& operator*() const {
            return it->second;
        }

        VertexItor& operator++() {
            ++it;
            return *this;
        }

        VertexItor operator++(int) {
            VertexItor temp = *this;
            ++it;
            return temp;
        }

        bool operator!=(const VertexItor& other) const {
            return it != other.it;
        }

    private:
        typename VertexMap::const_iterator it;
        friend VertexList;

        VertexItor(typename VertexMap::const_iterator _it) : it(_it) {}
    };

    class EdgeItor {
    public:
        const Edge& operator*() const {
            return it->second;
        }

        EdgeItor& operator++() {
            ++it;
            return *this;
        }

        EdgeItor operator++(int) {
            EdgeItor temp = *this;
            ++it;
            return temp;
        }

        bool operator!=(const EdgeItor& other) const {
            return it != other.it;
        }

    private:
        typename EdgeMap::const_iterator it;
        friend EdgeList;

        EdgeItor(typename EdgeMap::const_iterator _it) : it(_it) {}
    };

    class Vertex {
    public:
        bool isAdjacentTo(const Vertex& v) const {
            for (int edgeID : g.adjacencyList.at(id)) {
                Edge edge = g.edgeMap.at(edgeID);
                if ((id == edge.startVertex && v.id == edge.endVertex) ||
                    (id == edge.endVertex && v.id == edge.startVertex)) {
                    return true;
                }
            }
            return false;
        }

        EdgeList incidentEdges() const {
            EdgeMap incidentEdges;
            for (int edgeID : g.adjacencyList.at(id)) {
                Edge edge = g.edgeMap.at(edgeID);
                incidentEdges.emplace(edgeID, edge);
            }
            return EdgeList(incidentEdges);
        }

        const VertexElement& operator*() const {
            return element;
        }

    private:
        VertexElement element;
        int id;
        const Graph& g;
        friend Graph;

        Vertex(const VertexElement& _element, int _id, const Graph& _g) : element(_element), id(_id), g(_g) {}
    };

    class Edge {
    public:
        VertexList endVertices() const {
            VertexMap endV;
            endV.emplace(startVertex, g.vertexMap.at(startVertex));
            endV.emplace(endVertex, g.vertexMap.at(endVertex));
            return VertexList(endV);
        }

        const Vertex& opposite(const Vertex& v) const {
            return g.vertexMap.at(v.id == startVertex ? endVertex : startVertex);
        }

        bool isIncidentOn(const Vertex& v) const {
            return v.id == startVertex || v.id == endVertex;
        }

        const EdgeElement& operator*() const {
            return element;
        }

    private:
        EdgeElement element;
        int id;
        int startVertex, endVertex;
        const Graph& g;
        friend Graph;

        Edge(const EdgeElement& _element, int _id, int _startVertex, int _endVertex, const Graph& _g) :
            element(_element), id(_id), startVertex(_startVertex), endVertex(_endVertex), g(_g) {}
    };

    const Vertex& insertVertex(const VertexElement& element) {
        Vertex v(element, nextVertexID++, *this);
        vertexMap.emplace(v.id, v);
        adjacencyList[v.id] = {};
        return vertexMap.at(v.id);
    }

    const Edge& insertEdge(const Vertex& v, const Vertex& u, const EdgeElement& element) {
        if (vertexMap.count(v.id) == 0 || vertexMap.count(u.id) == 0) {
            throw std::runtime_error("Cannot add edge between unknown vector");
        }

        Edge e(element, nextEdgeID++, v.id, u.id, *this);
        edgeMap.emplace(e.id, e);
        adjacencyList[v.id].push_back(e.id);
        adjacencyList[u.id].push_back(e.id);
        return edgeMap.at(e.id);
    }

    void eraseVertex(const Vertex& v) {
        vertexMap.erase(v.id);
        for (int edgeID : adjacencyList.at(v.id)) {
            edgeMap.erase(edgeID);
        }
    }

    void eraseEdge(const Edge& e) {
        edgeMap.erase(e.id);
        auto startEdges = adjacencyList.at(e.startVertex);
        startEdges.erase(std::remove(startEdges.begin(), startEdges.end(), e.id), startEdges.end());
        auto endEdges = adjacencyList.at(e.endVertex);
        endEdges.erase(std::remove(endEdges.begin(), endEdges.end(), e.id), endEdges.end());
    }

    VertexList vertices() const {
        return VertexList(vertexMap);
    }

    EdgeList edges() const {
        return EdgeList(edgeMap);
    }

private:
    int nextVertexID = 1;
    int nextEdgeID = 1;
    AdjacencyList adjacencyList;
    VertexMap vertexMap;
    EdgeMap edgeMap;
};
