// SepMath.cpp
#include "SepMath.h"

namespace sep
{
	// Matrix Class
	// constructers

	template <typename _Type>
	Matrix<_Type>::Matrix()
	{
		init();
	}
	
	template <typename _Type>
	Matrix<_Type>::Matrix(int row_size, int col_size)
	{
		Create(row_size, col_size);
	}

	template <typename _Type>
	Matrix<_Type>::Matrix(int row_size, int col_size, _Type value)
	{
		Create(row_size, col_size, value);
	}

	template <typename _Type>
	Matrix<_Type>::Matrix(Matrix<_Type>& input)
	{
		init().push_back(input, MATRIX_COL);
	}
	
	template <typename _Type>
	Matrix<_Type>::Matrix(_Type* input, int size, ADD_TYPE type)
	{
		init().push_back(input, size, type);
	}

	template <typename _Type>
	Matrix<_Type>::~Matrix()
	{
		delete_matrix();
		this->row_size = this->col_size = this->row_cap = this->col_cap = 0;
	}

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::Create(int row_size, int col_size)
	{
#ifdef THROW_ERROR
		if (row_size < 1 || col_size < 1)
			throw std::invalid_argument("Can't Create Matrix with size lower than 1!!!");
#endif
		delete_matrix();
		this->row_size = row_size;
		this->col_size = col_size;
		create_matrix(row_size, col_size);

		for (int i = 0; i < this->row_size; i++)
			for (int j = 0; j < this->col_size; j++)
				this->data[i][j] = (_Type)rand() / RAND_MAX - 0.5;

		return *this;
	}

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::Create(int row_size, int col_size, _Type value)
	{
#ifdef THROW_ERROR
		if (row_size < 1 || col_size < 1)
			throw std::invalid_argument("Can't Create Matrix with size lower than 1!!!");
#endif
		delete_matrix();
		this->row_size = row_size;
		this->col_size = col_size;
		create_matrix(row_size, col_size);

		for (int i = 0; i < this->row_size; i++)
			for (int j = 0; j < this->col_size; j++)
				this->data[i][j] = value;

		return *this;
	}


	// private functions

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::init()
	{
		this->row_size = 0;
		this->col_size = 0;
		create_matrix(1, 2);

		return *this;
	}

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::increase_matrix_size(int len, bool addto_col)
	{
		// create pre_data
		int pre_row_size = this->row_size;
		int pre_col_size = this->col_size;
		_Type** pre_data = new _Type * [pre_row_size];
		for (int i = 0; i < pre_row_size; i++)
			pre_data[i] = new _Type[pre_col_size];

		// pre_data = data
		for (int i = 0; i < this->row_size; i++)
			for (int j = 0; j < this->col_size; j++)
				pre_data[i][j] = this->data[i][j];

		// resize data; start
		delete_matrix();
		if (addto_col)
			while (this->col_cap < this->col_size + len)
				this->col_cap *= 2;
		else
			while (this->row_cap < this->row_size + len)
				this->row_cap *= 2;
		create_matrix(this->row_cap, this->col_cap);
		// resize data; end

		// data = pre_data
		for (int i = 0; i < this->row_size; i++)
			for (int j = 0; j < this->col_size; j++)
				this->data[i][j] = pre_data[i][j];

		// delete pre_data
		for (int i = 0; i < pre_row_size; i++)
			delete[] pre_data[i];
		delete[] pre_data; // possible error if data size are 0
		pre_data = nullptr;

		return *this;
	}

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::create_matrix(int row_cap, int col_cap)
	{
		this->row_cap = row_cap;
		this->col_cap = col_cap;
		data = new _Type * [this->row_cap];
		for (int i = 0; i < this->row_cap; i++)
			data[i] = new _Type[col_cap];

		return *this;
	}

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::delete_matrix()
	{
		if (data != nullptr)
		{
			for (int i = 0; i < this->row_cap; i++)
				delete[] data[i];
			delete[] data;
			data = nullptr;
		}

		return *this;
	}


