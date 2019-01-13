#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
struct Settings {
	static const unsigned int buffer_size = 10;
	static const unsigned int min_repetition = 3;
	static const char unknown_char = 73;
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

	void write(vector<char> data) {
		// Append bianry data to the end of the file
		file = std::fstream(path, ios::app | ios::binary);
		string content_str(data.begin(), data.end());
		file << content_str;
		file.close();
	}
};

	
class Foo {
	std::vector<char> buffer;
	std::vector<char> memory;
	File infile;
	File outfile;
	char mapping[73] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
						  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
						  '0','1','2','3','4','5','6','7','8','9',' ','\n',',','.','+','-','*',':','=','(',')' };
public:
	Foo(string infile_path, string outfile_path) :
		buffer(), 
		infile(infile_path), 
		outfile(outfile_path) {
		
	}

	char encoded_letter(char letter) {
		for (char letter_from_map : mapping) {
			if (letter_from_map == letter) {
				return letter;
			}
		}
		return Settings::unknown_char;		
	}

	int find_match(int initial_pos) {
		char aaaaaaaaaaaa = buffer[initial_pos];
		int max_match_length = 0;

		string memory_str(memory.begin(), memory.end());
		for (int end_idx = 1; end_idx <= memory.size(); end_idx++) {
			// We must not exceed buffer boundary when extending lookup_fragment size
			if (initial_pos + end_idx > buffer.size())
				return max_match_length;

			// Extend lookup_fragment each time we found smaller match (longest common subsequence)
			string lookup_fragment(buffer.begin() + initial_pos, buffer.begin() + initial_pos + end_idx);
			int found = memory_str.find(lookup_fragment);
			if (found != std::string::npos) {
				max_match_length = lookup_fragment.size();
				//cout << "Found match at: " << found << " match: " << lookup_fragment << " length: " << max_match_length << endl;
			}
			else {
				return max_match_length;
			}
		}
		return max_match_length;
	}

	void compress() {
		while (true) {
			buffer = infile.read_next_buffer();
			if (buffer.empty())
				break;

			for (int letter_pos = 0; letter_pos < buffer.size(); letter_pos++) {
				int match_length = find_match(letter_pos);
				if (match_length < Settings::min_repetition) {
					char letter = buffer[letter_pos];
					memory.push_back(encoded_letter(letter));
				}
				else {
					char go_back_by= letter_pos - 1; // 0 - go back by 1 letters, 1 - go back by 2 letters, etc.
					char encoded_length = 71 + match_length; // 3-letter match = 74
					memory.push_back(encoded_length);
					memory.push_back(go_back_by);
					letter_pos += match_length - 1; // for loop will increase by additional 1
				}
			}
			outfile.write(memory);
		}
	}
	void decompress() {

	}
};

struct Menu {
	enum Action
	{
		compress,
		decompress
	};
	string prompt_for_input() {
		string inputPath;
		cout << "Enter input file path: ";
		cin >> inputPath;
		return inputPath;
	}

	string prompt_for_output() {
		string outputPath;
		cout << "Enter output file path: ";
		cin >> outputPath;
		return outputPath;
	}
	Action select_action() {
		int action;
		cout << "1) Compress" << endl;
		cout << "2) Decompress" << endl;
		cout << "> ";
		cin >> action;

		while (action < 1 && 2 < action) {
			cout << endl << "Invalid choice. Try again..." << endl;
			cout << "> ";
			cin >> action;
		}
		return static_cast<Action>(action);
	}
};

int main()
{	
	Menu menu;
	Menu::Action action = menu.select_action();
	//string inpath = menu.prompt_for_input();
	//string outpath = menu.prompt_for_output();

	string inpath = "D:\\siema.txt";
	string outpath = "D:\\beka.txt";

	if(action == Menu::Action::compress){
		Foo(inpath, outpath).compress();
	}
	else if (action == Menu::Action::decompress) {
		Foo(inpath, outpath).compress();
	}
	cout << "Done.";
	return 0;
}