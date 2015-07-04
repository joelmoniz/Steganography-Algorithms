#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include "HuffmanTree.h"
#include "HuffmanEncoding.h"

using namespace std;

HuffmanEncoding::HuffmanEncoding(void)
{
}

HuffmanEncoding::~HuffmanEncoding(void)
{
}

void HuffmanEncoding::get_freq_count(string file) {
  char temp;
  FILE *f=fopen(file.c_str(),"rb");
  if (f==NULL) {
    cout<<"ouch, file character count\n";
    return;
  }

  for (int i=0; i<255; i++) {
    char_freq[i] = 0;
  }

  while ((temp=fgetc(f))!=EOF){
    char_freq[temp]++;
    // cout<<temp;
  }

  fclose(f);
}


void HuffmanEncoding::generate_huffman_encoding(HuffmanTree *tr) {
  for (int i=0; i<255; i++) {
    if (char_freq[i])
      tr->add_to_encoding(i, char_freq[i]);
  }

  if (DEBUG) {
    cout<<"\n";
    tr->print_freqs_in_encoding();
  }

  tr->encode();
}

void HuffmanEncoding::print_freq_count() {
  cout<<"\n\nFrequency count:\n";
  cout<<  "----------------\n\n";
  for (map<char, unsigned int>::iterator miter = char_freq.begin(); miter != char_freq.end(); miter++) {
    if (miter->second > 0) {
      cout<<'\''<<miter->first<<"\':  "<<miter->second<<"\n";
    }
  }
}

// HuffmanTreeNode* HuffmanEncoding::encode (char file[]) {
// return NULL;



// }



int main() {
  // HuffmanTree *huff = new HuffmanTree;
  // huff->constructTree("test.txt");
  // cout<<"\n";
  HuffmanEncoding *huff = new HuffmanEncoding;
  HuffmanTree *tr = new HuffmanTree;
  huff->get_freq_count("MoV.txt");
  huff->generate_huffman_encoding(tr);
  if (DEBUG) {
    huff->print_freq_count();
    cout<<"\n\nEncoding:\n";
    cout<<    "---------\n\n";
    tr->print_encoding();
    cout<<"\n";
  }
  return 0;
}