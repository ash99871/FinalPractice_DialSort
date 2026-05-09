#include <iostream>
#include "benchmark.h"

using namespace std;


int main() {
    

    cout << "Iniciando prueba." << endl;
    cout << "Configuracion: n = 1,000,000 elementos, U = 1,000 de rango." << endl;

    
    Benchmark::run();

    cout << "\nPrueba finalizada con exito." << endl;

    return 0;
}

