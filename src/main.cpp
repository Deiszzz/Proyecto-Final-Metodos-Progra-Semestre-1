
#include "funciones.hpp"
#include "puntos.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

using namespace std;

int main() {
    // 1. Generar las 151 funciones matemáticas
    vector<Funcion> funciones = generar_funciones();

    // 2. Seleccionar una función aleatoria
    random_device semilla;
    mt19937 generador(semilla());

    uniform_int_distribution<size_t> distribucion(
        0, funciones.size() - 1
    );

    size_t indice = distribucion(generador);
    Funcion seleccionada = funciones[indice];

    // 3. Calcular los puntos disponibles
    uint32_t puntos = calculadora_puntos(
        seleccionada,
        ATAQUE_INICIAL
    );

    // 4. Mostrar la función seleccionada
    cout << "===== JUEGO MATEMATICO =====\n\n";

    cout << "Funciones disponibles: "
         << funciones.size() << '\n';

    cout << "Funcion seleccionada: "
         << seleccionada.nombre() << '\n';

    cout << "Numero: "
         << indice + 1 << " de "
         << funciones.size() << '\n';

    cout << "Variante: "
         << seleccionada.variante << '\n';

    cout << seleccionada.formula() << "\n\n";

    // 5. Analizar los ataques
    cout << "===== ANALISIS MATEMATICO =====\n\n";

    int64_t suma_ataques = 0;

    for (uint32_t turno = 0;
         turno < ANALISIS_PUNTAJE;
         ++turno) {

        int64_t cambio =
            seleccionada.cambio_ataque(turno);

        int64_t ataque =
            seleccionada.ataque(ATAQUE_INICIAL, turno);

        suma_ataques += ataque;

        cout << "Turno " << setw(2) << turno + 1
             << " | Cambio: " << showpos << setw(3) << cambio
             << noshowpos
             << " | Ataque: " << setw(2) << ataque
             << '\n';
    }

    // 6. Mostrar el promedio
    double promedio =
        static_cast<double>(suma_ataques) / ANALISIS_PUNTAJE;

    cout << "\nAtaque promedio: "
         << fixed << setprecision(2)
         << promedio << '\n';

    // 7. Mostrar los puntos disponibles
    cout << "\n===== SISTEMA DE PUNTOS =====\n\n";

    cout << "Puntos por jugador: "
         << puntos << '\n';

    cout << "Puntos para Jugador 1: "
         << puntos << '\n';

    cout << "Puntos para Jugador 2: "
         << puntos << '\n';

    return 0;
}
