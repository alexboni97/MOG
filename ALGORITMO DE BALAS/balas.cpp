/*
-Implementación del Algoritmo de Balas
-Facultad de Informática
-Universidad Complutense de Madrid
-Compilador: Visual Studio 2022
-Copyright (c) 2024 SERGIO SÁNCHEZ CARRASCO 3ºE.
*/

#include <fstream>
#include <vector>
#include <algorithm>
#include "EnterosInf.h"
#include <iostream>
#include <cmath>

using namespace std;

const EntInf MenosInfinito = -1000000000;
const int maximas_iteraciones = 1000;

struct Vueltas {
	int id;
	vector<int> solucion;
	int infactibilidad;
	EntInf resultado;
	bool operator<(const Vueltas& v) const { return infactibilidad < v.infactibilidad; }
};

class Restriccion {
public:
	Restriccion(vector<int> coeficientes, string type, int values) : coeficientes(coeficientes), valor(values), tipo(type) {}
	int getValor() const { return valor; }
	string getTipo() const { return tipo; }
	vector<int> getCoeficientes() const { return coeficientes; }
	bool verificaOperacion(int resultado) const {
		if (tipo == "<=") return resultado <= valor;
		else if (tipo == ">=") return resultado >= valor;
		else if (tipo == "==") return resultado == valor;
		return false;
	}
	int calcularInfactibilidad(const vector<int>& sol) {
		int resultado = 0;
		for (int i = 0; i < coeficientes.size(); i++) resultado += coeficientes[i] * sol[i];
		return resultado;
	}
private:
	vector<int> coeficientes;
	int valor;
	string tipo;
};

class AlgoritmoBalas1965 {
public:
	AlgoritmoBalas1965(vector<int> fo, string tip, vector<Restriccion> r) : funcion_objetivo(fo), solucion(fo.size(), 0), tipo(tip), resultado(0), restricciones(r) {
		bool optimal = false, vuelta_completa = false;
		int contador = 0;
		resultado = (tipo == "min") ? Infinito : MenosInfinito;
		int max_iteraciones = pow(2, funcion_objetivo.size()) + 1;
		mostrar();
		Vueltas iteracion = { contador, solucion, 0, (tipo == "min") ? Infinito : MenosInfinito };
		ejecutar_iteracion(contador, iteracion, optimal, contador);
		contador++;
		while (!optimal || !vuelta_completa) {
			if ((funcion_objetivo.size() < 10 && contador > max_iteraciones) || contador >= maximas_iteraciones) {
				cout << "Se ha alcanzado el limite maximo de iteraciones. El algoritmo no ha encontrado una solucion optima." << endl;
				break;
			}
			int indiceV = (contador - 1) % funcion_objetivo.size();
			iteracion = { contador, solucion, 0, (tipo == "min") ? Infinito : MenosInfinito };
			ejecutar_iteracion(indiceV, iteracion, optimal, contador);
			contador++;
			vuelta_completa = (contador - 1) % funcion_objetivo.size() == 0;
			if (vuelta_completa && contador > 1) {
				sort(iteraciones.begin(), iteraciones.end());
				int indice = (iteraciones[0].id - 1) % funcion_objetivo.size();
				solucion[indice] = 1;
			}
			vuelta_completa = contador > 2 * funcion_objetivo.size();
		}
		int solucion_indice = (tipo == "min") ? minimizacion(iteraciones) : maximizacion(iteraciones);
		mostrar_solucion(solucion_indice);
	}

	void mostrar() {
		interfaz_algoritmo_balas();
		cout << "\n\n" << tipo << " Z = ";
		for (int i = 0; i < funcion_objetivo.size(); i++) {
			if (funcion_objetivo[i] != 0) {
				if (i > 0 && funcion_objetivo[i] > 0) cout << " + ";
				cout << funcion_objetivo[i] << "x" << i + 1;
			}
		}
		cout << "\n\ns.a:\n\n";
		for (int i = 0; i < restricciones.size(); i++) {
			bool first_term = true;
			for (int j = 0; j < restricciones[i].getCoeficientes().size(); j++) {
				int coef = restricciones[i].getCoeficientes()[j];
				if (coef != 0) {
					if (!first_term && coef > 0) cout << " + ";
					else if (coef < 0) cout << " - ";
					else if (first_term && coef > 0) {}
					cout << abs(coef) << "x" << j + 1;
					first_term = false;
				}
			}
			cout << " " << restricciones[i].getTipo() << " " << restricciones[i].getValor() << "\n";
		}
		cout << "\n\n";
	}

