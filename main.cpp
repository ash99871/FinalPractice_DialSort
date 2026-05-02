#include <iostream>
#include <vector>
#include <chrono>
#include "dialSort.cpp"

using namespace std;

int main() {
    vector<size_t> Ns = {100000};
    vector<int> Us = {1000};

    for (auto n : Ns) {
        for (auto U : Us) {
            cout << "\n========================\n";
            cout << "N = " << n << " | U = " << U << endl;

            
            const auto datos = gen_uniform(n, U, 123);

            // --- PRUEBA DIALSORT ---
            auto datos_dial = datos; 
            
            auto start = chrono::high_resolution_clock::now();
            dialsort(datos_dial);
            auto end = chrono::high_resolution_clock::now();
            
            double time_dial = chrono::duration<double, milli>(end - start).count();
            cout << "DialSort Tiempo: " << time_dial << " ms" << endl;

            // --- PRUEBA RADIXSORT ---
            auto datos_radix = datos; 
            
            auto one = chrono::high_resolution_clock::now();
            radixSort(datos_radix);
            auto two = chrono::high_resolution_clock::now();
            
            double time_radix = chrono::duration<double, milli>(two - one).count();
            cout << "RadixSort Tiempo: " << time_radix << " ms" << endl;
            
            
        }
    }
    return 0;
}
