TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

DISTFILES += \
    Entrada_Entrenamiento.csv \
    Reconocimiento_Entrada.csv \
    Reconocimiento_Salida.csv \
    Salida_Entrenamiento.csv \
    b1.csv \
    b2.csv \
    b3.csv \
    b4.csv \
    w1.csv \
    w2.csv \
    w3.csv \
    w4.csv

HEADERS += \
    escribe_csv.h \
    leer_cvs.h
