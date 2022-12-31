#include "NeuralNetwork.h"

namespace nn
{
	template <typename _T>
	NeuralNetwork<_T>::NeuralNetwork()
	{
		lr = 0.1;
		decay = 0.01;
		alpha = 0;
		input_shape = 0;
		output_shape = 0;
		output_layer_index = 0;
		layers = nullptr;
		train = nullptr;
		train_label = nullptr;
		batch_index = 0;
		error = 0;
		accuracy = 0;
	}
	template <typename _T>
	NeuralNetwork<_T>::~NeuralNetwork()
	{
		if (layers != nullptr)
		{
			delete[] layers;
			layers = nullptr;
		}
	}


	template <typename _T>
	void NeuralNetwork<_T>::save(string filename)
	{
		std::ofstream file;
		file.open(filename);
		file << "Neural Network - Opt: " << optimizer << std::endl;
		file << "Layers: " << layer_shape.size() << std::endl;
		for (int i = 0; i < layer_shape.size(); i++)
		{
			if (i == 0)
				file << "Layer_" << i << ": (" << this->input_shape << ", " << this->layer_shape.at(i) << ")" << std::endl;
			else 
				file << "Layer_" << i << ": (, " << this->layer_shape.at(i) << ")" << std::endl;
			file << "Act: " << this->layer_act.at(i) << std::endl;
			file << "Weights: " << std::endl;
			this->layers[i].getWeights()->print(file, "matrix");
			file << "Bias Weights: " << std::endl;
			this->layers[i].getBiasWeights()->print(file, "matrix");
		}
		file.close();
	}
	template <typename _T>
	void NeuralNetwork<_T>::load(string filename)
	{
		if (layers != nullptr) // network already loaded
			return;
		std::ifstream file;
		file.open(filename);
		if (file.is_open())
		{
			layer_shape.clear();
			layer_act.clear();
			
			int layer_num;
			string str;
			std::getline(file, str); // Neural Network - Opt: optimizer
			optimizer = str.substr(str.find(": ") + 2);
			std::getline(file, str); // Layers: Num
			layer_num = atoi(str.substr(str.find(": ") + 2).c_str());
			layers = new nn::Layer<_T>[layer_num];
			output_layer_index = layer_num - 1;

			for (int i = 0; i < layer_num; i++)
			{
				int index;
				int index2;
				std::getline(file, str); // Layer_Num: (row, col)
				if (i == 0)
				{
					index = str.find("(") + 1;
					index2 = str.find(", ");
					this->input_shape = atoi(str.substr(index, index2 - index).c_str());
				}
				index = str.find(", ") + 2;
				index2 = str.find(")");
				layer_shape.push_back(atoi(str.substr(index, index2 - index).c_str()));

				std::getline(file, str); // Act:
				layer_act.push_back(str.substr(str.find(": ") + 2));

				if (i == 0)
					layers[0].init(input_shape, layer_shape.at(0), layer_act.at(0));
				else 
					layers[i].init(layer_shape.at(i - 1), layer_shape.at(i), layer_act.at(i));

				std::getline(file, str); // Weights:
				sep::read_matirx_data(file, layers[i].getWeights());
				std::getline(file, str); // Bias Weights: 
				sep::read_matirx_data(file, layers[i].getBiasWeights());
			}
			this->output_shape = layer_shape.at(output_layer_index);
			this->input.Create(1, input_shape, 0);
			this->target.Create(1, output_shape, 0);
			this->output_error.Create(1, output_shape, 0);
			file.close();
		}
		else
			std::cout << "Error Opening File \"" << filename << "\"!!!" << std::endl;
	}

	template <typename _T>
	NeuralNetwork<_T>& NeuralNetwork<_T>::addLayer(int units, string activation, int input_shape)
	{
		if (this->input_shape == 0 && input_shape >= 1)
			this->input_shape = input_shape;
		layer_shape.push_back(units);
		layer_act.push_back(activation);
		return *this;
	}

