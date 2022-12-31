#include <iostream>
#include "NeuralNetwork.h"
#include "SepMath.h"
#include "Util.h"

#pragma comment(lib, "opencv_world452d.lib")

using sep::Matrix;
using nn::NeuralNetwork;

int main()
{
	srand(time(NULL));

	try
	{
		Matrix<float> train;
		Matrix<float> train_label;
		load_database(train, train_label, "./Data/Points-35.dbs");
		to_categorical(train_label);

		NeuralNetwork<float> net;
		net.addLayer(5, "sigmoid", 2);
		net.compile(1, 0.01, 0.5, "rms");

		Matrix<float> history;
		history = net.fit(&train, &train_label, 10);

		std::cout << ">>> Train Sample Evaluation: " << std::endl;
		net.evaluate(train, train_label);
		plot_decisoin_boundary(net, train, train_label);
		net.save("./Data/NN-Model-Points-New.snn");
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "********************" << std::endl;
		std::cout << e.what();
		std::cout << "\n********************" << std::endl;
	}
}
