#include <algorithm>
#include <iostream>
#include <numeric> 
#include <vector>


// Definir la función objetivo
int funcion_objetivo(const std::vector<int>& solucion)
{ // Agregado el calificador const
    // TODO: Implementa tu función objetivo aquí
    // La función objetivo debe evaluar
    // la calidad de una solución dada y
    // devolver un valor numérico que represente
    // la aptitud de la solución
    // Ejemplo: return std::accumulate(solucion.begin(),
    // solucion.end(), 0);
    return std::accumulate(solucion.begin(), solucion.end(),
        0);
}


// Definir la función de vecindad
std::vector<std::vector<int> >
obtener_vecinos(const std::vector<int>& solucion)
{ // Agregado el calificador const
    std::vector<std::vector<int> > vecinos;
    for (size_t i = 0; i < solucion.size(); i++) {
        for (size_t j = i + 1; j < solucion.size(); j++) {
            std::vector<int> vecino = solucion;
            std::swap(vecino[i], vecino[j]);
            vecinos.push_back(vecino);
        }
    }
    return vecinos;
}


// Definir el algoritmo de Búsqueda Tabú
std::vector<int>
busqueda_tabu(const std::vector<int>& solucion_inicial,
    int max_iteraciones, int tamano_lista_tabu)
{ // Agregado el calificador const
    std::vector<int> mejor_solucion = solucion_inicial;
    std::vector<int> solucion_actual = solucion_inicial;
    std::vector<std::vector<int> > lista_tabu;


    for (int iter = 0; iter < max_iteraciones; iter++) {
        std::vector<std::vector<int> > vecinos
            = obtener_vecinos(solucion_actual);
        std::vector<int> mejor_vecino;
        int mejor_aptitud_vecino
            = std::numeric_limits<int>::max();


        for (const std::vector<int>& vecino : vecinos) {
            if (std::find(lista_tabu.begin(),
                lista_tabu.end(), vecino)
                == lista_tabu.end()) {
                int aptitud_vecino
                    = funcion_objetivo(vecino);
                if (aptitud_vecino
                    < mejor_aptitud_vecino) {
                    mejor_vecino = vecino;
                    mejor_aptitud_vecino
                        = aptitud_vecino;
                }
            }
        }


        if (mejor_vecino.empty()) {
            // No se encontraron vecinos no tabú,
            // termina la búsqueda
            break;
        }


        solucion_actual = mejor_vecino;
        lista_tabu.push_back(mejor_vecino);
        if (lista_tabu.size() > tamano_lista_tabu) {
            // Elimina la entrada más antigua de la
            // lista tabú si excede el tamaño
            lista_tabu.erase(lista_tabu.begin());
        }


        if (funcion_objetivo(mejor_vecino)
            < funcion_objetivo(mejor_solucion)) {
            // Actualiza la mejor solución si el
            // vecino actual es mejor
            mejor_solucion = mejor_vecino;
        }
    }


    return mejor_solucion;
}


int main()
{
    // Uso de ejemplo
    // Proporciona una solución inicial
    std::vector<int> solucion_inicial = { 1, 2, 3, 4, 5 };
    int max_iteraciones = 100;
    int tamano_lista_tabu = 10;


    std::vector<int> mejor_solucion = busqueda_tabu(
        solucion_inicial, max_iteraciones, tamano_lista_tabu);
    std::cout << "Mejor solución:";
    for (int val : mejor_solucion) {
        std::cout << " " << val;
    }
    std::cout << std::endl;
    std::cout << "Aptitud de la mejor solución: "
        << funcion_objetivo(mejor_solucion)
        << std::endl;


    return 0;
}