	// public functions

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::push_back(Matrix<_Type>& input, ADD_TYPE type)
	{
		if (this->row_size == 0 && this->col_size == 0)
		{
			if (input.row_size > this->row_cap || input.col_size > this->col_cap)
				delete_matrix().create_matrix(input.row_size, input.col_size);
			for (int i = 0; i < input.row_size; i++)
				for (int j = 0; j < input.col_size; j++)
					this->data[i][j] = input.data[i][j];
			this->col_size = input.col_size;
			this->row_size = input.row_size;
		}
		else if (type == MATRIX_COL && input.row_size == this->row_size)
		{
			if (this->col_size + input.col_size > this->col_cap)
				increase_matrix_size(input.col_size, true);
			for (int j = 0; j < input.col_size; j++)
				for (int i = 0; i < this->row_size; i++)
					this->data[i][this->col_size + j] = input.data[i][j];
			this->col_size += input.col_size;
		}
		else if (type == MATRIX_ROW && input.col_size == this->col_size)
		{
			if (this->row_size + input.row_size > this->row_cap)
				increase_matrix_size(input.row_size, false);
			for (int i = 0; i < input.row_size; i++)
				for (int j = 0; j < this->col_size; j++)
					this->data[this->row_size + i][j] = input.data[i][j];
			this->row_size += input.row_size;
		}
		else
			throw std::invalid_argument("Can't Push Back Matrix to Matrix!!!");

		return *this;
	}

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::push_back(_Type* input, int size, ADD_TYPE type)
	{
		if (this->row_size == 0 && this->col_size == 0)
		{
			if (type == SCALAR_COL || type == MATRIX_ROW)
			{
				if (size > col_cap)
					delete_matrix().create_matrix(1, size);
				for (int i = 0; i < size; i++)
					this->data[0][i] = input[i];
				this->row_size = 1;
				this->col_size = size;
			}
			else if (type == SCALAR_ROW || type == MATRIX_COL)
			{
				if (size > row_cap)
					delete_matrix().create_matrix(size, 1);
				for (int i = 0; i < size; i++)
					this->data[i][0] = input[i];
				this->row_size = size;
				this->col_size = 1;
			}
		}
		else if (type == SCALAR_COL && this->row_size == 1)
		{
			if (this->col_size + size > this->col_cap)
				increase_matrix_size(size, true);
			for (int i = 0; i < size; i++)
				this->data[0][this->col_size + i] = input[i];
			this->col_size += size;
		}
		else if (type == SCALAR_ROW && this->col_size == 1)
		{
			if (this->row_size + size > this->row_cap)
				increase_matrix_size(size, false);
			for (int i = 0; i < size; i++)
				this->data[this->row_size + i][0] = input[i];
			this->row_size += size;
		}
		else if (type == MATRIX_COL && size == this->row_size)
		{
			if (this->col_size + 1 > this->col_cap)
				increase_matrix_size(1, true);
			for (int i = 0; i < size; i++)
				this->data[i][this->col_size] = input[i];
			this->col_size++;
		}
		else if (type == MATRIX_ROW && size == this->col_size)
		{
			if (this->row_size + 1 > this->row_cap)
				increase_matrix_size(1, false);
			for (int i = 0; i < size; i++)
				this->data[this->row_size][i] = input[i];
			this->row_size++;
		}
		else
			throw std::invalid_argument("Can't Push Back Dynamic Array to Matrix!!!");

		return *this;
	}

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::push_back(_Type input, ADD_TYPE type)
	{
		if (this->row_size == 0 && this->col_size == 0)
		{
			this->data[0][0] = input;
			this->row_size = 1;
			this->col_size = 1;
		}
		else if (type == SCALAR_COL && this->row_size == 1)
		{
			if (this->col_size + 1 > this->col_cap)
				increase_matrix_size(1, true);
			this->data[0][this->col_size] = input;
			this->col_size++;
		}
		else if (type == SCALAR_ROW && this->col_size == 1)
		{
			if (this->row_size + 1 > this->row_cap)
				increase_matrix_size(1, false);
			this->data[this->row_size][0] = input;
			this->row_size++;
		}
		else
			throw std::invalid_argument("Can't Push Back Variable to Matrix!!!");

		return *this;
	}

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::ravel(Matrix<_Type>& input)
	{
		input.Create(1, this->row_size * this->col_size, 0);
		for (int row = 0; row < this->row_size; row++)
			for (int col = 0; col < this->col_size; col++)
				input.data[0][(row * this->col_size) + col] = this->data[row][col];

		return *this;
	}

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::transpose()
	{
		// create pre_data
		int pre_row_size = this->row_size;
		int pre_col_size = this->col_size;
		_Type** pre_data = new _Type * [pre_row_size];
		for (int i = 0; i < pre_row_size; i++)
			pre_data[i] = new _Type[pre_col_size];

		// pre_data = data
		for (int i = 0; i < this->row_size; i++)
			for (int j = 0; j < this->col_size; j++)
				pre_data[i][j] = this->data[i][j];

		// resize data; start
		delete_matrix();
		this->row_size = pre_col_size;
		this->col_size = pre_row_size;
		this->row_cap = this->row_size;
		this->col_cap = this->col_size;
		create_matrix(this->row_cap, this->col_cap);
		// resize data; end

		// data = pre_data transposed
		for (int row = 0; row < this->row_size; row++)
			for (int col = 0; col < this->col_size; col++)
				this->data[row][col] = pre_data[col][row];

		// delete pre_data
		for (int i = 0; i < pre_row_size; i++)
			delete[] pre_data[i];
		delete[] pre_data; // possible error if data size are 0
		pre_data = nullptr;

		return *this;
	}

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::swap(int index_1, int index_2, SWAP_TYPE type)
	{
#ifdef THROW_ERROR
		if (index_1 < 0 || index_2 < 0)
			throw std::invalid_argument("Index lower than 0!!!");
		if (type == ROW && (index_1 >= this->row_size || index_2 >= this->row_size))
			throw std::invalid_argument("Index higher than row size!!!");
		if (type == COL && (index_1 >= this->col_size || index_2 >= this->col_size))
			throw std::invalid_argument("Index higher than col size!!!");
#endif

		if (type == ROW)
		{
			_Type temp;
			for (int col = 0; col < this->col_size; col++)
			{
				temp = data[index_1][col];
				data[index_1][col] = data[index_2][col];
				data[index_2][col] = temp;
			}
		}
		else if (type == COL)
		{
			_Type temp;
			for (int row = 0; row < this->row_size; row++)
			{
				temp = data[row][index_1];
				data[row][index_1] = data[row][index_2];
				data[row][index_2] = temp;
			}
		}

		return *this;
	}


