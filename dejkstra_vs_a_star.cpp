#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <random>
#include <queue>

template<typename T> bool containes(std::vector<T*> const& container, T* x)
{
	bool result = false;
	for (T* item : container) if (item == x) { result = true; break; }
	return result;
}

inline float distance(sf::Vector2f const& a, sf::Vector2f const& b)
{
	float const dx = a.x - b.x, const dy = a.y - b.y;
	return std::sqrtf(dx * dx + dy * dy);
}


struct Node {

	float x{ 0.f }, y{ 0.f };
	float C{ -1.f };
	bool dead{ false };
	Node* from{ nullptr };
	bool path{ false };

	std::vector<struct Edge*> edges;

	std::vector<Node*> neighbors();
	Node* add(Node* node);
	float distance_to(Node* node)
	{
		float const dx = x - node->x, const dy = y - node->y;
		return std::sqrtf(dx * dx + dy * dy);
	}
};


struct Edge {
	Node* node1{ nullptr }, * node2{ nullptr };
	float length()
	{
		float const dx{ node1->x - node2->x }, const dy{ node1->y - node2->y };
		return std::sqrtf(dx * dx + dy * dy);
	}
	inline Node* neighbor_of(Node* node)
	{
		return node == node1 ? node2 : node1;
	}
};


struct Graph {

	std::vector<Node*> find_path(bool a_star, Node* from, Node* to = nullptr)
	{
		if (to == nullptr) to = nodes_pool.back();

		std::vector<Node*> result;
		if (from == to) return result;

		std::vector<Node*> Q;
		Q.push_back(from);

		for (auto node : nodes_pool)
		{
			node->path = node->dead = false;
			if (node == from)
			{
				node->C = a_star ? distance({ from->x, from->y }, {to->x, to->y}) : 0.f
			}
			else
			{
				node->C = -1.f;
			}
			node->from = nullptr;
		}

		while (!Q.empty())
		{
			Node* node = pop(Q);
			node->dead = true;

			if (node == to)
			{
				fill_backward(node, result);
				for (auto n : result) n->path = true;
				break;
			}

			for (size_t idx = 0; idx < node->edges.size(); ++idx)
			{
				Edge* edge = node->edges[idx];
				Node* neighbor = edge->neighbor_of(node);

				if (!neighbor->dead)
				{
					float cost_to_come = neighbor->C + edge->length();
					
					if (a_star)
					{
						cost_to_come += distance({neighbor->x, neighbor->y}, {to->x, to->y});
					}

					if (neighbor->C < 0.f || cost_to_come < neighbor->C)
					{
						neighbor->C = cost_to_come;
						neighbor->from = node;
					}

					Q.push_back(neighbor);
				}
			}
		}

		return result;
	}
	void fill(size_t size, float max_distance, sf::Vector2u field)
	{
		clear();

		static std::random_device device;
		static std::mt19937 engine(device());
		std::uniform_real_distribution<float> dist_x(0.f, field.x), dist_y(0.f, field.y);
		std::uniform_int_distribution<int> dist_prop(0, 1);

		for (size_t idx = 0; idx < size; ++idx)
		{
			make_node(dist_x(engine), dist_y(engine));
		}

		for (size_t idx = 0; idx < size; ++idx)
		{
			Node* node_i = nodes_pool[idx];

			for (size_t jdx = 0; jdx < size; ++jdx)
			{
				if (idx == jdx) break;

				Node* node_j = nodes_pool[jdx];

				bool need_edge = dist_prop(engine)
					&& node_i->distance_to(node_j) < max_distance
					&& !containes(node_i->neighbors(), node_j);

				if (need_edge)
				{
					make_edge(node_i, node_j);
					if (node_i->edges.size() > 2) break;
				}
			}
		}
	}
	void draw_to(sf::RenderWindow& canvas)
	{
		static sf::VertexArray vtxs(sf::PrimitiveType::Lines, 2);
		for (auto edge : edges_pool)
		{
			if (edge->node1 != nullptr && edge->node2 != nullptr)
			{
				vtxs[0].position.x = edge->node1->x, vtxs[0].position.y = edge->node1->y;
				vtxs[1].position.x = edge->node2->x, vtxs[1].position.y = edge->node2->y;
				vtxs[1].color = vtxs[0].color = edge->node1->path && edge->node2->path ? sf::Color(250, 150, 0) : (edge->node1->dead || edge->node2->dead) ? sf::Color{0,255,255, 200} : sf::Color{ 100,100,100,100 };
				canvas.draw(vtxs);
			}
		}
		static sf::CircleShape cir;
		cir.setOrigin(10.f, 10.f);
		cir.setRadius(10.f);

		sf::Vector2f mouse(sf::Mouse::getPosition(canvas));

		for (auto node : nodes_pool)
		{
			if (distance(mouse, { node->x, node->y }) < 10.f)
			{
				bool a_star = sf::Mouse::isButtonPressed(sf::Mouse::Left);
				find_path(a_star, node);
				cir.setFillColor({ 250, 150, 0 });
				canvas.setTitle(a_star ? "A*" : "Dejkstra");
			}
			else
			{
				cir.setFillColor(node == nodes_pool.back() ? sf::Color::Red : node->path ? sf::Color{250,150,0} : sf::Color{ 100,100,100,100 });
			}
			cir.setPosition(node->x, node->y);
			canvas.draw(cir);
		}
	}
	static void clear()
	{
		for (auto edge : edges_pool) delete edge;
		edges_pool.clear();
		for (auto node : nodes_pool) delete node;
		nodes_pool.clear();
	}
	static Edge* make_edge(Node* node1, Node* node2)
	{
		Edge* edge;
		Graph::edges_pool.push_back(edge = new Edge());
		edge->node1 = node1, edge->node2 = node2;
		node1->edges.push_back(edge), node2->edges.push_back(edge);
		return edge;
	}
	static void destroy(Edge* edge)
	{
		if (edge != nullptr)
		{
			for (size_t idx = 0; idx < edges_pool.size(); ++idx)
			{
				if (edges_pool[idx] == edge)
				{
					edges_pool.erase(edges_pool.begin() + idx);
					delete edge;
					break;
				}
			}
		}
	}
	static Node* make_node(float x, float y)
	{
		Node* node;
		nodes_pool.push_back(node = new Node());
		node->x = x, node->y = y;
		return node;
	}
	static void destroy(Node* node)
	{
		for (auto edge : node->edges)
		{
			(edge->node1 == node ? edge->node1 : edge->node2) = nullptr;
		}
		for (size_t idx = 0; idx < nodes_pool.size(); ++idx)
		{
			if (node == nodes_pool[idx])
			{
				nodes_pool.erase(nodes_pool.begin() + idx);
				delete node;
				break;
			}
		}
	}
private:

