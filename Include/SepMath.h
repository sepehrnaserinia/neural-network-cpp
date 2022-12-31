// SepMath.h
#pragma once
#include <iostream>
#include <fstream>
#include <string>

using std::string;
using std::ofstream;
using std::ifstream;

namespace sep
{
	enum ADD_TYPE { SCALAR_COL, SCALAR_ROW, MATRIX_COL, MATRIX_ROW };
	enum SWAP_TYPE { ROW, COL };

	// Matrix Class
	template <typename _Type>
	class Matrix
	{
	public:
		Matrix();
		// create matrix with size (row, col) and random initialization
		Matrix(int row_size, int col_size);
		// create matrix with size (row, col) and with value initialization
		Matrix(int row_size, int col_size, _Type value);
		Matrix(Matrix<_Type>& input);
		Matrix(_Type* input, int size, ADD_TYPE type = SCALAR_COL);
		~Matrix();
		Matrix& Create(int row_size, int col_size);
		Matrix& Create(int row_size, int col_size, _Type value);

		Matrix& push_back(Matrix<_Type>& input, ADD_TYPE type = MATRIX_COL);
		Matrix& push_back(_Type* input, int size, ADD_TYPE type = SCALAR_COL);
		Matrix& push_back(_Type input, ADD_TYPE type = SCALAR_COL);
		Matrix& ravel(Matrix<_Type>& input);
		Matrix& transpose();
		Matrix& swap(int index_1, int index_2, SWAP_TYPE type = ROW);

		bool IsEmpty();
		int getRows();
		int getCols();
		void get_row(int index, sep::Matrix<_Type>& input);
		void get_col(int index, sep::Matrix<_Type>& input);
		void set_row(int index, sep::Matrix<_Type>& input);
		void set_col(int index, sep::Matrix<_Type>& input);
		_Type** getData();
		_Type* getDataRow(int index);

		_Type get(int row, int col);
		Matrix& set(int row, int col, _Type input);
		_Type operator()(int row, int col) const;
		_Type& operator()(int row, int col);
		_Type operator()(int index) const;
		_Type& operator()(int index);
		Matrix& operator=(Matrix& input);

		void clear();
		void print(string out_type = "normal", string msg = "");
		void print(ofstream& file, string out_type = "normal", string msg = "");
		void print(string& str, string out_type = "normal", string msg = "");
	private:
		Matrix& init();
		Matrix& increase_matrix_size(int len, bool addto_col);
		Matrix& create_matrix(int row_cap, int col_cap);
		Matrix& delete_matrix();
	private:
		int row_size, col_size;
		int row_cap, col_cap;
		_Type** data = nullptr;
		_Type fake_data = 0;
	};

	// Matrix Accessories
	// File Mangament

	void read_matrix_size(ifstream& file, int& rows, int& cols);
	template <typename _T>
	void read_matirx_data(ifstream& file, Matrix<_T>* mat);
	
	// Utils

	template <typename _T>
	void dot(Matrix<_T>* input_a, Matrix<_T>* input_b, Matrix<_T>* output);
	template <typename _T>
	void MatrixMultiplication(Matrix<_T>* input_a, Matrix<_T>* input_b, Matrix<_T>* output);
	template <typename _T>
	int argmax(Matrix<_T>& input);

	// Math Functions

	template <typename _T>
	_T max(_T x, _T y);
	template <typename _T>
	_T min(_T x, _T y);
}
