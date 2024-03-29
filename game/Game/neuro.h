#pragma once

#include "Enums.h"

class NeuralNet
{
public:
    NeuralNet(uint8_t L, uint16_t* n);
    void Forward(uint16_t size, double* data);
    void getResult(uint16_t size, double* data);
    void learnBackpropagation(double* data, double* ans, double acs, double k);
private:
    std::vector<std::vector<std::vector<double>>> neurons;
    std::vector<std::vector<std::vector<double>>> weights;
    uint8_t numLayers;
    std::vector<double> neuronsInLayers;
    double Func(double in);
    double Func_p(double in);
    uint32_t MaxEl(uint16_t size, uint16_t* arr);
    void CreateNeurons(uint8_t L, uint16_t* n);
    void CreateWeights(uint8_t L, uint16_t* n);

};