/*
-Implementación del Algoritmo de Balas  
-Facultad de Informática
-Universidad Complutense de Madrid
-Visual Studio 2022
-Copyright (c) 2024 SERGIO SÁNCHEZ CARRASCO 3ºE.  
*/

//Librerías utilizadas
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

//Definición de constantes
const int MAX_VAL = INT_MAX;
const int MIN_VAL = INT_MIN;

//Definimos una clase para representar las restricciones del problema
class Restriccion {
public:
	Restriccion(vector<int> coeficientes, int values, string type) : coeficientes(coeficientes), valor(values), tipo(type) {}

	//Devolvemos el valor
	int getValor() const {
		return valor;
	}

	//Devolvemos el tipo
	string getTipo() const {
		return tipo;
	}

	//Devolvemos los coeficientes
	vector<int> getCoeficientes() const {
		return coeficientes;
	}


	bool verificaOperacion(int resultado) const {
		if (tipo == "<=")
		{
			return resultado <= valor;
		}
		else if (tipo == ">=")
		{
			return resultado >= valor;
		}
		else if (tipo == "==")
		{
			return resultado == valor;
		}

		return false;
	}

	int calcularInfactibilidad(const vector<int>& solucion)  {
		int resultado = 0;

		for (int i = 0; i < this->coeficientes.size(); i++)
		{
			resultado += this->coeficientes[i] * solucion[i];
		}

		return resultado;
	}

private:
	vector<int> coeficientes;
	int valor;
	string tipo;
};

//Definimos un struct para representar las "vueltas" que se dan en una iteración 
struct Vueltas
{
	int id; 
	vector<int> solucion;
	int infactibilidad;
	int resultado; //z

	//Sobrecargamos el operador < para poder comparar dos iteraciones
	bool operator<(const Vueltas& v) const {
		return infactibilidad < v.infactibilidad;
	}
};


//Definimos una clase para representar el algoritmo de balas
class AlgoritmoBalas {

public:
	AlgoritmoBalas(vector<int> fo, vector<Restriccion> r, string tip): funcion_objetivo(fo), solucion(fo.size(),0),tipo(tip), resultado(0), restricciones(r) {
	
		bool optimal = false;//Booleano para saber si hemos encontrado la solución óptima
		int contador = 0;//Contador para saber en qué iteración estamos
		bool vuelta_completa= false;

		if (this->tipo == "min")
		{
			this->resultado = MAX_VAL;
		}
		else if (this->tipo == "max")
		{
			this->resultado = MIN_VAL;
		}

		this->mostrar();
	
		//Caso base todas las variables a 0

		Vueltas iteracion = {contador, solucion,0,0};

		if (tipo == "min")
		{
			iteracion.resultado = MAX_VAL;
		}
		else if (tipo == "max")
		{
			iteracion.resultado = MIN_VAL;
		}

		this->ejecutar_iteracion(contador, iteracion, optimal, contador);
		contador++;

		while (!optimal || !vuelta_completa)
		{
			int indiceVariable = (contador - 1) % funcion_objetivo.size();//Calculamos el índice de la variable a cambiar

			iteracion = { contador, solucion,0,0 };

			if (tipo == "min")
			{
				iteracion.resultado = MAX_VAL;
			}
			else if (tipo == "max")
			{
				iteracion.resultado = MIN_VAL;
			}

			this->ejecutar_iteracion(indiceVariable, iteracion, optimal, contador);
			contador++;

			vuelta_completa = (contador - 1) % funcion_objetivo.size() == 0;

			if (contador >1 &&vuelta_completa )
			{
				//Miramos el que tenga menor factibilidad
				sort(iteraciones.begin(), iteraciones.end());

				int indice = (iteraciones[0].id - 1) % funcion_objetivo.size();
				this->solucion[indice] = 1;
			}
			vuelta_completa = contador > 2 * funcion_objetivo.size();
		}

		int solucion_indice = 0;

		if (this->tipo == "min")
		{
			solucion_indice = minimizacion(iteraciones);
		}
		else if (this->tipo == "max")
		{
			solucion_indice = maximizacion(iteraciones);
		}

		cout << "SOLUCION OPTIMA\n";
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		cout << "Z = " << iteraciones[solucion_indice].resultado << "\n";
		cout << "Para las variables: ";

		for (int i = 0; i < funcion_objetivo.size(); i++)
		{
			cout << "x" << i + 1 << " = " << iteraciones[solucion_indice].solucion[i] << ", ";
		}

		cout << "iteración: " << iteraciones[solucion_indice].id << "\n";
	}


	void mostrar() {

		this->interfaz_algoritmo_balas();

		cout << "\n\n";

		cout << this->tipo << " Z = ";

		for (int i = 0; i < funcion_objetivo.size(); i++)
		{
			cout << funcion_objetivo[i] << "(x" << i + 1 << ")";
			if (i < funcion_objetivo.size()-1)
			{
				cout << " + ";
			}
		}

		cout << "\n\ns.a\n\n";

		for (int i = 0; i < restricciones.size(); i++)
		{
			for (int j = 0; j < restricciones[i].getCoeficientes().size(); j++)
			{
				cout << restricciones[i].getCoeficientes()[j] << "(x" << j + 1 << ")";
				if (j < restricciones[i].getCoeficientes().size() - 1)
				{
					cout << " + ";
				}
			}

			cout << restricciones[i].getTipo() << " " << restricciones[i].getValor() << "\n";
		}

		cout << "\n\n";
	}

