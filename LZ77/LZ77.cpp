#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Settings {
	static const unsigned int buffer_size = 4086;
};

class File {
	std::string path;
	std::fstream file;

public:
	File(std::string path) {
		this->path = path;
		this->file = std::fstream(path);
	}

	std::vector<char> read_next_buffer(){
		std::vector<char> buffer(Settings::buffer_size, 0);
		if(file.read(buffer.data(), buffer.size())){
			return buffer;
		}
		else {
			return std::vector<char>();
		}
	}

	// TODO Take input data
	void write() {

	}
};

struct Compressor {
	static void execute() {
		File infile = File("D:\\siema.txt");

		while(true) {
			std::vector<char> buffer = infile.read_next_buffer();
			if (buffer.empty())
				break;
			std::cout << std::endl << "Buffer -------------------" << std::endl;
			std::cout << buffer.data() << std::endl;
		}
	}
};

struct Decompressor {

};

int main()
{
	Compressor::execute();
	return 0;
}