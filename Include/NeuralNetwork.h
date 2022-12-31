#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <chrono>
#include "SepMath.h"
#include "Layer.h"

using std::string;
using std::vector;
using sep::Matrix;
using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

namespace nn
{
	template <typename _T>
	class NeuralNetwork
	{
	public:
		NeuralNetwork();
		~NeuralNetwork();

		void save(string filename);
		void load(string filename);
		void summary(); // for future
		NeuralNetwork& addLayer(int units, string activation = "sigmoid", int input_shape = -1);
		NeuralNetwork& compile(_T lr = 0.1, _T decay = 0.01, _T momentum = 0, string optimizer = "rms");
		Matrix<_T>& fit(Matrix<_T>* data, Matrix<_T>* label, int epochs, int batch_size = 1);
		
		void evaluate(Matrix<_T>& data, Matrix<_T>& label);
		void predict(Matrix<_T>& input, Matrix<_T>& output);
		int predict_class(Matrix<_T>& input);
	private:
		void feed_forward();
		void set_error();
		void back_propagation();
		void update_weights();
		void set_total_error();
	private:
		// learning parameters
		_T lr;
		_T decay;
		_T alpha; // momentum
		string optimizer;

		// network topology
		int input_shape;
		int output_shape;
		int output_layer_index;
		vector<int> layer_shape;
		vector<string> layer_act;
		nn::Layer<_T>* layers;

		// training data
		Matrix<_T>* train;
		Matrix<_T>* train_label;
		Matrix<_T> input;
		Matrix<_T> target;
		Matrix<_T> output_error;
		
		int batch_index;
		Matrix<_T> batch_input;
		Matrix<_T> batch_error;

		// error and accuracy for each epoch; in [set_error()] error for each 
		// sample is calculated; in [set_total_error()] epoch's error is determined
		// in [history] matrix; the columns represent epochs
		// first row is total error; second row is total accuracy
		double error;
		double accuracy;
		Matrix<_T> history;
	};
}