	template <typename _Type>
	bool Matrix<_Type>::IsEmpty()
	{
		if (this->row_size == 0 && this->col_size == 0)
			return true;
		return false;
	}
	
	template <typename _Type>
	int Matrix<_Type>::getRows()
	{
		return this->row_size;
	}

	template <typename _Type>
	int Matrix<_Type>::getCols()
	{
		return this->col_size;
	}
	
	template <typename _Type>
	void Matrix<_Type>::get_row(int index, sep::Matrix<_Type>& input)
	{
#ifdef THROW_ERROR
		if (index < 0 || index >= this->row_size)
			throw std::invalid_argument("Index Row Out of Bounds!!!");
		if (input.row_size != 1 || input.col_size != this->col_size)
			throw std::invalid_argument("Input Data Size mismatch!!!");
#endif
		for (int col = 0; col < this->col_size; col++)
			input.data[0][col] = this->data[index][col];
	}

	template <typename _Type>
	void Matrix<_Type>::get_col(int index, sep::Matrix<_Type>& input)
	{
#ifdef THROW_ERROR
		if (index < 0 || index >= this->col_size)
			throw std::invalid_argument("Index Col Out of Bounds!!!");
		if (input.row_size != this->row_size || input.col_size != 1)
			throw std::invalid_argument("Input Data Size mismatch!!!");
#endif
		for (int row = 0; row < this->row_size; row++)
			input.data[row][0] = this->data[row][index];
	}

	template <typename _Type>
	void Matrix<_Type>::set_row(int index, sep::Matrix<_Type>& input)
	{
#ifdef THROW_ERROR
		if (index < 0 || index >= this->row_size)
			throw std::invalid_argument("Index Row Out of Bounds!!!");
		if (input.row_size != 1 || input.col_size != this->col_size)
			throw std::invalid_argument("Input Data Size mismatch!!!");
#endif
		for (int col = 0; col < this->col_size; col++)
			this->data[index][col] = input.data[0][col];
	}

	template <typename _Type>
	void Matrix<_Type>::set_col(int index, sep::Matrix<_Type>& input)
	{
#ifdef THROW_ERROR
		if (index < 0 || index >= this->col_size)
			throw std::invalid_argument("Index Col Out of Bounds!!!");
		if (input.row_size != this->row_size || input.col_size != 1)
			throw std::invalid_argument("Input Data Size mismatch!!!");
#endif
		for (int row = 0; row < this->row_size; row++)
			this->data[row][index] = input.data[row][0];
	}

	template <typename _Type>
	_Type** Matrix<_Type>::getData()
	{
		return this->data;
	}

	template <typename _Type>
	_Type* Matrix<_Type>::getDataRow(int index)
	{
#ifdef THROW_ERROR
		if (index < 0 || index >= this->row_size)
			throw std::invalid_argument("Index Row Out of Bounds!!!");
#endif
		return this->data[index];
	}


