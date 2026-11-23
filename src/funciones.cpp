#include "funciones.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace {

    const array<string, 16> nombres_familias = {
        "Lineal lenta",
        "Cuadratica",
        "Cubica",
        "Raiz cuadrada",
        "Logaritmica",
        "Racional",
        "Exponencial Limitada",
        "Seno",
        "Coseno",
        "Tangente controlada",
        "Lineal + Seno",
        "Raiz + Coseno",
        "Seno + Coseno",
        "Onda Triangular",
        "Diente de Sierra",
        "Tangente cuadratica absoluta"
    };
}

int64_t convertir_a_entero(double valor){
    if (isnan(valor)) return 0;
    constexpr double LIMITE = 9223372036854775807.0; // Max int64_t
    if (valor >= LIMITE) return numeric_limits <int64_t>::max();
    if (valor <= -LIMITE) return numeric_limits <int64_t>::min();
    return static_cast<int64_t>(valor);
}

int64_t resta_saturada(int64_t a, int64_t b){
    constexpr auto MIN = numeric_limits<int64_t>::min();
    constexpr auto MAX = numeric_limits<int64_t>::max();
    if (b > 0 && a < MIN + b) return MIN;
    if (b < 0 && a > MAX + b) return MAX;
    return a - b;
}

int64_t suma_saturada(int64_t a, int64_t b){
    constexpr auto MIN = numeric_limits<int64_t>::min();
    constexpr auto MAX = numeric_limits<int64_t>::max();
    if (b > 0 && a > MAX - b) return MAX;
    if (b < 0 && a < MIN - b) return MIN;
    return a + b;
}

string Funcion::nombre() const {
    if (familia >= nombres_familias.size()) {
        throw out_of_range("Familia no valida");
    }
    return nombres_familias[familia];
}

int64_t Funcion::evaluar(uint32_t turno) const {
    const double t = static_cast<double>(turno);
    const double k = static_cast<double>(variante);

    const double amplitud = static_cast<double>(2 + variante  % 4);
    const double angulo1 = t * (0.14 + 0.012 * k) + 0.37 *k;
    const double angulo2 = t * (0.29 + 0.006 * k) + 0.51 *k;

    double resultado = 0.0;
    switch (familia) {
        case 0: // Lineal lenta
            resultado = floor (t / (6.0 + k)); 
            break;
        case 1: // Cuadratica
            resultado = floor (t * t / (80.0 + 12.0 * k));
            break;
        case 2: // Cubica
            resultado = floor (t * t *t / (600.0 + 150.0 * k));
            break;
        case 3: // Raiz cuadrada
            resultado = floor ((0.55 + 0.10 * k) * sqrt(t));
            break;
        case 4: // Logaritmica
            resultado = floor ((1.2 + 0.2 * k) * log (t + 1.0));
            break;
        case 5: // Racional
            resultado = round ((4.0 + k) * t / (t + 8.0 + k));
            break;
        case 6: // Exponencial Limitada
            resultado = round ((4.0 + k) * (1.0 - exp (-t / (4.0 + k))));
            break;
        case 7: // Seno
            resultado = round (amplitud * sin (angulo1));
            break;
        case 8: // Coseno
            resultado = round (amplitud * cos (angulo1));
            break;
        case 9: // Tangente controlada
            {
                const uint64_t periodo = 22 + variante;
                const uint64_t posicion = (static_cast <uint64_t>(turno) + 2ULL * variante) % periodo;
                const double angulo = -0.65 + 11.30 * static_cast<double> (posicion) / (periodo - 1);
                resultado = round (amplitud * tan (angulo));
                break;
            }
        case 10:
            resultado = floor (t / (9.0 + k)) + round (3.0 * sin (angulo1));
            break;
        case 11:
            resultado = floor (0.65 * sqrt(t)) + round (amplitud * cos(angulo1));
            break;
                case 12:
            resultado = std::round(3.0 * std::sin(angulo1 / 1) +
                                   2.0 * std::cos(angulo2));
            break;
        // Onda triangular: sube y baja de forma periódica.
        case 13: {
            const std::uint64_t periodo = 8 + 2ULL * variante;
            const std::uint64_t posicion =
                (static_cast<std::uint64_t>(turno) + variante) % periodo;
            const double mitad = static_cast<double>(periodo - 1) / 2.0;
            resultado = std::round(5.0 -
                10.0 * std::abs(static_cast<double>(posicion) - mitad) / mitad);
            break;
        }
        // Diente de sierra: aumenta y se reinicia al terminar cada periodo.
        case 14: {
            const std::uint64_t periodo = 7 + variante;
            const std::uint64_t posicion =
                (static_cast<std::uint64_t>(turno) + variante) % periodo;
            resultado = std::round(static_cast<double>(posicion) -
                                   static_cast<double>(periodo - 1) / 2.0);
            break;
        }
        // La función especial de la profe Julia.
        case 15:
            resultado = std::round(std::abs(std::tan(t * t - 1.0)));
            break;
        default:
            throw out_of_range("Familia no valida");
    }
    return convertir_a_entero(resultado);
}

