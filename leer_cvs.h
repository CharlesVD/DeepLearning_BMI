#include <fstream>
#include <iostream>
#include <sstream>

#ifndef LEER_CSV_H
#define LEER_CSV_H

double** datosCSV(const char* dir, unsigned int noDatos, unsigned int dimensiones)
{
    std::ifstream archivo(dir);
    if (!archivo)
    {
        std::cerr << "Error: No se pudo abrir el archivo " << dir << std::endl;
        return nullptr;
    }

    // Reservar un solo bloque de memoria contiguo
    double** datos = new double*[noDatos];
    double* dataBlock = new double[noDatos * dimensiones];

    // Ajustar los punteros de fila
    for (unsigned int i = 0; i < noDatos; i++)
    {
        datos[i] = dataBlock + (i * dimensiones);
    }

    std::string linea;
    unsigned int dato = 0;

    // Cargar datos del CSV
    while (std::getline(archivo, linea) && dato < noDatos)
    {
        std::stringstream ss(linea);
        std::string valor;
        unsigned int dim = 0;

        while (std::getline(ss, valor, ',') && dim < dimensiones)
        {
            try
            {
                datos[dato][dim] = std::stod(valor);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error al convertir: " << valor << " en línea " << dato + 1 << std::endl;
                datos[dato][dim] = 0.0; // Valor predeterminado
            }
            dim++;
        }
        dato++;
    }

    archivo.close();
    return datos;
}

// Función para liberar memoria contigua
void liberarDatos(double** datos)
{
    if (datos)
    {
        delete[] datos[0]; // Liberar el bloque contiguo
        delete[] datos;    // Liberar la lista de punteros
    }
}
#endif // LEER_CSV_H
