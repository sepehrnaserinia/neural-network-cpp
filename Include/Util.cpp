#include "Util.h"

// Database Functions

template <typename T, typename T2 = int>
void save_database(Matrix<T>& data, Matrix<T2>& label, string filename)
{
	std::ofstream file(filename);
	data.print(file, "size", "Data");
	label.print(file, "size", "Label");
	file.close();
}
template <typename T, typename T2 = int>
void load_database(Matrix<T>& data, Matrix<T2>& label, string filename)
{
	std::ifstream file(filename);
	if (file.is_open())
	{
		int rows, cols;
		sep::read_matrix_size(file, rows, cols); // Data: (rows, cols)
		data.Create(rows, cols, 0);
		sep::read_matirx_data(file, &data);

		sep::read_matrix_size(file, rows, cols); // Label: (rows, cols)
		label.Create(rows, cols, 0);
		sep::read_matirx_data(file, &label);
	}
	else
		std::cout << "Error Opening File \"" << filename << "\"!!!" << std::endl;
	file.close();
}
bool load_datasets(vector<vector<string>>& dataset, string dataset_path)
{
	vector<string> className;
	list_dirs(className, dataset_path);
	for (int i = 0; i < className.size(); i++)
	{
		vector<string> dataPath;
		list_files(dataPath, path_join(dataset_path, className.at(i)));
		for (int index = 0; index < dataPath.size(); index++)
			dataPath.at(index) = path_join(path_join(dataset_path, className.at(i)), dataPath.at(index));
		dataset.push_back(dataPath);
	}
	if (dataset.at(0).size() != 0)
		return true;
	return false;
}
template <typename T, typename T2 = int>
void shuffle_in_unison(Matrix<T>& data, Matrix<T2>& label)
{
	if (data.getRows() != label.getRows())
		throw std::invalid_argument("Data and Label row size mismatch!!!");

	int row_size = data.getRows();
	for (int row = 0; row < row_size; row++)
	{
		int index = rand() % row_size;
		data.swap(row, index);
		label.swap(row, index);
	}
}
template <typename T>
void to_categorical(Matrix<T>& label)
{
	Matrix<T> tmp(label);
	T** preData = tmp.getData();

	int classNumbers = 0;
	for (int row = 0; row < label.getRows(); row++)
		if (preData[row][0] > classNumbers)
			classNumbers = preData[row][0];

	label.Create(label.getRows(), classNumbers + 1, 0);
	T** data = label.getData();
	for (int row = 0; row < label.getRows(); row++)
		data[row][(int)preData[row][0]] = 1;
}


// Plot Functions

template <typename T, typename T2 = int>
void plot_decisoin_boundary(NeuralNetwork<T>& net, Matrix<T>& data, Matrix<T2>& label)
{
	cv::Vec3b* colors = new cv::Vec3b[5];
	colors[0] = cv::Vec3b(255, 0, 0);
	colors[1] = cv::Vec3b(0, 255, 0);
	colors[2] = cv::Vec3b(0, 0, 255);
	colors[3] = cv::Vec3b(255, 0, 255);
	colors[4] = cv::Vec3b(255, 255, 0);

	Matrix<T> input_data(1, 2, 0); // two datas for x and y coordinates
	T* input = input_data.getDataRow(0); // get the pointer to datas
	Matrix<T> output_label(1, label.getCols(), 0);
	T* output = output_label.getDataRow(0);

	int size = 100;
	cv::Mat plt(size, size, CV_8UC3, cv::Scalar(255, 255, 255));
	for (T row = 0; row < size; row++)
		for (T col = 0; col < size; col++)
		{
			input[0] = (T) col / size; // x coordinate
			input[1] = (T) row / size; // y coordinate
			int n_class = net.predict_class(input_data);
			if (n_class == -1)
 				plt.at<cv::Vec3b>(row, col) =  cv::Vec3b(255, 255, 255); 
			else
				plt.at<cv::Vec3b>(row, col) = colors[n_class];
		}
	size *= 4;
	cv::resize(plt, plt, cv::Size(size, size), 0, 0, cv::INTER_CUBIC);
	
	for (int row = 0; row < data.getRows(); row++)
	{
		int n_class = 0;
		for (int col = 0; col < label.getCols(); col++)
			if (label(row, col) == 1)
				n_class = col;
		cv::Point center(data(row, 0) * size, data(row, 1) * size);
		cv::Scalar color(colors[n_class]);
		cv::circle(plt, center, 5, cv::Scalar(0, 0, 0), cv::FILLED);
		cv::circle(plt, center, 4, color, cv::FILLED);
	}
	cv::namedWindow("Plot");
	cv::imshow("Plot", plt);
	cv::waitKey(0);

	delete[] colors;
}


// Linkers
// Database Functions
template void save_database(Matrix<float>&, Matrix<int>&, string);
template void save_database(Matrix<float>&, Matrix<float>&, string);
template void save_database(Matrix<double>&, Matrix<int>&, string);
template void save_database(Matrix<double>&, Matrix<double>&, string);
template void load_database(Matrix<float>&, Matrix<int>&, string);
template void load_database(Matrix<float>&, Matrix<float>&, string);
template void load_database(Matrix<double>&, Matrix<int>&, string);
template void load_database(Matrix<double>&, Matrix<double>&, string);
template void shuffle_in_unison(Matrix<float>&, Matrix<int>&);
template void shuffle_in_unison(Matrix<float>&, Matrix<float>&);
template void shuffle_in_unison(Matrix<double>&, Matrix<int>&);
template void shuffle_in_unison(Matrix<double>&, Matrix<double>&);
template void to_categorical(Matrix<int>&);
template void to_categorical(Matrix<float>&);
template void to_categorical(Matrix<double>&);;
// Plot Functions
template void plot_decisoin_boundary(NeuralNetwork<float>&, Matrix<float>&, Matrix<float>&);
template void plot_decisoin_boundary(NeuralNetwork<double>&, Matrix<double>&, Matrix<float>&);
