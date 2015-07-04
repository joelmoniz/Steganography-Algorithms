#include "HuffmanTree.h"
#include <map>
#include <string>
#include <functional>
#pragma once

using namespace std;

class HuffmanEncoding
{

private:

  map<char, unsigned int, greater<int> > char_freq;

public:
  void get_freq_count(string file);
  void generate_huffman_encoding(HuffmanTree *tr);
  void print_freq_count();

	HuffmanEncoding(void);

	~HuffmanEncoding(void);

  struct	HuffmanTreeNode* encode(char file[]);

};