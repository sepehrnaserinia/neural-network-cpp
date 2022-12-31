#include "Dir.h"

int path_exists(string path)
{
	return fs::exists(path);
}
string path_join(string path1, string path2)
{
	if (path_exists(path1))
	{
		if (path1.at(path1.length() - 1) != '/')
			path1 = path1 + "/";
		if (path2.at(0) == '/')
			path2 = path2.substr(1);
		return path1 + path2;
	}
}
int is_dir(string path)
{
	return fs::is_directory(fs::path(path));
}
int is_file(string path)
{
	if (path_exists(path))
		return !is_dir(path);
	return 0;
}
void list_dirs(vector<string>& list, string path)
{
	if (!path_exists(path))
		return;
	string filename;
	for (auto& p : fs::directory_iterator(path))
	{
		filename = p.path().filename().string();
		if (is_dir(path_join(path, filename)))
			list.push_back(filename);
	}
}
void list_files(vector<string>& list, string path)
{
	if (!path_exists(path))
		return;
	string filename;
	for (auto& p : fs::directory_iterator(path))
	{
		filename = p.path().filename().string();
		if (!is_dir(path_join(path, filename)))
			list.push_back(filename);
	}
}
