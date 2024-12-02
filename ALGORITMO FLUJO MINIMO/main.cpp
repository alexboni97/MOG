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

using namespace std;

bool resolverCaso(){

    if(!cin)
        return false;

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