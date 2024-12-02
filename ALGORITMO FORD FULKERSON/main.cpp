/*
-Implementación del Algoritmo de Balas  
-Facultad de Informática
-Universidad Complutense de Madrid
-Visual Studio 2022
*/

//Librerías utilizadas
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
// Función auxiliar para realizar una búsqueda en amplitud (BFS) en la red residual
bool bfs(const vector<vector<int>>& residualGraph, int s, int t, vector<int>& parent) {
    int n = residualGraph.size();
    vector<bool> visited(n, false);
    queue<int> q;

    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) { // Arista con capacidad residual
                q.push(v);
                parent[v] = u;
                visited[v] = true;
                if (v == t) return true; // Si llegamos al nodo sumidero
            }
        }
    }

    return false;
}

// Algoritmo de Ford-Fulkerson
int fordFulkerson(vector<vector<int>>& graph, int origen, int destino) {
    int n = graph.size();
    vector<vector<int>> residualGraph = graph; // Copia del grafo para capacidades residuales
    vector<int> parent(n);                    // Para almacenar el camino aumentante
    int maxFlow = 0;

    // Mientras haya un camino aumentante en la red residual
    while (bfs(residualGraph, origen, destino, parent)) {
        // Encontrar la capacidad mínima (bottleneck) en el camino aumentante
        int pathFlow = INT_MAX;
        for (int v = destino; v != origen; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, residualGraph[u][v]);
        }

        // Actualizar las capacidades residuales en el grafo
        for (int v = destino; v != origen; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        // Añadir el flujo del camino al flujo máximo
        maxFlow += pathFlow;
    }

    return maxFlow;
}

bool resolverCaso(){
    int n,m;
    cin >>n>>m;
    if(!cin)
        return false;

    vector<vector<int>>grafo(n,vector<int>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin>>grafo[i][j];
        }
        
    }
    int origen,destino;
    cin>>origen>>destino;
    int flujoTotal= fordFulkerson(grafo,origen,destino);
    cout<<flujoTotal<<"\n";
    return true;
}



int main() {

	// ajustes para que cin extraiga directamente de un fichero
	std::ifstream in("casos.txt");
	if (!in.is_open())
		std::cout << "Error: no se ha podido abrir el archivo de entrada." << std::endl;
	auto cinbuf = std::cin.rdbuf(in.rdbuf());

    std::ofstream out("salida.txt");
    auto coutbuf = std::cout.rdbuf(out.rdbuf());
    while (resolverCaso());

	// para dejar todo como estaba al principio
	std::cin.rdbuf(cinbuf);
	std::cout << "Pulsa Intro para salir..." << std::flush;
	std::cin.get();
	return 0;
}