#include <iostream>
#include <cmath>
#include <leer_cvs.h>
#include <escribe_csv.h>

int main(void)
{

    bool entrenar=false;


    unsigned int nEpocas = 50000;
    unsigned int nPatrones = 100;
    unsigned int nNeuronasEntrada = 3;
    unsigned int nNeuronasOculta1 = 8;
    unsigned int nNeuronasOculta2 = 10;
    unsigned int nNeuronasOculta3 = 8;
    unsigned int nNeuronasSalida = 4;
    double Tolerancia = 0.0001;
    double alpha = 0.1;
    double error_t0 = 1.79769e+308;
    double error_t1 = 1.79769e+308;


    /////tasa de aprendizaje
    double t_aprenizaje = 0.00001;
    double aprenizaje_min = 0.000000001;
    double aprenizaje_max = 0.0001;
    double caidaAprendizaje = 0.9;
    double gradienteAprendizaje = 0.0;


    double target[nPatrones][nNeuronasSalida];

    double** datosEntrada= datosCSV("/home/charles/RedNeuronalProfunda/Entrada_Entrenamiento.csv",nPatrones,nNeuronasEntrada);
    double patEntrada[nPatrones][nNeuronasEntrada];
    for(unsigned int i=0;i<nPatrones;i++)
    {
        for(unsigned int j=0;j<nNeuronasEntrada;j++)
        {
            patEntrada[i][j] = datosEntrada[i][j];
        }
    }
    liberarDatos(datosEntrada);
    double** datosSalida= datosCSV("/home/charles/RedNeuronalProfunda/Salida_Entrenamiento.csv",nPatrones,nNeuronasSalida);
    for(unsigned int i=0;i<nPatrones;i++)
    {
        for(unsigned int j=0;j<nNeuronasSalida;j++)
        {
            target[i][j] = datosSalida[i][j];
        }
        std::cout << std::endl;
    }
    liberarDatos(datosSalida);

    // Inicialización de pesos y bias
    double w1[nNeuronasEntrada][nNeuronasOculta1];
    double w2[nNeuronasOculta1][nNeuronasOculta2];
    double w3[nNeuronasOculta2][nNeuronasOculta3];
    double w4[nNeuronasOculta3][nNeuronasSalida];

    double **w1_opt = new double*[nNeuronasEntrada];
    double **w2_opt = new double*[nNeuronasOculta1];
    double **w3_opt = new double*[nNeuronasOculta2];
    double **w4_opt = new double*[nNeuronasOculta3];

    double **bias1_opt = new double*[nNeuronasOculta1];
    double **bias2_opt = new double*[nNeuronasOculta2];
    double **bias3_opt = new double*[nNeuronasOculta3];
    double **bias4_opt = new double*[nNeuronasSalida];

    double bias1[nNeuronasOculta1];
    double bias2[nNeuronasOculta2];
    double bias3[nNeuronasOculta3];
    double bias4[nNeuronasSalida];

    double delw4[nNeuronasOculta3][nNeuronasSalida];
    double delw3[nNeuronasOculta2][nNeuronasOculta3];
    double delw2[nNeuronasOculta1][nNeuronasOculta2];
    double delw1[nNeuronasEntrada][nNeuronasOculta1];

    double delta3[nPatrones][nNeuronasSalida];
    double delta2[nPatrones][nNeuronasOculta3];
    double delta1[nPatrones][nNeuronasOculta2];
    double delta0[nPatrones][nNeuronasOculta1];

    double SalidaOculta1[nPatrones][nNeuronasOculta1];
    double SalidaOculta2[nPatrones][nNeuronasOculta2];
    double SalidaOculta3[nPatrones][nNeuronasOculta3];
    double SalidaFinal[nPatrones][nNeuronasSalida];


    for(unsigned int x=0;x<nNeuronasOculta3;x++)
    {
        for(unsigned int y=0;y<nNeuronasSalida;y++)
        {
            delw4[x][y]=1;
        }
    }
    for(unsigned int x=0;x<nNeuronasOculta2;x++)
    {
        for(unsigned int y=0;y<nNeuronasOculta3;y++)
        {
            delw3[x][y]=1;
        }
    }
    for(unsigned int x=0;x<nNeuronasOculta1;x++)
    {
        for(unsigned int y=0;y<nNeuronasOculta2;y++)
        {
            delw2[x][y]=1;
        }
    }
    for(unsigned int x=0;x<nNeuronasEntrada;x++)
    {
        for(unsigned int y=0;y<nNeuronasOculta1;y++)
        {
            delw1[x][y]=1;
        }
    }

    // Inicialización aleatoria de pesos

    if(entrenar)
    {
        for (unsigned int i = 0; i < nNeuronasEntrada; i++)
        {
            for (unsigned int j = 0; j < nNeuronasOculta1; j++)
            {
                w1[i][j] = ((rand() % 2000) - 1000) / 1000.0;
            }
        }

        for (unsigned int j = 0; j < nNeuronasOculta1; j++)
        {
            for (unsigned int k = 0; k < nNeuronasOculta2; k++)
            {
                w2[j][k] = ((rand() % 2000) - 1000) / 1000.0;
            }
        }

        for (unsigned int k = 0; k < nNeuronasOculta2; k++)
        {
            for (unsigned int l = 0; l < nNeuronasOculta3; l++)
            {
                w3[k][l] = ((rand() % 2000) - 1000) / 1000.0;
            }
        }

        for (unsigned int l = 0; l < nNeuronasOculta3; l++)
        {
            for (unsigned int m = 0; m < nNeuronasSalida; m++)
            {
                w4[l][m] = ((rand() % 2000) - 1000) / 1000.0;
            }
        }

        for (unsigned int i = 0; i < nNeuronasOculta1; i++)
        {
            bias1[i] = 0.08;
        }
        for (unsigned int i = 0; i < nNeuronasOculta2; i++)
        {
            bias2[i] = 0.08;
        }
        for (unsigned int i = 0; i < nNeuronasOculta3; i++)
        {
            bias3[i] = 0.08;
        }
        for (unsigned int i = 0; i < nNeuronasSalida; i++)
        {
            bias4[i] = 0.08;
        }
    }
    else
    {
        double** cargar_w1= datosCSV("/home/charles/RedNeuronalProfunda/w1.csv",nNeuronasEntrada,nNeuronasOculta1);
        for(unsigned int i=0;i<nNeuronasEntrada;i++)
        {
            for(unsigned int j=0;j<nNeuronasOculta1;j++)
            {
                w1[i][j] = cargar_w1[i][j];
            }
        }
        liberarDatos(cargar_w1);

        double** cargar_w2= datosCSV("/home/charles/RedNeuronalProfunda/w2.csv",nNeuronasOculta1,nNeuronasOculta2);
        for(unsigned int i=0;i<nNeuronasOculta1;i++)
        {
            for(unsigned int j=0;j<nNeuronasOculta2;j++)
            {
                w2[i][j] = cargar_w2[i][j];
            }
        }
        liberarDatos(cargar_w2);

        double** cargar_w3= datosCSV("/home/charles/RedNeuronalProfunda/w3.csv",nNeuronasOculta2,nNeuronasOculta3);
        for(unsigned int i=0;i<nNeuronasOculta2;i++)
        {
            for(unsigned int j=0;j<nNeuronasOculta3;j++)
            {
                w3[i][j] = cargar_w3[i][j];
            }
        }
        liberarDatos(cargar_w3);

        double** cargar_w4= datosCSV("/home/charles/RedNeuronalProfunda/w4.csv",nNeuronasOculta3,nNeuronasSalida);
        for(unsigned int i=0;i<nNeuronasOculta3;i++)
        {
            for(unsigned int j=0;j<nNeuronasSalida;j++)
            {
                w4[i][j] = cargar_w4[i][j];
            }
        }
        liberarDatos(cargar_w4);

        double** cargar_b1= datosCSV("/home/charles/RedNeuronalProfunda/b1.csv",nNeuronasOculta1,1);
        for(unsigned int i=0;i<nNeuronasOculta1;i++)
        {
            for(unsigned int j=0;j<1;j++)
            {
                bias1[i] = cargar_b1[i][j];
            }
        }
        liberarDatos(cargar_b1);

        double** cargar_b2= datosCSV("/home/charles/RedNeuronalProfunda/b2.csv",nNeuronasOculta2,1);
        for(unsigned int i=0;i<nNeuronasOculta2;i++)
        {
            for(unsigned int j=0;j<1;j++)
            {
                bias2[i] = cargar_b2[i][j];
            }
        }
        liberarDatos(cargar_b2);

        double** cargar_b3= datosCSV("/home/charles/RedNeuronalProfunda/b3.csv",nNeuronasOculta3,1);
        for(unsigned int i=0;i<nNeuronasOculta3;i++)
        {
            for(unsigned int j=0;j<1;j++)
            {
                bias3[i] = cargar_b3[i][j];
            }
        }
        liberarDatos(cargar_b3);

        double** cargar_b4= datosCSV("/home/charles/RedNeuronalProfunda/b3.csv",nNeuronasSalida,1);
        for(unsigned int i=0;i<nNeuronasSalida;i++)
        {
            for(unsigned int j=0;j<1;j++)
            {
                bias4[i] = cargar_b4[i][j];
            }
        }
        liberarDatos(cargar_b4);
    }

    // Inicia la red
    for (unsigned int q = 0; q < nEpocas; q++)
    {
        for (unsigned int p = 0; p < nPatrones; p++)
        {
            // Forward
            for (unsigned int j = 0; j < nNeuronasOculta1; j++)
            {
                double suma = bias1[j];
                for (unsigned int i = 0; i < nNeuronasEntrada; i++)
                {
                    suma += w1[i][j] * patEntrada[p][i];
                }
                SalidaOculta1[p][j] = std::max(0.0, suma);
            }
            for (unsigned int k = 0; k < nNeuronasOculta2; k++)
            {
                double suma = bias2[k];
                for (unsigned int j = 0; j < nNeuronasOculta1; j++)
                {
                    suma += w2[j][k] * SalidaOculta1[p][j];
                }
                SalidaOculta2[p][k] = std::max(0.0, suma);
            }
            for (unsigned int l = 0; l < nNeuronasOculta3; l++)
            {
                double suma = bias3[l];
                for (unsigned int k = 0; k < nNeuronasOculta2; k++)
                {
                    suma += w3[k][l] * SalidaOculta2[p][k];
                }
                SalidaOculta3[p][l] = std::max(0.0, suma);
            }
            for (unsigned int m = 0; m < nNeuronasSalida; m++)
            {
                double suma = bias4[m];
                for (unsigned int l = 0; l < nNeuronasOculta3; l++)
                {
                    suma += w4[l][m] * SalidaOculta3[p][l];
                }

                SalidaFinal[p][m] = 1.0 / (1.0 + exp(-suma));
                delta3[p][m] = (target[p][m] - SalidaFinal[p][m]) * SalidaFinal[p][m] * (1.0 - SalidaFinal[p][m]);

            }

            if(entrenar)
            {
                //Backpropagation
                for (unsigned int l = 0; l < nNeuronasOculta3; l++)
                {
                    double suma = 0.0;
                    for (unsigned int m = 0; m < nNeuronasSalida; m++)
                    {
                        suma += delta3[p][m] * w4[l][m];
                    }
                    delta2[p][l] = suma * (SalidaOculta3[p][l] > 0 ? 1.0 : 0.0);
                }
                for (unsigned int k = 0; k < nNeuronasOculta2; k++)
                {
                    double suma = 0.0;
                    for (unsigned int l = 0; l < nNeuronasOculta3; l++)
                    {
                        suma += delta2[p][l] * w3[k][l];
                    }
                    delta1[p][k] = suma * (SalidaOculta2[p][k] > 0 ? 1.0 : 0.0);
                }
                for (unsigned int j = 0; j < nNeuronasOculta1; j++)
                {
                    double suma = 0.0;
                    for (unsigned int k = 0; k < nNeuronasOculta2; k++)
                    {
                        suma += delta1[p][k] * w2[j][k];
                    }
                    delta0[p][j] = suma * (SalidaOculta1[p][j] > 0 ? 1.0 : 0.0);
                }
                //ajuste de pesos

                for (unsigned int m = 0; m < nNeuronasSalida; m++)
                {
                    double suma_delta = 0.0;
                    for (unsigned int n = 0; n < nPatrones; n++)
                    {
                        suma_delta += delta3[n][m];
                    }
                    bias4[m] += t_aprenizaje * suma_delta;
                    for (unsigned int l = 0; l < nNeuronasOculta3; l++)
                    {
                        double suma = 0.0;
                        for (unsigned int n = 0; n < nPatrones; n++)
                        {
                            suma += delta3[n][m] * SalidaOculta3[n][l];
                        }
                        double dw = t_aprenizaje * suma + alpha * delw4[l][m];
                        w4[l][m] += dw;
                        delw4[l][m] = dw;
                    }
                }

                for (unsigned int l = 0; l < nNeuronasOculta3; l++)
                {
                    double suma_delta = 0.0;
                    for (unsigned int n = 0; n < nPatrones; n++)
                    {
                        suma_delta += delta2[n][l];
                    }
                    bias3[l] += t_aprenizaje * suma_delta;
                    for (unsigned int k = 0; k < nNeuronasOculta2; k++)
                    {
                        double suma = 0.0;
                        for (unsigned int n = 0; n < nPatrones; n++)
                        {
                            suma += delta2[n][l] * SalidaOculta2[n][k];
                        }
                        double dw = t_aprenizaje * suma + alpha * delw3[k][l];
                        w3[k][l] += dw;
                        delw3[k][l] = dw;
                    }
                }

                for (unsigned int k = 0; k < nNeuronasOculta2; k++)
                {
                    double suma_delta = 0.0;
                    for (unsigned int n = 0; n < nPatrones; n++)
                    {
                        suma_delta += delta1[n][k];
                    }
                    bias2[k] += t_aprenizaje * suma_delta;
                    for (unsigned int j = 0; j < nNeuronasOculta1; j++)
                    {
                        double suma = 0.0;
                        for (unsigned int n = 0; n < nPatrones; n++)
                        {
                            suma += delta1[n][k] * SalidaOculta1[n][j];
                        }
                        double dw = t_aprenizaje * suma + alpha * delw2[j][k];
                        w2[j][k] += dw;
                        delw2[j][k] = dw;
                    }
                }


                for (unsigned int j = 0; j < nNeuronasOculta1; j++)
                {
                    double suma_delta = 0.0;
                    for (unsigned int n = 0; n < nPatrones; n++)
                    {
                        suma_delta += delta0[n][j];
                    }
                    bias1[j] += t_aprenizaje * suma_delta;
                    for (unsigned int i = 0; i < nNeuronasEntrada; i++)
                    {
                        double suma = 0.0;

                        for (unsigned int n = 0; n < nPatrones; n++)
                        {
                            suma += delta0[n][j] * patEntrada[n][i];
                        }
                        double dw = t_aprenizaje * suma + alpha * delw1[i][j];
                        w1[i][j] += dw;
                        delw1[i][j] = dw;
                    }
                }
            }

        }


        if(entrenar)
        {
            alpha = fmax(alpha * 0.99, 0.01);
        }

        if (q % 100 == 0)
        {
            if(error_t0<error_t1)
            {
                // Guarda los pesos y bias con menor error
                for(unsigned int x=0;x<nNeuronasSalida;x++)
                {
                    bias4_opt[x] = new double[1];
                    for(unsigned int y=0;y<1;y++)
                    {
                        bias4_opt[x][y] = bias4[x];
                    }
                }
                for(unsigned int x=0;x<nNeuronasOculta3;x++)
                {
                    bias3_opt[x] = new double[1];
                    for(unsigned int y=0;y<1;y++)
                    {
                        bias3_opt[x][y] = bias3[x];
                    }
                }
                for(unsigned int x=0;x<nNeuronasOculta2;x++)
                {
                    bias2_opt[x] = new double[1];
                    for(unsigned int y=0;y<1;y++)
                    {
                        bias2_opt[x][y] = bias2[x];
                    }
                }
                for(unsigned int x=0;x<nNeuronasOculta1;x++)
                {
                    bias1_opt[x] = new double[1];
                    for(unsigned int y=0;y<1;y++)
                    {
                        bias1_opt[x][y] = bias1[x];
                    }
                }

                for (unsigned int i = 0; i < nNeuronasEntrada; i++)
                {
                    w1_opt[i] = new double[nNeuronasOculta1];
                    for (unsigned int j = 0; j < nNeuronasOculta1; j++)
                    {
                        w1_opt[i][j] = w1[i][j];
                    }
                }

                for (unsigned int j = 0; j < nNeuronasOculta1; j++)
                {
                    w2_opt[j] = new double[nNeuronasOculta2];
                    for (unsigned int k = 0; k < nNeuronasOculta2; k++)
                    {
                        w2_opt[j][k] = w2[j][k];
                    }
                }

                for (unsigned int k = 0; k < nNeuronasOculta2; k++)
                {
                    w3_opt[k] = new double[nNeuronasOculta1];
                    for (unsigned int l = 0; l < nNeuronasOculta3; l++)
                    {
                        w3_opt[k][l] = w3[k][l];
                    }
                }

                for (unsigned int l = 0; l < nNeuronasOculta3; l++)
                {
                    w4_opt[l] = new double[nNeuronasOculta1];
                    for (unsigned int m = 0; m < nNeuronasSalida; m++)
                    {
                        w4_opt[l][m] = w4[l][m];
                    }
                }
            }
            error_t0 = error_t1;
            error_t1 = 0.0;
            for (unsigned int p = 0; p < nPatrones; p++)
            {
                for (unsigned int k = 0; k < nNeuronasSalida; k++)
                {
                    error_t1 += (target[p][k] - SalidaFinal[p][k])*(target[p][k] - SalidaFinal[p][k]);
                }
            }
            error_t1 = error_t1 / nPatrones;
            if(entrenar)
            {
                gradienteAprendizaje= error_t0 - error_t1;
                gradienteAprendizaje = gradienteAprendizaje*gradienteAprendizaje;
                t_aprenizaje = aprenizaje_max / (1 + caidaAprendizaje * gradienteAprendizaje);

                t_aprenizaje = std::max(t_aprenizaje, aprenizaje_min);
            }



            std:: cout << "Época:" <<  q << " Error:" <<  error_t1 << " t_aprenizaje:" << t_aprenizaje <<std::endl;
        }
        if(entrenar)
        {
            if (error_t1 < Tolerancia)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    if(entrenar)
    {
        escribirCSV("/home/charles/RedNeuronalProfunda/w1.csv",nNeuronasEntrada,nNeuronasOculta1,w1_opt);
        escribirCSV("/home/charles/RedNeuronalProfunda/w2.csv",nNeuronasOculta1,nNeuronasOculta2,w2_opt);
        escribirCSV("/home/charles/RedNeuronalProfunda/w3.csv",nNeuronasOculta2,nNeuronasOculta3,w3_opt);
        escribirCSV("/home/charles/RedNeuronalProfunda/w4.csv",nNeuronasOculta3,nNeuronasSalida,w4_opt);

        escribirCSV("/home/charles/RedNeuronalProfunda/b1.csv",nNeuronasOculta1,1,bias1_opt);
        escribirCSV("/home/charles/RedNeuronalProfunda/b2.csv",nNeuronasOculta2,1,bias2_opt);
        escribirCSV("/home/charles/RedNeuronalProfunda/b3.csv",nNeuronasOculta3,1,bias3_opt);
        escribirCSV("/home/charles/RedNeuronalProfunda/b4.csv",nNeuronasSalida,1,bias4_opt);
    }
    unsigned int bienClasificados=0;
    std:: cout << "Resultados finales:" << std::endl;
    for (unsigned int p = 0; p < nPatrones; p++)
    {
        double max=0;
        unsigned int i=0;
        for (unsigned int m = 0; m < nNeuronasSalida; m++)
        {
            if(SalidaFinal[p][m]>max)
            {
                max=SalidaFinal[p][m];
                i=m;
            }
        }
        for (unsigned int m = 0; m < nNeuronasSalida; m++)
        {
            if(m==i)
            {
                SalidaFinal[p][m]=1;
            }
            else
            {
                SalidaFinal[p][m]=0;
            }
        }
        if(SalidaFinal[p][0]==target[p][0] && SalidaFinal[p][1]==target[p][1] && SalidaFinal[p][2]==target[p][2] && SalidaFinal[p][3]==target[p][3])
        {
            bienClasificados++;
            std::cout << "Patrón: " << p << "\t BIEN" << std::endl;
        }
        else
        {
            std::cout << "Patrón: " << p << "\t MAL" << std::endl;
        }


        for (unsigned int m = 0; m < nNeuronasSalida; m++)
        {
            std::cout << "Salida esperada= " << target[p][m] <<"\tSalida obtenida= " << SalidaFinal[p][m] << std::endl;
        }
    }
    std::cout << "Bien clasificados= " << bienClasificados << std::endl;

    bienClasificados=0;
    unsigned int nPatronesRec = 48;
    datosEntrada= datosCSV("/home/charles/RedNeuronalProfunda/Reconocimiento_Entrada.csv",nPatrones,nNeuronasEntrada);
    double patRec[nPatronesRec][nNeuronasEntrada];
    for(unsigned int i=0;i<nPatronesRec;i++)
    {
        for(unsigned int j=0;j<nNeuronasEntrada;j++)
        {
            patRec[i][j] = datosEntrada[i][j];
        }
    }
    liberarDatos(datosEntrada);
    datosSalida= datosCSV("/home/charles/RedNeuronalProfunda/Reconocimiento_Salida.csv",nPatrones,nNeuronasSalida);
    double salidaRec[nPatrones][nNeuronasSalida];
    for(unsigned int i=0;i<nPatronesRec;i++)
    {
        for(unsigned int j=0;j<nNeuronasSalida;j++)
        {
            salidaRec[i][j] = datosSalida[i][j];
        }
        std::cout << std::endl;
    }
    liberarDatos(datosSalida);
    // Inicia la red reconocimiento

    for (unsigned int p = 0; p < nPatronesRec; p++)
    {
        // Forward
        for (unsigned int j = 0; j < nNeuronasOculta1; j++)
        {
            double suma = bias1[j];
            for (unsigned int i = 0; i < nNeuronasEntrada; i++)
            {
                suma += w1[i][j] * patRec[p][i];
            }
            SalidaOculta1[p][j] = std::max(0.0, suma);
        }
        for (unsigned int k = 0; k < nNeuronasOculta2; k++)
        {
            double suma = bias2[k];
            for (unsigned int j = 0; j < nNeuronasOculta1; j++)
            {
                suma += w2[j][k] * SalidaOculta1[p][j];
            }
            SalidaOculta2[p][k] = std::max(0.0, suma);
        }
        for (unsigned int l = 0; l < nNeuronasOculta3; l++)
        {
            double suma = bias3[l];
            for (unsigned int k = 0; k < nNeuronasOculta2; k++)
            {
                suma += w3[k][l] * SalidaOculta2[p][k];
            }
            SalidaOculta3[p][l] = std::max(0.0, suma);
        }
        for (unsigned int m = 0; m < nNeuronasSalida; m++)
        {
            double suma = bias4[m];
            for (unsigned int l = 0; l < nNeuronasOculta3; l++)
            {
                suma += w4[l][m] * SalidaOculta3[p][l];
            }

            SalidaFinal[p][m] = 1.0 / (1.0 + exp(-suma));

        }

        double max=0;
        unsigned int i=0;
        for (unsigned int m = 0; m < nNeuronasSalida; m++)
        {
            if(SalidaFinal[p][m]>max)
            {
                max=SalidaFinal[p][m];
                i=m;
            }
        }
        for (unsigned int m = 0; m < nNeuronasSalida; m++)
        {
            if(m==i)
            {
                SalidaFinal[p][m]=1;
            }
            else
            {
                SalidaFinal[p][m]=0;
            }
        }
        if(SalidaFinal[p][0]==salidaRec[p][0] && SalidaFinal[p][1]==salidaRec[p][1] && SalidaFinal[p][2]==salidaRec[p][2] && SalidaFinal[p][3]==salidaRec[p][3])
        {
            bienClasificados++;
            std::cout << "Patrón: " << p+1 << "\t BIEN" << std::endl;
        }
        else
        {
            std::cout << "Patrón: " << p+1 << "\t MAL" << std::endl;
        }


        for (unsigned int m = 0; m < nNeuronasSalida; m++)
        {
            std::cout << "Salida esperada= " << salidaRec[p][m] <<"\tSalida obtenida= " << SalidaFinal[p][m] << std::endl;
        }
    }
    std::cout << "Bien clasificados= " << bienClasificados << std::endl;

    return 0;
}