	void interfaz_algoritmo_balas() {
		string texto = "ALGORITMO DE BALAS";
		int total_width = 50;  // Ancho total de la línea (ajustar según tu preferencia)
		int text_width = texto.length();  // Longitud del texto a centrar
		int padding = (total_width - text_width) / 2;  // Espacios para centrar el texto

		// Línea superior 
		cout << string(total_width, '-') << endl;

		// Línea con el texto centrado
		cout << string(padding, '-') << texto << string(padding, '-') << endl;

		// Línea inferior 
		cout << string(total_width, '-') << endl;
	}

private:
	vector<int> funcion_objetivo;//Función objetivo de nuestro problema
	vector<Restriccion> restricciones;
	vector<Vueltas> iteraciones;
	vector<int> solucion;
	string tipo;
	int resultado;//z

	
	int funcion_obj(Vueltas iteracion) {
		int resultado = 0;

		for (int i = 0; i < this->funcion_objetivo.size(); i++)
		{
			resultado += this->funcion_objetivo[i] * iteracion.solucion[i];
		}

		return resultado;
	}

	void ejecutar_iteracion(int variable, Vueltas iteracion, bool& es_sol_optima, int contador) {

		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		cout << "ITERACION NUMERO -> " << iteracion.id << "\n";
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

		if (iteracion.id != 0)
		{
			iteracion.solucion[variable] = 1;
		}

		cout << "B" << contador <<": ";

		for (int i = 0; i < funcion_objetivo.size(); i++)
		{
			cout << "x" << i + 1 << " = " << iteracion.solucion[i];
			if (i < funcion_objetivo.size()-1)
			{
				cout << ", ";
			}
		}

		//Calculamos las restricciones de la iteración
		for (int i = 0; i < restricciones.size(); i++)
		{
			int resultado = restricciones[i].calcularInfactibilidad(iteracion.solucion);

			cout << "\n\nRestriccion " << i + 1 << ": " << resultado << " " << restricciones[i].getTipo() << " " << restricciones[i].getValor() << "\n";

			if (!restricciones[i].verificaOperacion(resultado))
			{
				cout << "La solucion no es factible\n";
				//iteracion.infactibilidad = restricciones[i].getValor() - resultado;
				iteracion.infactibilidad += abs(restricciones[i].getValor() - resultado);
				int infactibilidad = abs(restricciones[i].getValor() - resultado);

				cout << "Infactibilidad restriccion " << i +1 << ": " << infactibilidad << "\n";
			}
			else
			{
				cout << "La solucion es factible\n";
			}

		}

		cout << "\n\nInfactibilidad total: " << iteracion.infactibilidad << "\n";

		if (iteracion.infactibilidad == 0)
		{
			iteracion.resultado = funcion_obj(iteracion);

			//Estamos en un problema de minimización
			if (this->tipo == "min")
			{
				this->resultado = min(iteracion.resultado, this->resultado);
			}

			//Estamos en un problema de maximización
			else if (this->tipo == "max")
			{
				this->resultado = max(iteracion.resultado, this->resultado);
			}

			cout << "\n\n";
			cout << this->tipo << " Z = " << iteracion.resultado << "\n";
			es_sol_optima = true;
		}

		iteraciones.push_back(iteracion);
		cout << "\n";
	}

	int maximizacion(vector<Vueltas> its) {

		int indice = 0;
		int max = MIN_VAL;

		for (int i = 0; i < its.size(); i++)
		{
			if (its[i].resultado > max)
			{
				max = its[i].resultado;
				indice = i;
			}
		}

		return indice;
	}

	int minimizacion(vector<Vueltas> its) {

		int indice = 0;
		int min = MAX_VAL;

		for (int i = 0; i < its.size(); i++)
		{
			if (its[i].resultado < min)
			{
				min = its[i].resultado;
				indice = i;
			}
		}

		return indice;
	}

};

//Función para resolver el caso correspondiente, se nos da esta plantilla para los ejercicios de TAIS y la voy a reutilizar
bool resolverCaso() {

	int variables, restricciones;

	cin >> variables >> restricciones;

	if (variables==-1 && restricciones==-1)
	{
		return false;
	}

	string tipo;
	cin >> tipo;

	vector<int> funcion_objetivo(variables);//Inicializamos el vector de la función objetivo con el número de variables del problema.

	//Leemos la función objetivo
	for (int i = 0; i < variables; i++)
	{
		cin >> funcion_objetivo[i];
	}

	string restriccion;
	vector<Restriccion> restricciones_problema;//Inicializamos el vector de restricciones con el número de restricciones del problema.

	//Leemos las restricciones que impone el problema.
	for (int i = 0; i < restricciones; i++)
	{
		vector<int> vector_r;

		for (int j = 0; j < variables; j++)
		{
			int valor;
			cin >> valor;
			vector_r.push_back(valor);
		}

		int RHS;

		cin >> restriccion >> RHS;
		Restriccion r(vector_r, RHS, restriccion);

		restricciones_problema.push_back(r);	
	}

	AlgoritmoBalas algoritmo(funcion_objetivo, restricciones_problema, tipo);
	cout << "\n";

	return true;
}

int main() {

	// ajustes para que cin extraiga directamente de un fichero
	std::ifstream in("casos.txt");
	if (!in.is_open())
		std::cout << "Error: no se ha podido abrir el archivo de entrada." << std::endl;
	auto cinbuf = std::cin.rdbuf(in.rdbuf());

    std::ofstream out("salida.txt");
    
	while (resolverCaso());

	// para dejar todo como estaba al principio
	std::cin.rdbuf(cinbuf);
	std::cout << "Pulsa Intro para salir..." << std::flush;
	std::cin.get();
	return 0;
}
