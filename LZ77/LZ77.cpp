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

	std::vector<char> read_next_buffer() {
		std::vector<char> buffer(Settings::buffer_size, 0);
		if (file.read(buffer.data(), buffer.size())) {
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

class Compressor {
	char mapping[73] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
						  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
						  '0','1','2','3','4','5','6','7','8','9',' ','\n',',','.','+','-','*',':','=','(',')' };

public:
	static void execute() {
		File infile = File("D:\\siema.txt");

		while (true) {
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

struct Menu {

	std::string promptForInputPath() {

		std::string inputPath;
		std::cout << "Enter input file path: ";
		std::cin >> inputPath;
		return inputPath;
	}

	std::string promptForOutputPath() {

		std::string outputPath;
		std::cout << "Enter input file path: ";
		std::cin >> outputPath;
		return outputPath;
	}
	int selectAction() {

		int chosenAction;
		std::cout << "Enter 1 to compress, or 2 to quit: ";
		std::cin >> chosenAction;

		// Decompression is not implemented yet
		while (chosenAction < 1 && 2 < chosenAction) {

			std::cout << "Action not supported. Try again... : ";
			std::cin >> chosenAction;
		}
		return chosenAction;
	}
};




int main()
{

	Compressor::execute();
	return 0;
}