	static std::vector<Edge*> edges_pool;
	static std::vector<Node*> nodes_pool;

	void fill_backward(Node* node, std::vector<Node*>& result)
	{
		result.push_back(node);
		while (node->from != nullptr)
		{
			result.push_back(node = node->from);
		}
	}
	Node* pop(std::vector<Node*>& Q)
	{
		std::sort(Q.begin(), Q.end(), [](Node* A, Node* B)
			{
				return A->C != -1 && A->C > B->C;
			});

		Node* node = Q.back();
		Q.pop_back();
		return node;
	}
};
std::vector<Edge*> Graph::edges_pool;
std::vector<Node*> Graph::nodes_pool;


std::vector<Node*> Node::neighbors()
{
	std::vector<Node*> result;
	for (auto edge : edges) if (edge != nullptr) result.push_back(edge->neighbor_of(this));
	return result;
}


Node* Node::add(Node* node)
{
	edges.push_back(Graph::make_edge(this, node));
	return this;
}


int main()
{
	sf::RenderWindow window{ {1280,720}, "Dejkstra" };
	Graph graph;
	graph.fill(220, 150.f, window.getSize());
	bool was_pressed = false;

	while (window.isOpen())
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				window.close();
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		{
			if (!was_pressed)
			{
				graph.fill(220, 150.f, window.getSize());
				was_pressed = true;
			}
		}
		else
		{
			was_pressed = false;
		}

		window.clear();
		graph.draw_to(window);
		window.display();
	}

	return 0;
}
