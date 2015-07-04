#include <vector>
#include <functional>
#include <queue>

#pragma once

using namespace std;

const bool DEBUG = true;

struct HuffmanTreeNode {
	char c;
	bool isGroupNode;
	unsigned int freq;
	vector<bool> code;
	HuffmanTreeNode *left;
	HuffmanTreeNode *right;
};

struct CompareHuffmanNode : public binary_function<HuffmanTreeNode*, HuffmanTreeNode*, bool>
{
    bool operator()(const HuffmanTreeNode* f, const HuffmanTreeNode* s) const
    {
        return f->freq > s->freq;
    }
};

class HuffmanTree
{
private:
	priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, CompareHuffmanNode> hfHeap;
public:


	HuffmanTree(void);

	~HuffmanTree(void);

	void add_to_encoding(char c, unsigned int freq);

	void print_freqs_in_encoding();

	void encode();

	void print_encoding();

//	void HuffmanTree::initialiseArray(HuffmanTreeNode*, int ) ;
//
//	void HuffmanTree::getFreqIntoArray(char[]);
//
//	void HuffmanTree::minHeapify();
//
//	void HuffmanTree::buildHuffmanTree();

};