	template <typename _Type>
	_Type Matrix<_Type>::get(int row, int col)
	{
#ifdef THROW_ERROR
		if (row < 0 || row >= row_size || col < 0 || col >= col_size)
			throw std::invalid_argument("Index Data Out of Bounds!!!");
#endif
		return this->data[row][col];
	}

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::set(int row, int col, _Type input)
	{
#ifdef THROW_ERROR
		if (row < 0 || row >= row_size || col < 0 || col >= col_size)
			throw std::invalid_argument("Index Data Out of Bounds!!!");
#endif
		data[row][col] = input;
		return *this;
	}

	template <typename _Type>
	_Type Matrix<_Type>::operator()(int row, int col) const
	{
#ifdef THROW_ERROR
		if (row < 0 || row >= row_size || col < 0 || col >= col_size)
			throw std::invalid_argument("Index Data Out of Bounds!!!");
#endif
		return this->data[row][col];
	}
	
	template <typename _Type>
	_Type& Matrix<_Type>::operator()(int row, int col)
	{
#ifdef THROW_ERROR
		if (row < 0 || row >= row_size || col < 0 || col >= col_size)
			throw std::invalid_argument("Index Data Out of Bounds!!!");
#endif
		return this->data[row][col];
	}

	template <typename _Type>
	_Type Matrix<_Type>::operator()(int index) const
	{
#ifdef THROW_ERROR
		if ((this->row_size == 1 && index >= this->col_size) || 
			(this->col_size == 1 && index >= this->row_size) ||
			index < 0)
			throw std::invalid_argument("Index Data Out of Bounds!!!");
#endif
		if (this->row_size == 1)
			return this->data[0][index];
		else if (this->col_size == 1)
			return this->data[index][0];
	}

	template <typename _Type>
	_Type& Matrix<_Type>::operator()(int index)
	{
#ifdef THROW_ERROR
		if ((this->row_size == 1 && index >= this->col_size) ||
			(this->col_size == 1 && index >= this->row_size) ||
			index < 0)
			throw std::invalid_argument("Index Data Out of Bounds!!!");
#endif
		if (this->row_size == 1)
			return this->data[0][index];
		else if (this->col_size == 1)
			return this->data[index][0];
	}

	template <typename _Type>
	Matrix<_Type>& Matrix<_Type>::operator=(Matrix& input)
	{
		if (&input == this)
			return *this;
		this->delete_matrix().init().push_back(input);
		return *this;
	}


	template <typename _Type>
	void Matrix<_Type>::clear()
	{
		delete_matrix().init();
	}

	template <typename _Type>
	void Matrix<_Type>::print(string out_type, string msg)
	{
		string str;
		this->print(str, out_type, msg);
		std::cout << str;
	}

	template <typename _Type>
	void Matrix<_Type>::print(ofstream& file, string out_type, string msg)
	{
		string str;
		this->print(str, out_type, msg);
		file << str;
	}

	template <typename _Type>
	void Matrix<_Type>::print(string& str, string out_type, string msg)
	{
		str = "";
		char char_str[200];

		// print heading
		if (out_type == "normal")
		{
			sprintf_s(char_str, "Matrix Size: (%d, %d) - Capacity: (%d, %d)\n", this->row_size, this->col_size, this->row_cap, this->col_cap);
			str.append(char_str);
			out_type = "matrix";
		}
		else if (out_type == "size")
		{
			str.append(msg);
			sprintf_s(char_str, ": (%d, %d)\n", this->row_size, this->col_size);
			str.append(char_str);
			out_type = "matrix";
		}
		else if (msg != "")
			str.append(msg);

		// print matrix
		if (out_type == "matrix")
		{
			for (int i = 0; i < this->row_size; i++)
			{
				for (int j = 0; j < this->col_size; j++)
				{
					if (std::is_same<_Type, int>::value)
						sprintf_s(char_str, "%d, ", data[i][j]);
					else
						sprintf_s(char_str, "%f, ", data[i][j]);
					str.append(char_str);
				}
				str.append("\n");
			}
		}
		else if (out_type == "python")
		{
			str.append("[");
			for (int i = 0; i < this->row_size; i++)
			{
				str.append("[");
				for (int j = 0; j < this->col_size; j++)
				{
					if (std::is_same<_Type, int>::value)
						sprintf_s(char_str, "%d, ", data[i][j]);
					else
						sprintf_s(char_str, "%f, ", data[i][j]);
					str.append(char_str);
				}
				str = str.substr(0, str.length() - 2); // to delete the last two characters ", "
				str.append("], "); 
			}
			str = str.substr(0, str.length() - 2); // to delete the last two characters ", "
			str.append("]\n"); 
		}
	}
	// Matrix Class; end

