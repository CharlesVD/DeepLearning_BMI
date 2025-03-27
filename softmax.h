#include <cmath>

#ifndef SOFTMAX_H
#define SOFTMAX_H

void softmax(double* input, double* output, int size) {
    double sum_exp = 0.0;
    double exp_values[size];

    // Calcular e^x para cada entrada
    for (int i = 0; i < size; i++)
    {
        exp_values[i] = std::exp(input[i]);
        sum_exp += exp_values[i];
    }

    // Normalizar dividiendo por la suma total
    for (int i = 0; i < size; i++)
    {
        output[i] = exp_values[i] / sum_exp;
    }
}

#endif // SOFTMAX_H
