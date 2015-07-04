#include "HuffmanTree.h"
#include <fstream>
#include <iostream>
#include <functional>
#include <vector>
#include <queue>

using namespace std;

HuffmanTree::HuffmanTree(void)
{
}

HuffmanTree::~HuffmanTree(void)
{
}

/*
struct HuffmanTreeNode {
  char c;
  bool isGroupNode;
  unsigned int freq;
  vector<bool> code;
  HuffmanTreeNode *left;
  HuffmanTreeNode *right;
};
*/

void HuffmanTree::add_to_encoding(char c, unsigned int freq) {
  HuffmanTreeNode *hfn = new HuffmanTreeNode;
  hfn->c = c;
  hfn->freq = freq;
  hfn->left = hfn->right = NULL;
  hfn->isGroupNode = false;
  hfHeap.push(hfn);
}

void HuffmanTree::print_freqs_in_encoding() {
  priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, CompareHuffmanNode> temp(hfHeap);
  while (!temp.empty()) {
    cout<<temp.top()->c<<":  "<<temp.top()->freq<<"\n";
    temp.pop();
  }
}

void HuffmanTree::encode() {
  while (hfHeap.size() > 1) {
    HuffmanTreeNode *r = hfHeap.top();
    hfHeap.pop();
    HuffmanTreeNode *l = hfHeap.top();
    hfHeap.pop();

    if (DEBUG) {
      cout<<"pop:  "<<l->freq<<" and "<<r->freq<<"\n";
    }
    HuffmanTreeNode *hfn = new HuffmanTreeNode;
    hfn->c = -1;
    hfn->freq = l->freq + r->freq;
    hfn->left = l;
    hfn->right = r;
    hfn->isGroupNode = true;

    hfHeap.push(hfn);
  }
}

void HuffmanTree::print_encoding() {

}

// hf[256];
//struct HuffmanTreeNode hf[256];

//void HuffmanTree::initialiseArray(HuffmanTreeNode* p, int i) {
//	//for  (int i=0; i<256; i++) {
//	if(i>=0) {
//		p->c = (char) i;
//		p->isGroupNode = false;
//	}
//	else
//		p->isGroupNode = true;
//	p->freq = freqArray[i];
//	p->left = p->right = NULL;
//	p->code = 0;
//	//}
//}
//
//void HuffmanTree::getFreqIntoArray(char file[])
//{
//	for (int i=0; i<256; i++)
//		freqArray[i] = 0;
//	char temp;
//	unsigned char c;
//	int count = 0;
//	FILE *f;
//	if ((f=fopen(file,"r"))==NULL) {
//		cout<<"ouch, text\n";
//		return;
//	}
//	while((temp=getc(f))!=EOF){
//		c = (unsigned char)temp;
//		freqArray[(int)c]++;
//	}
//	minHeapify();
//}
//
//void HuffmanTree::minHeapify() {
//	cout<<"\n";
//	for (int i=0; i<256; i++) {
//		if(freqArray[i]!=0) {
//			cout<<(unsigned char)i;
//			HuffmanTreeNode p;
//			initialiseArray(&p,i);
//			hfHeap.push_back(p);
//			int in = hfHeap.size()-1;
//
//			//actual heapification loop
//			while(in>0 && PARENT(in)>=0 && (hfHeap[PARENT(in)].freq > hfHeap[in].freq)) {
//				HuffmanTreeNode temp = hfHeap[PARENT(in)];
//				hfHeap[PARENT(in)] = hfHeap[in];
//				hfHeap[in] = temp;
//				in = PARENT(in);
//			}
//		}
//	}
//	cout<<"\n";
//	for(int j=0;j<hfHeap.size();j++)
//		cout<<hfHeap[j].freq<<" ";
//}
//
//void HuffmanTree::buildHuffmanTree() {
//	HuffmanTreeNode min = hfHeap[0];
//	hfHeap.pop_back();
//
//	int top = 0;
//	while(true)
//	if(hfHeap[LEFT(top)].freq > hfHeap[RIGHT(top)].freq);
//
//}


