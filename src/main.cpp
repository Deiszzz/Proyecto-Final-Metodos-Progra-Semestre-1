#include "funciones.hpp"
#include "puntos.hpp"

#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    // Generar las 151 funciones y seleccionar una al azar.
    vector<Funcion> funciones = generar_funciones();

    random_device semilla;
    mt19937 generador(semilla());
    uniform_int_distribution<size_t> distribucion(0, funciones.size() - 1);

    size_t indice = distribucion(generador);
    Funcion seleccionada = funciones[indice];
    int puntos = calculadora_puntos(seleccionada, ATAQUE_INICIAL);

    cout << "===== JUEGO MATEMATICO =====\n\n";
    cout << "Funciones disponibles: " << funciones.size() << '\n';
    cout << "Funcion seleccionada: " << seleccionada.nombre() << '\n';
    cout << "Numero: " << indice + 1 << " de " << funciones.size() << '\n';
    cout << "Variante: " << seleccionada.variante << '\n';
    cout << seleccionada.formula() << "\n\n";

    cout << "===== ANALISIS MATEMATICO =====\n\n";

    // La suma mantiene 64 bits porque evaluar() puede producir valores grandes.
    int64_t suma_ataques = 0;

    for (int turno = 0; turno < ANALISIS_PUNTAJE; ++turno) {
        int64_t cambio = seleccionada.cambio_ataque(turno);
        int64_t ataque = seleccionada.ataque(ATAQUE_INICIAL, turno);
        suma_ataques += ataque;

        cout << "Turno " << setw(2) << turno + 1
             << " | Cambio: " << showpos << setw(3) << cambio << noshowpos
             << " | Ataque: " << setw(2) << ataque << '\n';
    }

    double promedio = static_cast<double>(suma_ataques) / ANALISIS_PUNTAJE;
    cout << "\nAtaque promedio: " << fixed << setprecision(2) << promedio << '\n';

    cout << "\n===== SISTEMA DE PUNTOS =====\n\n";
    cout << "Puntos por jugador: " << puntos << '\n';
    cout << "Puntos para Jugador 1: " << puntos << '\n';
    cout << "Puntos para Jugador 2: " << puntos << '\n';
}
