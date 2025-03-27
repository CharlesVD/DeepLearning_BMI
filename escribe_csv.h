#include <fstream>
#include <iostream>
#include <sstream>

#ifndef ESCRIBE_CSV_H
#define ESCRIBE_CSV_H


void escribirCSV(const char* dir, unsigned int yCapa, unsigned int XCapa,double **datos)
{
    std::ofstream archivo(dir);
    if (!archivo)
    {
        std::cerr << "Error: No se pudo abrir el archivo para escritura: " << dir << std::endl;
        return;
    }

    for (unsigned int i = 0; i < yCapa; i++)
    {
        for (unsigned int j = 0; j < XCapa; j++)
        {
            archivo << datos[i][j];
            if (j < XCapa - 1) archivo << ","; // Separador de columnas
        }
        archivo << "\n"; // Nueva línea para la siguiente fila
    }

    archivo.close();
    std::cout << "Archivo guardado correctamente en: " << dir << std::endl;
}

#endif // ESCRIBE_CSV_H
