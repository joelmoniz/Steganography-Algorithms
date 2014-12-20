#include "Steganography.h"
#include "HuffmanTree.h"
#include "LSBSteganography.h"
#include <iostream>

using namespace std;

Steganography::Steganography(void)
{
}

Steganography::~Steganography(void)
{
}

int main() {
LSBSteganography *lsbSteg = new LSBSteganography;
//lsbSteg->stegify("Koala.bmp","Koala5.bmp","stmv.txt",5);
//cout<<"\n";
cout<<"DeStegifying...\n";
lsbSteg->destegify("Koala1.bmp",1);

//	HuffmanTree *ht = new HuffmanTree;
	//ht->minHeapify();
//	ht->getFreqIntoArray("test.txt");
cout<<"\n";
return 0;
}