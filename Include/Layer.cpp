#include "Layer.h"

namespace nn
{
	template <typename _T>
	Layer<_T>::Layer()
	{
		this->activation = "sigmoid";
	}
	template <typename _T>
	Layer<_T>::~Layer()
	{

	}

	template <typename _T>
	Layer<_T>& Layer<_T>::init(int rows, int cols, string activation)
	{
		this->activation = activation;
		// creating neurons and their derivative with zero value
		neurons.Create(1, cols, 0);
		derivative.Create(1, cols, 0);

		// creating weigth matrixes with random values
		weights.Create(rows, cols);
		weightDif.Create(rows, cols, 0);
		biasWeights.Create(1, cols);
		biasWeightDif.Create(1, cols, 0);
		// creating delta matrix with zero value; used to update weights
		delta.Create(1, cols, 0);

		return *this;
	}

	template <typename _T>
	Layer<_T>& Layer<_T>::activate()
	{
		// get the pointer for the first row 
		_T* n = neurons.getDataRow(0);
		if (activation == "sigmoid")
			for (int i = 0; i < neurons.getCols(); i++)
				n[i] = 1 / (1 + exp(-n[i]));
		else if (activation == "relu")
			for (int i = 0; i < neurons.getCols(); i++)
				n[i] = (n[i] > 0.0 ? n[i] : 0);
		else if (activation == "linear")
			for (int i = 0; i < neurons.getCols(); i++)
				n[i] = n[i];
		else if (activation == "tanh")
			for (int i = 0; i < neurons.getCols(); i++)
				n[i] = tanh(n[i]);
		else
			for (int i = 0; i < neurons.getCols(); i++)
				n[i] = n[i] > 0; // hard-limiter
		return *this;
	}
	template <typename _T>
	Layer<_T>& Layer<_T>::derived()
	{
		// get the pointer for the first row 
		_T* n = neurons.getDataRow(0);
		_T* d = derivative.getDataRow(0);
		if (activation == "sigmoid")
			for (int i = 0; i < neurons.getCols(); i++)
				d[i] = n[i] * (1 - n[i]);
		else if (activation == "relu")
			for (int i = 0; i < neurons.getCols(); i++)
				d[i] = n[i] > 0;
		else if (activation == "linear")
			for (int i = 0; i < neurons.getCols(); i++)
				d[i] = 1;
		else if (activation == "tanh")
			for (int i = 0; i < neurons.getCols(); i++)
				d[i] = 1 - pow(n[i], 2);
		else
			for (int i = 0; i < neurons.getCols(); i++)
				d[i] = 1; // hard-limiter
		return *this;
	}

	template <typename _T>
	Matrix<_T>* Layer<_T>::getNeurons()
	{
		return &neurons;
	}
	template <typename _T>
	Matrix<_T>* Layer<_T>::getDerivative()
	{
		return &derivative;
	}

	template <typename _T>
	Matrix<_T>* Layer<_T>::getWeights()
	{
		return &weights;
	}
	template <typename _T>
	Matrix<_T>* Layer<_T>::getWeightDif()
	{
		return &weightDif;
	}
	template <typename _T>
	Matrix<_T>* Layer<_T>::getBiasWeights()
	{
		return &biasWeights;
	}
	template <typename _T>
	Matrix<_T>* Layer<_T>::getBiasWeightDif()
	{
		return &biasWeightDif;
	}
	template <typename _T>
	Matrix<_T>* Layer<_T>::getDelta()
	{
		return &delta;
	}

	template class Layer<float>;
	template class Layer<double>;
}
