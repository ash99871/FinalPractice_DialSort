#include "benchmark.h"
#include "dialSort.h"
#include "radixSort.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <atomic>
#include <cstdlib>
#include <cstddef>
using namespace std;


std::atomic<size_t> memoria_actual{0};
std::atomic<size_t> memoria_pico{0};

void* operator new(size_t size) {
    memoria_actual += size;
    if (memoria_actual > memoria_pico) memoria_pico = (size_t)memoria_actual;
    return malloc(size);
}

void operator delete(void* memory, size_t size) noexcept {
    memoria_actual -= size;
    free(memory);
}

void compararCaso(string nombre, vector<int> datos, int U) {

    vector<double> tiemposDial;
    vector<double> tiemposRadix;

    size_t n = datos.size();

    for (int i = 0; i < 5; i++) {

        auto d1 = datos;
        auto d2 = datos;

        // DialSort
        auto start1 = chrono::high_resolution_clock::now();
        dialsort(d1);
        auto end1 = chrono::high_resolution_clock::now();

        double t1 = chrono::duration<double, milli>(end1 - start1).count();
        tiemposDial.push_back(t1);

        // RadixSort
        auto start2 = chrono::high_resolution_clock::now();
        radixSort(d2);
        auto end2 = chrono::high_resolution_clock::now();

        double t2 = chrono::duration<double, milli>(end2 - start2).count();
        tiemposRadix.push_back(t2);
    }

    // calcular promedio
    auto promedio = [](vector<double>& v) {
        double sum = 0;
        for (double x : v) sum += x;
        return sum / v.size();
    };

    // calcular desviacion estandar
    auto desviacion = [](vector<double>& v, double mean) {
        double sum = 0;
        for (double x : v) sum += (x - mean)*(x - mean);
        return sqrt(sum / v.size());
    };

    double meanDial = promedio(tiemposDial);
    double meanRadix = promedio(tiemposRadix);

    double stdDial = desviacion(tiemposDial, meanDial);
    double stdRadix = desviacion(tiemposRadix, meanRadix);

    // calcular datos procesados por segundo
    double thDial = n / (meanDial / 1000.0);
    double thRadix = n / (meanRadix / 1000.0);

    cout << "\n========================\n";
    cout << "--- " << nombre << " ---\n\n";

    cout << "ALGORITMO        Promedio(ms)   Desviacion Estandar     Datos por segundo\n";
    cout << "---------------------------------------------------------------------------\n";

    cout << "DialSort         "
         << meanDial << "            "
         << stdDial << "            "
         << thDial << endl;

    cout << "RadixSort        "
         << meanRadix << "            "
         << stdRadix << "            "
         << thRadix << endl;

    cout << "\nCOMPARACION:\n";

    if (meanDial < meanRadix)
        cout << "-- Mejor en tiempo: DialSort\n";
    else
        cout << "-- Mejor en tiempo: RadixSort\n";

    cout << "Memoria teórica DialSort: O(U) = " << U << endl;
    cout << "Memoria teórica RadixSort: O(n) = " << n << endl;

    if (U < n)
        cout << "-- Mejor en memoria teórica: DialSort\n";
    else
        cout << "-- Mejor en memoria teórica: RadixSort\n";

    //Memoria real
    memoria_pico = 0; 
    {
        auto d1 = datos; 
        dialsort(d1);    
    }
    size_t picoDial = memoria_pico;

    
    memoria_pico = 0; 
    {
        auto d2 = datos; 
        radixSort(d2);   
    }
    size_t picoRadix = memoria_pico;

    
    cout << "\nANALISIS DE MEMORIA (Pico de uso):" << endl;
    cout << "DialSort:  " << picoDial << " bytes" << endl;
    cout << "RadixSort: " << picoRadix << " bytes" << endl;

    if (picoDial < picoRadix) {
        double ahorro = (double)(picoRadix - picoDial) / picoRadix * 100.0;
        cout << ">> Ganador en Memoria Real: DialSort (Usa " << ahorro << "% menos RAM)" << endl;
    } else if (picoRadix < picoDial) {
        double ahorro = (double)(picoDial - picoRadix) / picoDial * 100.0;
        cout << ">> Ganador en Memoria Real: RadixSort (Usa " << ahorro << "% menos RAM)" << endl;
    } else {
        cout << ">> Empate técnico en uso de memoria real." << endl;
    }
    
}

void Benchmark::run() {

    size_t n = 1000000;
    int U = 1000;

    auto uniform = gen_uniform(n, U, 123);
    auto skewed  = gen_skewed(n, U, 123);
    auto sorted  = gen_sorted(n, U, 123);
    auto reverse = gen_reverse(n, U, 123);

    compararCaso("UNIFORME (numeros desordenados aleatoriamente)", uniform, U);
    compararCaso("MUCHOS REPETIDOS (SKEWED)", skewed, U);
    compararCaso("ORDENADOS", sorted, U);
    compararCaso("INVERSO", reverse, U);
}
