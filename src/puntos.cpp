#include "puntos.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

using namespace std;

uint32_t calculadora_puntos(
    const Funcion& funcion,
    int64_t ataque_inicial
    ) {
        int64_t suma_ataques = 0;

        for (uint32_t turno = 0; turno < ANALISIS_PUNTAJE; ++turno){
            suma_ataques += funcion.ataque(ataque_inicial, turno);
            };


        double promedio = static_cast<double>(suma_ataques) / ANALISIS_PUNTAJE;

        double puntos_calculados = 100 * promedio / max <int64_t>(1, ataque_inicial);

        int64_t puntos = static_cast<int64_t>(round(puntos_calculados));

        puntos = clamp(
            puntos,
            int64_t(60),
            int64_t(180)
        );
        return static_cast<uint32_t>(puntos);
    }