std::string Funcion::formula() const {
    const double k = static_cast<double>(variante);
    const std::uint32_t amplitud = 2 + variante % 4;

    const double a1 = 0.14 + 0.012 * k;
    const double b1 = 0.37 * k;
    const double a2 = 0.29 + 0.006 * k;
    const double b2 = 0.51 * k;

    std::ostringstream texto;

    switch (familia) {
        case 0:
            texto << "f(t) = ⌊t / " << 6 + variante << "⌋";
            break;
        case 1:
            texto << "f(t) = ⌊t² / " << 80 + 12 * variante << "⌋";
            break;
        case 2:
            texto << "f(t) = ⌊t³ / " << 600 + 150 * variante << "⌋";
            break;
        case 3:
            texto << "f(t) = ⌊" << fixed << setprecision(2)
                  << 0.55 + 0.10 * k << "√t⌋";
            break;
        case 4:
            texto << "f(t) = ⌊" << fixed << setprecision(2)
                  << 1.2 + 0.2 * k << "·ln(t + 1)⌋";
            break;
        case 5:
            texto << "f(t) = redondear(" << 4 + variante << "t / (t + "
                  << 8 + variante << "))";
            break;
        case 6:
            texto << "f(t) = redondear(" << 4 + variante
                  << "·(1 - e^(-t / " << 4 + variante << ")))";
            break;
        case 7:
            texto << "f(t) = redondear(" << amplitud << "·sen("
                  << fixed << setprecision(3) << a1 << "t + " << b1 << "))";
            break;
        case 8:
            texto << "f(t) = redondear(" << amplitud << "·cos("
                  << fixed << setprecision(3) << a1 << "t + " << b1 << "))";
            break;
        case 9: {
            const std::uint32_t periodo = 22 + variante;
            texto << "r(t) = (t + " << 2 * variante << ") mod " << periodo << '\n'
                  << "f(t) = redondear(" << amplitud
                  << "·tan(-0.65 + 1.30r(t) / " << periodo - 1 << "))";
            break;
        }
        case 10:
            texto << "f(t) = ⌊t / " << 9 + variante
                  << "⌋ + redondear(3·sen(" << fixed << setprecision(3)
                  << a1 << "t + " << b1 << "))";
            break;
        case 11:
            texto << "f(t) = ⌊0.65√t⌋ + redondear(" << amplitud
                  << "·cos(" << fixed << setprecision(3)
                  << a1 << "t + " << b1 << "))";
            break;
        case 12:
            texto << "f(t) = redondear(3·sen(" << fixed << setprecision(3)
                  << a1 << "t + " << b1 << ") + 2·cos("
                  << a2 << "t + " << b2 << "))";
            break;
        case 13: {
            const uint32_t periodo = 8 + 2 * variante;
            const double mitad = static_cast<double>(periodo - 1) / 2.0;
            texto << "r(t) = (t + " << variante << ") mod " << periodo << '\n'
                  << "f(t) = redondear(5 − 10·|r(t) − " << fixed
                  << setprecision(1) << mitad << "| / " << mitad << ")";
            break;
        }
        case 14: {
            const uint32_t periodo = 7 + variante;
            const double mitad = static_cast<double>(periodo - 1) / 2.0;
            texto << "r(t) = (t + " << variante << ") mod " << periodo << '\n'
                  << "f(t) = redondear(r(t) − " << fixed
                  << setprecision(1) << mitad << ")";
            break;
        }
        case 15:
            texto << "g(t) = |tan(t² − 1)|\nf(t) = redondear(g(t))";
            break;
        default:
            throw std::out_of_range("Familia matemática desconocida");
    }

    return texto.str();
}

int64_t Funcion::cambio_ataque(uint32_t turno) const {
    const int64_t diferencia = resta_saturada(evaluar(turno), evaluar(0));
    return clamp(diferencia, static_cast<int64_t>(-8), static_cast<int64_t>(12));
}

int64_t Funcion::ataque(int64_t ataque_inicial, uint32_t turno) const {
    return max<int64_t>(1, suma_saturada(ataque_inicial, cambio_ataque(turno)));
}

vector<Funcion> generar_funciones() {
    vector<Funcion> funciones;
    funciones.reserve(TOTAL_FUNCIONES);

    for (size_t familia = 0; familia < FAMILIAS_CON_VARIANTES; ++familia) {
        for (uint32_t variante = 1; variante <= VARIANTES_POR_FAMILIA; ++variante) {
            funciones.push_back({familia, variante});
        }
    }

    funciones.push_back({15, 1});  // Función especial #151.
    return funciones;
}