	template <typename _T>
	NeuralNetwork<_T>& NeuralNetwork<_T>::compile(_T lr, _T decay, _T momentum, string optimizer)
	{
		if (lr > 0)
			this->lr = lr;
		if (decay > 0)
			this->decay = decay;
		if (momentum >= 0 && momentum <= 1)
			this->alpha = momentum;
		this->optimizer = optimizer;

		if (layers != nullptr) // compiling twice will only change train parameters
			return *this;
		// layers are the layers after the input layer
		// so: perceptron has one layer [layers[0]] which is the output;
		// with one neuron and input_shape * 1 weights
		layers = new nn::Layer<_T>[layer_shape.size()];

		// layers[0] is the first hidden layer after input; in order to determine the weigth matrix
		// we need the neuron shape from previous layer(input layer)[input_shape] and the neuron shape for current layer
		// the layer neuron shape and activation are stored in [layer_shape, layer_act]
		layers[0].init(input_shape, layer_shape.at(0), layer_act.at(0));

		// creating the layers after the first hidden layer (the last layer created is the output layer)
		// if layer_shape.size() is 1 the previous layer [layers[0]] is the output layer
		this->output_layer_index = layer_shape.size() - 1;
		this->output_shape = layer_shape.at(output_layer_index);
		for (int i = 1; i < layer_shape.size(); i++)
			layers[i].init(layer_shape.at(i - 1), layer_shape.at(i), layer_act.at(i));

		return *this;
	}

	template <typename _T>
	Matrix<_T>& NeuralNetwork<_T>::fit(Matrix<_T>* data, Matrix<_T>* label, int epochs, int batch_size)
	{
		if (data->getCols() != input_shape ||
			label->getCols() != output_shape)
			throw std::invalid_argument("Train or Target shape mismatch!!!");
		if (batch_size <= 0)
			throw std::invalid_argument("Invalid Batch Size!!!");

		this->train = data;
		this->train_label = label;
		this->input.Create(1, input_shape, 0);
		this->target.Create(1, output_shape, 0);
		this->output_error.Create(1, output_shape, 0);

		this->batch_input.Create(batch_size, input_shape, 0);
		this->batch_error.Create(batch_size, output_shape, 0);

		int update_rate = epochs / 5 < 3 ? 1 : 5;
		for (int i = 1; i <= epochs; i++)
		{
			auto timer_start = high_resolution_clock::now();

			// before starting each epoch the error and accuracy are reset
			// the error and accuracy are updated for each sample; in the end the correct value is calculated
			this->error = 0;
			this->accuracy = 0;
			this->batch_index = 0;
			for (int iteration = 0; iteration < train->getRows(); iteration++)
			{
				train->get_row(iteration, input);
				train_label->get_row(iteration, target);
				feed_forward();
				set_error();
				batch_input.set_row(batch_index, input);
				batch_error.set_row(batch_index, output_error);
				batch_index++;

				if (batch_index == batch_size || iteration == train->getRows() - 1)
				{
					for (int index = 0; index < batch_index; index++)
					{
						batch_input.get_row(index, input);
						batch_error.get_row(index, output_error);
						back_propagation();
						update_weights();
					}
					batch_index = 0;
				}
			}
			set_total_error();

			// updating lr
			if (i % update_rate == 0)
				lr = (lr * 0.9 > decay ? lr * 0.9 : decay);
			// print quick evaluation - every 5 epoch
			if (i % update_rate == 0)
			{
				auto end = high_resolution_clock::now();
				auto duration = duration_cast<milliseconds>(end - timer_start);
				int ms_time = duration.count();
				std::cout << "Epoch " << i << ": " << ms_time << " ms/epoch - Loss: " 
					<< std::setprecision(4) << history(0, i - 1) << " - Accuracy: " 
					<< std::setprecision(2) << history(1, i - 1) << std::endl;

				char c[10];
				sprintf_s(c, "%d", i);
				string new_path = "C:/Users/Sep/OneDrive/MyCodes/Database/_Neural_Networks/Temp/Test-";
				new_path.append(c).append(".snn");
				this->save(new_path);
			}
		}
		return this->history;
	}

