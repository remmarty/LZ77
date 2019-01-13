#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
struct Settings {
	static const unsigned int buffer_size = 15;
	static const unsigned int search_buffer_size = 8;
	static const unsigned int lookahead_buffer_size = 6;
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

	void write(pair<pair<int, int>, char> data) {
		file = std::fstream(path, ios::app);
		file << data.first.first << data.first.second << data.second << ',';
	}
};

	/*char mapping[73] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
						  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
						  '0','1','2','3','4','5','6','7','8','9',' ','\n',',','.','+','-','*',':','=','(',')' };*/
class LZ77 {
	std::vector<char> search_buf;
	std::vector<char> lookahead_buf;
	std::vector<char> output_buf;
	std::vector<char> buffer;
public:
	LZ77() :
		search_buf(Settings::search_buffer_size), 
		lookahead_buf(Settings::lookahead_buffer_size),
		output_buf(),
		buffer() {
	}
	std::pair<std::pair<int, int>, char> PCS() {
		int last_found = 0;
		int max_length = 0;
		int l_end_idx = 1;
		while (true) {
			// We can match max <abb>c|<abb>c (c is not allowed)
			if (l_end_idx >= Settings::lookahead_buffer_size) {
				break;
			}
			if (l_end_idx >= lookahead_buf.size()) {
				return std::make_pair(std::make_pair(last_found, max_length), -1);
			}
			std::string rightside(lookahead_buf.begin(), lookahead_buf.begin() + l_end_idx);
			std::string leftside(search_buf.begin(), search_buf.end());

			int found = leftside.find(rightside);
			if (found != std::string::npos) {
				//std::cout << "found match at: " << found << " match: " << rightside << " length: " << rightside.size() << '\n';
				l_end_idx++;
				last_found = found;
				max_length = rightside.size();
			}
			else {
				//std::cout << "Failure, but last found was: " << last_found << "length: " << l_end_idx - 1 << '\n';
				break;
			}
		}
		return std::make_pair(std::make_pair(last_found, max_length), lookahead_buf[max_length]);
	}
	
	void shift_all_buffers(int offset) {
		search_buf.erase(search_buf.begin(), search_buf.begin() + offset);
		for (int i = 0; i < offset; i++)
			search_buf.push_back(lookahead_buf[i]);

		lookahead_buf.erase(lookahead_buf.begin(), lookahead_buf.begin() + offset);
		for (int i = 0; i < offset; i++) {
			if (i < buffer.size())
				lookahead_buf.push_back(buffer[i]);
		}

		if (buffer.size() >= offset)
			buffer.erase(buffer.begin(), buffer.begin() + offset);
	}

	void compress() {
		File infile = File("D:\\siema.txt");

		while (true) {
			buffer = infile.read_next_buffer();
			if (buffer.empty())
				break;

			// Initialize serach buffer by filling it with first letter from buffer
			// Bufor słownikowy jest wypełniany pierwszym symbolem wejściowym
			for (int i = 0; i < search_buf.size(); i++) {
				search_buf[i] = buffer[0];
			}
			// Initialize lookahead buffer by rewriting first n letters from buffer
			for (int i = 0; i < lookahead_buf.size(); i++) {
				lookahead_buf[i] = buffer[i];
			}
			// usunmy skopiowane rzecyz z buffera
			buffer.erase(buffer.begin(), buffer.begin() + lookahead_buf.size());

			// i ten symbol jest zapisywany na wyjście
			output_buf.push_back(buffer[0]);


			while (true) {
				pair<pair<int, int>, char> pcs = PCS();
				int P = pcs.first.first;
				int C = pcs.first.second;
				char S = pcs.second;
				int shift_offset = C + 1;

				shift_all_buffers(shift_offset);

				cout << "(" << P << ", " << C << ", " << S << ")" << endl;
				File("D:\\elo.txt").write(pcs);
				if (S == -1) {
					break;
				}
			}

			std::cout << std::endl << "Buffer -------------------" << std::endl;
			std::cout << buffer.data() << std::endl;
		}
	}

	void decompress() {

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
	//std::vector<char> lookahead_buf = { 'a', 'a', 'b', 'b' };
	//std::vector<char> search_buf = { 'a', 'a', 'a', 'a', 'a', 'b', 'b'};

	/*
	File f = File("D:\\elo.txt");
	f.write(make_pair(make_pair(1,2), 'C'));
	f.write(make_pair(make_pair(1, 2), 'D'));
	f.write(make_pair(make_pair(1, 2), 'E'));*/
	LZ77().compress();
	return 0;
}