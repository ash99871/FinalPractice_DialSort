#include <iostream>
#include <vector>
#include <chrono>
#include "dialSort.cpp"

using namespace std;

int main()
{
    // numero/tamaño del dataset
    vector<size_t> Ns = {100000};
    // universo de valores
    vector<int> Us = {1000};

    for (auto n : Ns) {
        for (auto U : Us) {

            cout << "\n========================\n";
            cout << "N = " << n << " | U = " << U << endl;

            // generar dataset
            auto datos = gen_uniform(n, U, 123);

            // mostrar 20 desordenados
            cout << "\nPrimeros 20 datos DESORDENADOS:\n";
            for (int i = 0; i < 20; i++) {
                cout << datos[i] << " ";
            }
            cout << endl;

            // dialSort
            auto start = chrono::high_resolution_clock::now();
            dialsort(datos);
            auto end = chrono::high_resolution_clock::now();

            double time_dial = chrono::duration<double, milli>(end - start).count();

            // 🔹 Mostrar 50 ordenados
            cout << "\nPrimeros 50 datos ORDENADOS:\n";
            for (int i = 0; i < 300; i++) {
                cout << datos[i] << " ";
            }
            cout << endl;

            // tiempo
            cout << "\nDialSort Tiempo: " << time_dial << " ms" << endl;
        }
    }

    return 0;
}
