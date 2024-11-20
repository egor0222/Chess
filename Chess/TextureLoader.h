#pragma once
#include "global.h"
#include <filesystem>

struct TextureContainerBucket{
	std::string name;
	Texture* texture;
};

using Tcb = TextureContainerBucket;
using namespace std::filesystem;

//std::string strCopy(std::string str, int begin, int end) {
//	std::string res;
//	for (int i = begin; i != end; i++) {
//		res += str[i];
//	}
//	return res;
//}
//
//std::string PathToName(path p) {
//	std::string str = p.generic_string();
//	int end = str.length() - 1;
//	while (str[end] != '/' || end != 0) {
//		end--;
//	}
//	std::string res = strCopy(str, end + 1, str.length() - 1);
//	end = res.length() - 1;
//	while (res[end] != '.' || end != 0) {
//		end--;
//	}
//	res.erase(end, res.length());
//	return res;
//}

class TextureLoader {
public:
	void init() {
		inited = true;
		this->getData();
	}

	~TextureLoader() {
		for (int i = 0; i < this->size; i++) {
			delete data[i].texture;
		}
		delete[] this->data;
	}

	Texture* operator()(std::string name) {
		if (!inited) return nullptr;
		name.append(".png");
		for (int i = 0; i < this->size; i++) {
			if (data[i].name == name) {
				return data[i].texture;
			}
		}
		std::cout << "File not found: " << name << endl;
	}

private:
	Tcb* data;
	size_t size = 0;
	bool inited = false;

	void getData() {
		path cpath = current_path();
		cpath.append("images");
		if (!exists(cpath) || !is_directory(cpath)) {
			std::cout << "Not derictory" << std::endl;
			std::abort(); // vary important
		}
		for (const auto& entry : directory_iterator(cpath)) {
			this->size++;
		}

		this->data = new Tcb[this->size];
		int iter = 0;

		for (const auto& entry : directory_iterator(cpath)) {
			if (entry.is_regular_file()) {
				//std::string name = PathToName(entry.path());
				//this->data[iter].name = PathToName(entry.path());
				this->data[iter].name = entry.path().filename().generic_string();
				this->data[iter].texture = new Texture();
				this->data[iter].texture->loadFromFile(entry.path().generic_string());
				iter++;
			}
		}
	}
};