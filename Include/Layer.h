#pragma once
#include <iostream>
#include "SepMath.h"

using std::string;
using sep::Matrix;

namespace nn
{
	template <typename _T>
	class Layer
	{
	public:
		Layer();
		~Layer();
		// rows = previous layer neurons
		// cols = current layer neurons
		Layer& init(int rows, int cols, string activation = "sigmoid");
		// activate the current values of neurons with layer's activation function
		Layer& activate();
		Layer& derived();

		Matrix<_T>* getNeurons();
		Matrix<_T>* getDerivative();

		Matrix<_T>* getWeights();
		Matrix<_T>* getWeightDif();
		Matrix<_T>* getDelta();
		Matrix<_T>* getBiasWeights();
		Matrix<_T>* getBiasWeightDif();
	private:
		string activation;
		Matrix<_T> neurons;
		Matrix<_T> derivative;

		Matrix<_T> weights;
		Matrix<_T> weightDif;
		Matrix<_T> biasWeights;
		Matrix<_T> biasWeightDif;
		Matrix<_T> delta;
	};
}
