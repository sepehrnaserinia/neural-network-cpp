#pragma once
#include <iostream>
#include <vector>
#include "Dir.h"
#include "SepMath.h"
#include "NeuralNetwork.h"
#include "opencv.hpp"

using std::string;
using std::vector;
using sep::Matrix;
using nn::NeuralNetwork;

// Database Functions

template <typename T, typename T2 = int>
void save_database(Matrix<T>& data, Matrix<T2>& label, string filename);
template <typename T, typename T2 = int>
void load_database(Matrix<T>& data, Matrix<T2>& label, string filename);
bool load_datasets(vector<vector<string>>& dataset, string dataset_path);
template <typename T, typename T2 = int>
void shuffle_in_unison(Matrix<T>& data, Matrix<T2>& label);
template <typename T>
void to_categorical(Matrix<T>& label);


// Plot Function

template <typename T, typename T2 = int>
void plot_decisoin_boundary(NeuralNetwork<T>& net, Matrix<T>& data, Matrix<T2>& label);