	// Matrix Accessories
	// File Mangament

	void read_matrix_size(ifstream& file, int& rows, int& cols)
	{
		string str;
		std::getline(file, str); // X: (row, col)
		
		int index1 = str.find("(");
		int index2 = str.find(", ");
		rows = atoi(str.substr(index1 + 1, index2 - index1 - 1).c_str());
		
		index1 = index2;
		index2 = str.find(")");
		cols = atoi(str.substr(index1 + 2, index2 - index1 - 2).c_str());
	}
	template <typename _T>
	void read_matirx_data(ifstream& file, Matrix<_T>* mat)
	{
		_T** data = mat->getData();

		string str;
		for (int row = 0; row < mat->getRows(); row++)
		{
			std::getline(file, str);
			for (int col = 0; col < mat->getCols(); col++)
			{
				int index = str.find(", ");
				data[row][col] = atof(str.substr(0, index).c_str());
				str = str.substr(index + 2);
			}
		}
	}

	// Utils

	template <typename _T>
	void dot(Matrix<_T>* input_a, Matrix<_T>* input_b, Matrix<_T>* output)
	{
#ifdef THROW_ERROR
		if (input_a->getCols() != output->getCols() ||
			input_b->getCols() != output->getCols() ||
			input_a->getRows() != output->getRows() ||
			input_b->getRows() != output->getRows())
			throw std::invalid_argument("Error at dot!");
#endif
		_T** a = input_a->getData();
		_T** b = input_b->getData();
		_T** o = output->getData();
		for (int row = 0; row < output->getRows(); row++)
			for (int col = 0; col < output->getCols(); col++)
				o[row][col] = a[row][col] * b[row][col];
	}
	template <typename _T>
	void MatrixMultiplication(Matrix<_T>* input_a, Matrix<_T>* input_b, Matrix<_T>* output)
	{
#ifdef THROW_ERROR
		if (input_a->getCols() != input_b->getRows())
			throw std::invalid_argument("Error at MatrixMultiplication!");
#endif
		_T** a = input_a->getData();
		_T** b = input_b->getData();
		_T** o = output->getData();
		for (int row = 0; row < input_a->getRows(); row++)
			for (int col = 0; col < input_b->getCols(); col++)
			{
				o[row][col] = 0;
				for (int index = 0; index < input_a->getCols(); index++)
					o[row][col] += a[row][index] * b[index][col];
			}
	}
	template <typename _T>
	int argmax(Matrix<_T>& input)
	{
#ifdef THROW_ERROR
		if (input.getRows() != 1)
			throw std::invalid_argument("Error at argmax!");
#endif 
		int index = 0;
		_T max = input(0, 0);
		for (int col = 0; col < input.getCols(); col++)
			if (input(0, col) > max)
			{
				index = col;
				max = input(0, col);
			}
		return index;
	}

	// Math Functions

	template <typename _T>
	_T max(_T x, _T y)
	{
		return (x > y ? x : y);
	}
	template <typename _T>
	_T min(_T x, _T y)
	{
		return (x < y ? x : y);
	}
	

	// linkers
	template class Matrix<int>;
	template class Matrix<float>;
	template class Matrix<double>;
	// File Mangament
	template void read_matirx_data(ifstream&, Matrix<int>*);
	template void read_matirx_data(ifstream&, Matrix<float>*);
	template void read_matirx_data(ifstream&, Matrix<double>*);
	// Utils
	template void dot(Matrix<int>*, Matrix<int>*, Matrix<int>*);
	template void dot(Matrix<float>*, Matrix<float>*, Matrix<float>*);
	template void dot(Matrix<double>*, Matrix<double>*, Matrix<double>*);
	template void MatrixMultiplication(Matrix<int>*, Matrix<int>*, Matrix<int>*);
	template void MatrixMultiplication(Matrix<float>*, Matrix<float>*, Matrix<float>*);
	template void MatrixMultiplication(Matrix<double>*, Matrix<double>*, Matrix<double>*);
	template int argmax(Matrix<int>&);
	template int argmax(Matrix<float>&);
	template int argmax(Matrix<double>&);
	template int max(int, int);
	template float max(float, float);
	template double max(double, double);
	template int min(int, int);
	template float min(float, float);
	template double min(double, double);
}