	void mostrar_solucion(int solucion_indice) {
		cout << "SOLUCION OPTIMA\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nZ = " << iteraciones[solucion_indice].resultado << "\nPara las variables: ";
		for (int i = 0; i < funcion_objetivo.size(); i++) cout << "x" << i + 1 << " = " << iteraciones[solucion_indice].solucion[i] << ", ";
		cout << "iteracion: " << iteraciones[solucion_indice].id << "\n";
	}

	void interfaz_algoritmo_balas() {
		string texto = "ALGORITMO DE BALAS";
		int total_width = 50, text_width = texto.length(), padding = (total_width - text_width) / 2;
		cout << string(total_width, '-') << endl << string(padding, '-') << texto << string(padding, '-') << endl << string(total_width, '-') << endl;
	}

private:
	vector<int> funcion_objetivo, solucion;
	vector<Restriccion> restricciones;
	vector<Vueltas> iteraciones;
	string tipo;
	EntInf resultado;

	int funcion_obj(Vueltas iteracion) {
		int resultado = 0;
		for (int i = 0; i < funcion_objetivo.size(); i++) resultado += funcion_objetivo[i] * iteracion.solucion[i];
		return resultado;
	}

	void ejecutar_iteracion(int variable, Vueltas iteracion, bool& es_sol_optima, int contador) {
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nSUBPROBLEMA NUMERO -> " << iteracion.id << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		if (iteracion.id != 0) iteracion.solucion[variable] = 1;
		cout << "B" << contador << ": ";
		for (int i = 0; i < funcion_objetivo.size(); i++) cout << "x" << i + 1 << " = " << iteracion.solucion[i] << ((i < funcion_objetivo.size() - 1) ? ", " : "");
		for (int i = 0; i < restricciones.size(); i++) {
			int resultado = restricciones[i].calcularInfactibilidad(iteracion.solucion);
			cout << "\n\nRestriccion " << i + 1 << ": " << resultado << " " << restricciones[i].getTipo() << " " << restricciones[i].getValor() << "\n";
			if (!restricciones[i].verificaOperacion(resultado)) {
				cout << "La solucion no es factible\n";
				iteracion.infactibilidad += abs(restricciones[i].getValor() - resultado);
				cout << "Infactibilidad restriccion " << i + 1 << ": " << abs(restricciones[i].getValor() - resultado) << "\n";
			}
			else cout << "La solucion es factible\n";
		}
		if (iteracion.infactibilidad != 0) cout << "\n\nInfactibilidad total: " << iteracion.infactibilidad << "\n";
		if (iteracion.infactibilidad == 0) {
			iteracion.resultado = funcion_obj(iteracion);
			resultado = (tipo == "min") ? min(iteracion.resultado, resultado) : max(iteracion.resultado, resultado);
			cout << "\n\n" << tipo << " Z = " << iteracion.resultado << "\n";
			es_sol_optima = true;
		}
		iteraciones.push_back(iteracion);
		cout << "\n";
	}

	int maximizacion(vector<Vueltas> its) {
		int indice = 0;
		EntInf max = MenosInfinito;
		for (int i = 0; i < its.size(); i++) if (its[i].resultado > max) { max = its[i].resultado; indice = i; }
		return indice;
	}

	int minimizacion(vector<Vueltas> its) {
		int indice = 0;
		EntInf min = Infinito;
		for (int i = 0; i < its.size(); i++) if (its[i].resultado < min) { min = its[i].resultado; indice = i; }
		return indice;
	}
};

bool resolverCaso() {
	int variables, restricciones;
	cin >> variables >> restricciones;
	if (variables == -1 && restricciones == -1) return false;
	string tipo;
	cin >> tipo;
	vector<int> funcion_objetivo(variables);
	for (int i = 0; i < variables; i++) cin >> funcion_objetivo[i];
	string restriccion;
	vector<Restriccion> restricciones_problema;
	for (int i = 0; i < restricciones; i++) {
		vector<int> vector_r;
		for (int j = 0; j < variables; j++) {
			int valor;
			cin >> valor;
			vector_r.push_back(valor);
		}
		int RHS;
		cin >> restriccion >> RHS;
		restricciones_problema.push_back(Restriccion(vector_r, restriccion, RHS));
	}
	AlgoritmoBalas1965 algoritmo_balas(funcion_objetivo, tipo, restricciones_problema);
	cout << "\n";
	return true;
}

int main() {
#ifndef DOMJUDGE
	std::ifstream in("casos.txt");
	if (!in.is_open()) std::cout << "Error: no se ha podido abrir el archivo de entrada." << std::endl;
	auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif
	while (resolverCaso());
#ifndef DOMJUDGE
	std::cin.rdbuf(cinbuf);
	std::cout << "Pulsa Intro para salir..." << std::flush;
	std::cin.get();
#endif
	return 0;
}
