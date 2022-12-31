#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <direct.h>
#include <vector>

using std::string;
using std::vector;
// make sure you are using C++ 17
namespace fs = std::filesystem;

int path_exists(string path);
string path_join(string path1, string path2);
int is_dir(string path);
int is_file(string path);
void list_dirs(vector<string>& list, string path);
void list_files(vector<string>& list, string path);
