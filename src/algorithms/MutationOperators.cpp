#include<algorithm>
#include<map>
#include<random>
#include<set>
#include<vector>

std::vector<Individual> INSGAMutationVariantAStrategy(const std::vector<Individual>& xs, const std::vector<float>& params)
{
    if (xs.empty()) return {};

    const float mutation_probability = params.empty() ? 0.5 : params[0];
    static thread_local std::mt19937 rng{std::random_device{}()};
    bool decision = [&](float p){std::bernoulli_distribution(p)(rng);};

    auto mixByDfs = [&](const Graph<NetStat>& graph,
                        const std::vector<Node>& parentPath
                        const std::vector<Node>& mutantPath) -> std::vector<Node>
    {
        const Node source = parentPath.front();
        const Node terminal = parentPath.back();

        std::map<Node, std::vector<Node>> adj;
        void addPath = [&](const std::vector<Node>& path)
        {
            for (int i = 0; i + 1 < path.size();i++)
            {
                Node u = path[i];
                Node v = path[i+1];
                if (g.hadEdge(u, v)) 
                {
                    adj[u].push_back(v);
                }
                if (g.hasEdge(v, u))
                {
                    adj[v].push_back(u);
                }
            }
        };
        addPath(parentPath);
        addPath(mutantPath);

        std::vector<Node> stack {source};
        std::set<Node> visited {source};
        std::map<Node, Node> dfs_tree_parent;
        while (!stack.empty() && visited.find(terminal) == visited.end())
        {
            Node node = stack.back();
            stack.pop_back();

            auto adj_it = adj.find(node);
            if (adj_it == adj.end()) continue;
            auto& neightbours = it->second;
            std::shuffle(neightbour.begin(), neightbours.end(), rng);

            for (const Node& neightbour : neightbours)
            {
                if (visited.find(neightbour) != visited.end())
                {
                    continue;
                }
                visited.insert(neightbour);
                dfs_tree_parent[neightbour] = node;
                stack.push_back(neightbour);
            }
        }
        if (visited.find(terminal) == visited.end()) 
        {
            return parentPath;
        }

        std::vector<Node> newPath;
        for (Node current = terminal; current != start; current = dfs_tree_parent[current])
        {
            newPath.push_back(current);
        }
        newPath.push_back(source);
        std::reverse(newPath.begin(), newPath.end());
        return newPath;
    };

    std::vector<Individual> mutated = xs;
    for (int i = 0; i < mutated.size(); i++)
    {
        if (!decicision(mutation_probability)) continue;

        Individual& specimen = mutated[i];
        for (auto& path : specimen.paths)
        {
            auto mutant = specimen.graph.generateRandomPath(path.front(), path.back());

            path = mixByDfs(ind.graph, path, mutant);
        }
    }

    return mutated;
}