	template <typename _T>
	void NeuralNetwork<_T>::evaluate(Matrix<_T>& data, Matrix<_T>& label)
	{
		if (data.getCols() != input_shape ||
			label.getCols() != output_shape)
			throw std::invalid_argument("Data or Label shape mismatch!!!");

		// the error and accuracy are updated for each sample; in the end the correct value is calculated
		this->error = 0;
		this->accuracy = 0;
		for (int iteration = 0; iteration < data.getRows(); iteration++)
		{
			// [input] used in [feed_forward()]
			// [target] used in [set_error()] 
			// each row has one data stored; with [get_row()] one sample is received
			data.get_row(iteration, input);
			label.get_row(iteration, target);

			feed_forward();
			// store the error and accuracy
			set_error();
		}
		double samples = data.getRows();
		
		// error was calculated in [set_error()]
		if (optimizer == "rms")
			error = sqrt(error / (samples * output_shape));
		else if (optimizer == "mse")
			error = error / (2 * samples); // total mean square error
		accuracy = accuracy / samples;
		std::cout << "Loss :: " << error << std::endl;
		std::cout << "Accuracy :: " << accuracy << "\n" << std::endl;
	}

	template <typename _T>
	void NeuralNetwork<_T>::predict(Matrix<_T>& input, Matrix<_T>& output)
	{
		if (input.getCols() != input_shape)
			throw std::invalid_argument("Data shape mismatch!!!");

		if (output.getCols() != output_shape ||
			output.getRows() != input.getRows())
			output.Create(input.getRows(), output_shape, 0);
		for (int row = 0; row < input.getRows(); row++)
		{
			input.get_row(row, this->input);
			feed_forward();
			_T* outputData = output.getDataRow(row);
			_T* netOutputData = this->layers[output_layer_index].getNeurons()->getDataRow(0);
			for (int i = 0; i < output_shape; i++)
				outputData[i] = netOutputData[i];
		}
	}

	template <typename _T>
	int NeuralNetwork<_T>::predict_class(Matrix<_T>& input)
	{
		if (input.getCols() != input_shape)
			throw std::invalid_argument("Data shape mismatch!!!");

		this->input = input;
		feed_forward();
		_T* out = this->layers[output_layer_index].getNeurons()->getDataRow(0);
		int ans = 0;
		_T max = out[0];
		for (int i = 1; i < output_shape; i++)
			if (out[i] > max)
			{
				max = out[i];
				ans = i;
			}
		for (int i = 1; i < output_shape; i++)
			if (out[i] == max && i != ans)
				return -1;
		return ans;
	}


	// private functions

	template <typename _T>
	void NeuralNetwork<_T>::feed_forward()
	{
		for (int i = 0; i <= output_layer_index; i++)
		{
			Matrix<_T>* inputNeurons;
			Matrix<_T>* weight = this->layers[i].getWeights();
			Matrix<_T>* b_weight = this->layers[i].getBiasWeights();
			Matrix<_T>* outputNeurons = this->layers[i].getNeurons();
			// for the first layer we need samples; for other layers we need previous layer neurons
			if (i == 0)
				inputNeurons = &(this->input); // [input] is train data
			else
				inputNeurons = this->layers[i - 1].getNeurons();

			// normal matrix multiplication
			MatrixMultiplication(inputNeurons, weight, outputNeurons);

			// adding bias for each output neuron
			_T* biasWeightData = b_weight->getDataRow(0);
			_T* outputNeuronData = outputNeurons->getDataRow(0);
			for (int j = 0; j < outputNeurons->getCols(); j++)
				outputNeuronData[j] += biasWeightData[j];

			// [activate()] is an in-built Layer function that activates neuron values
			layers[i].activate();
			// [derived()] is and in-built Layer function that calculate the derivative of neuron's activated value
			// the derivative is saved in a matrix which can later be called by getDerivative()
			// storing the derivative may not be an efficient memory management method but overall it helps
			// the understanding of the code better and easier
			layers[i].derived();
		}
	}

	template <typename _T>
	void NeuralNetwork<_T>::set_error()
	{
		Matrix<_T>* output = this->layers[output_layer_index].getNeurons();
		_T* outputData = output->getDataRow(0);
		_T* targetData = target.getDataRow(0);
		_T* outputErrorData = output_error.getDataRow(0);

		for (int col = 0; col < output->getCols(); col++)
		{
			// [output_error] is the matrix of error for each output layer's neurons
			// it is later used in [back_propagation()]
			outputErrorData[col] = targetData[col] - outputData[col];

			// error is for network error
			this->error += pow(outputErrorData[col], 2);
		}
		// if the max_output_neuron_index is equal to label; the data is correctly predicted
		if (targetData[sep::argmax(*output)] == 1)
			accuracy++;
	}

