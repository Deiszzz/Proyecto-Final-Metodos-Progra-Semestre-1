#include "puntos.hpp"

#include <algorithm>
#include <cmath>

using namespace std;

int calculadora_puntos(const Funcion& funcion, int ataque_inicial) {
    // Mantener 64 bits en la suma evita pérdidas si el ataque crece.
    int64_t suma_ataques = 0;

    for (int turno = 0; turno < ANALISIS_PUNTAJE; ++turno) {
        suma_ataques += funcion.ataque(ataque_inicial, turno);
    }

    double promedio = static_cast<double>(suma_ataques) / ANALISIS_PUNTAJE;
    double puntos_calculados = 100.0 * promedio / max(1, ataque_inicial);

    // Limitar ANTES de convertir a int evita conversiones fuera de rango.
    double puntos_limitados = clamp(round(puntos_calculados), 60.0, 180.0);
    return static_cast<int>(puntos_limitados);
}
