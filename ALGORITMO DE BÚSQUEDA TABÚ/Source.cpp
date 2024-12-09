#include <algorithm>
#include <iostream>
#include <numeric> 
#include <vector>
#include <fstream>

// Definir la funcion objetivo
int funcion_objetivo(const std::vector<int>& solucion) {
    // Calcula la suma de las distancias absolutas entre elementos consecutivos
    int aptitud = 0;
    for (size_t i = 0; i < solucion.size() - 1; i++) {
        aptitud += std::abs(solucion[i] - solucion[i + 1]);
    }
    return aptitud;
}


// Definir la funcion de vecindad
std::vector<std::vector<int>> obtener_vecinos(const std::vector<int>& solucion)
{ // Agregado el calificador const
    std::vector<std::vector<int>> vecinos;
    for (size_t i = 0; i < solucion.size(); i++) {
        for (size_t j = i + 1; j < solucion.size(); j++) {
            std::vector<int> vecino = solucion;
            std::swap(vecino[i], vecino[j]);
            vecinos.push_back(vecino);
        }
    }
    return vecinos;
}


// Definir el algoritmo de Busqueda Tabu
std::vector<int>
busqueda_tabu(const std::vector<int>& solucion_inicial,
    int max_iteraciones, int tamano_lista_tabu)
{ // Agregado el calificador const
    std::vector<int> mejor_solucion = solucion_inicial;
    std::vector<int> solucion_actual = solucion_inicial;
    std::vector<std::vector<int> > lista_tabu;


    for (int iter = 0; iter < max_iteraciones; iter++) {
        std::vector<std::vector<int> > vecinos = obtener_vecinos(solucion_actual);
        std::vector<int> mejor_vecino;
        int mejor_aptitud_vecino = std::numeric_limits<int>::max();


        for (const std::vector<int>& vecino : vecinos) {
            if (std::find(lista_tabu.begin(), lista_tabu.end(), vecino) == lista_tabu.end()) {
                int aptitud_vecino = funcion_objetivo(vecino);
                if (aptitud_vecino < mejor_aptitud_vecino) {
                    mejor_vecino = vecino;
                    mejor_aptitud_vecino = aptitud_vecino;
                }
            }
        }


        if (mejor_vecino.empty()) {
            // No se encontraron vecinos no tabu,
            // termina la busqueda
            break;
        }


        solucion_actual = mejor_vecino;
        lista_tabu.push_back(mejor_vecino);
        if (lista_tabu.size() > tamano_lista_tabu) {
            // Elimina la entrada mas antigua de la
            // lista tabu si excede el tamanio
            lista_tabu.erase(lista_tabu.begin());
        }


        if (funcion_objetivo(mejor_vecino) < funcion_objetivo(mejor_solucion)) {
            // Actualiza la mejor solucion si el
            // vecino actual es mejor
            mejor_solucion = mejor_vecino;
        }
    }


    return mejor_solucion;
}


bool resolverCaso(){
    int max_iteraciones;
    int tamano_lista_tabu, tam;
    std::cin >> max_iteraciones >> tamano_lista_tabu >> tam;
    if (!std::cin)
        return false;
    std::vector<int> solucion_inicial(tam);
    for (int i = 0; i < tam; i++)
    {
        std::cin >> solucion_inicial[i];
    }

    std::vector<int> mejor_solucion = busqueda_tabu(solucion_inicial, max_iteraciones, tamano_lista_tabu);
    std::cout << "Mejor solucion:";
    for (int val : mejor_solucion) {
        std::cout << " " << val;
    }
    std::cout << std::endl;
    std::cout << "Aptitud de la mejor solucion: " << funcion_objetivo(mejor_solucion) << std::endl;
    return true;
}

int main()
{

    std::ifstream in("casos.txt");
    if (!in.is_open())
         std::cout << "Error: no se ha podido abrir el archivo de entrada." << std::endl;
    auto cinbuf = std::cin.rdbuf(in.rdbuf());

    std::ofstream out("salida.txt");
    auto coutbuf = std::cout.rdbuf(out.rdbuf());
    while (resolverCaso());

    return 0;
}