	template <typename _T>
	void NeuralNetwork<_T>::back_propagation()
	{
		Matrix<_T>* delta;
		Matrix<_T>* output_der;
		Matrix<_T>* next_layer_weight;
		Matrix<_T>* next_layer_delta;
		Matrix<_T> propagated_error;

		// for output Layer
		// delta_last_hidden_to_output_weight(i) = output_derivative(i) * output_error(i)
		delta = this->layers[output_layer_index].getDelta();
		output_der = this->layers[output_layer_index].getDerivative();
		sep::dot(output_der, &output_error, delta);

		// for the hidden Layers
		for (int i = output_layer_index - 1; i >= 0; i--)
		{
			// each delta is a 1 by X matrix; X is the layer neuron's size
			// delta_for_hidden_layer(i) = hidden_output_derivative(i) * propagated_error(i)
			// propagated_error(i) = next_layer_weight(i, j) * next_layer_delta_transposed(j, 1); matrix multiplication
			// the sigma in documents can be interpreted as matrix multiplication
			delta = this->layers[i].getDelta();
			output_der = this->layers[i].getDerivative();
			next_layer_weight = this->layers[i + 1].getWeights();
			next_layer_delta = this->layers[i + 1].getDelta();

			// [next_layer_delta] is temporarily transposed; before 1 by X matrix; after X by 1 matrix
			next_layer_delta->transpose();
			propagated_error.Create(next_layer_weight->getRows(), next_layer_delta->getCols(), 0);
			MatrixMultiplication(next_layer_weight, next_layer_delta, &propagated_error);
			next_layer_delta->transpose();

			// [propagated_error] is X by 1 matrix; output_der is 1 by X
			propagated_error.transpose();
			sep::dot(output_der, &propagated_error, delta);
		}
	}

	template <typename _T>
	void NeuralNetwork<_T>::update_weights()
	{
		_T* inputNeuron;
		_T* delta;
		_T** wdif;
		_T** weight;

		for (int i = 0; i <= output_layer_index; i++)
		{
			// for the first layer we need samples; for other layers we need previous layer neurons
			if (i == 0)
				inputNeuron = (this->input).getDataRow(0);
			else
				inputNeuron = this->layers[i - 1].getNeurons()->getDataRow(0);
			delta = this->layers[i].getDelta()->getDataRow(0);
			
			weight = this->layers[i].getWeights()->getData();
			wdif = this->layers[i].getWeightDif()->getData();
			for (int row = 0; row < this->layers[i].getWeights()->getRows(); row++)
				for (int col = 0; col < this->layers[i].getWeights()->getCols(); col++)
				{
					wdif[row][col] = lr * delta[col] * inputNeuron[row] + alpha * wdif[row][col];
					weight[row][col] += wdif[row][col];
				}

			// weight update for bias
			weight = this->layers[i].getBiasWeights()->getData();
			wdif = this->layers[i].getBiasWeightDif()->getData();
			for (int col = 0; col < this->layers[i].getWeights()->getCols(); col++)
			{
				wdif[0][col] = lr * delta[col] + alpha * wdif[0][col];
				weight[0][col] += wdif[0][col];
			}
		}
	}

	template <typename _T>
	void NeuralNetwork<_T>::set_total_error()
	{
		double samples = train->getRows();
		Matrix<_T> result(2, 1, 0);

		// error was calculated in [set_error()]
		if (optimizer == "rms")
			result(0) = sqrt(error / (samples * output_shape));
		else if (optimizer == "mse")
			result(0) = error / (2 * samples); // total mean square error
		// calculate accuracy
		result(1) = accuracy / samples;

		this->history.push_back(result, sep::MATRIX_COL); // each data adds to col size
	}

	// Linkers
	template class NeuralNetwork<float>;
	template class NeuralNetwork<double>;
}
