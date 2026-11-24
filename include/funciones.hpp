#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

// Cantidad de funciones matemáticas disponibles.
inline constexpr int VARIANTES_POR_FAMILIA = 10;
inline constexpr std::size_t FAMILIAS_CON_VARIANTES = 15;
inline constexpr std::size_t TOTAL_FUNCIONES =
    VARIANTES_POR_FAMILIA * FAMILIAS_CON_VARIANTES + 1;

// Configuración del sistema de ataque.
inline constexpr int ATAQUE_INICIAL = 10;
inline constexpr int64_t CAMBIO_MINIMO = -8;
inline constexpr int64_t CAMBIO_MAXIMO = 12;
inline constexpr int ANALISIS_PUNTAJE = 20;

// Una familia matemática y una de sus variantes.
struct Funcion {
    std::size_t familia;
    int variante;

    std::string nombre() const;
    int64_t evaluar(int turno) const;
    std::string formula() const;
    int64_t cambio_ataque(int turno) const;
    int64_t ataque(int64_t ataque_inicial, int turno) const;
};

std::vector<Funcion> generar_funciones();
