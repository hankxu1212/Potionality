#pragma once

#include <filesystem>
#include <vector>

class Files
{
public:
	// read relative path from executable from a file, store as vector of bytes
	static std::vector<std::byte> Read(const std::string& pathFromExecutable);

	// read absolute path
	static std::vector<std::byte> ReadAbsolute(const std::string& absolutePath);

	// returns the absolute path of a given relative path. If assert=true, will check if the path exists
	// Probably only use assert=false when writing to a certain file
	static std::string Path(const std::string& suffix, bool assert=true);

	// const char* version of Path
	static std::string Path(const char* suffix, bool assert=true);

	// check if a given path exists
	static bool Exists(const std::string& path);
};

