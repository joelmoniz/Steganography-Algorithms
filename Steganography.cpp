#include "Steganography.h"
#include "HuffmanTree.h"
#include "LSBSteganography.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

Steganography::Steganography(void) {
}

Steganography::~Steganography(void) {
}

void profile(char input[], char output1[], char output2[], char output3[]);
void test();

int main() {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	cout<<"Initializing...\n";
	LSBSteganography *lsbSteg = new LSBSteganography;
//	lsbSteg->stegify("Koala.bmp", "Koala5.bmp", "organizations.csv", 5);
	lsbSteg->improvedStegify("Koala.bmp", "Koala5ii.bmp", "organizations.csv", 5);
	cout << "Stegification done...\n";

	test();

//cout<<"DeStegifying...\n";
//lsbSteg->destegify("Koala5.bmp",5);

//	HuffmanTree *ht = new HuffmanTree;
	//ht->minHeapify();
//	ht->getFreqIntoArray("test.txt");

//	profile("Koala.bmp", "Koala_p1.bmp", "Koala_p2.bmp", "Koala_p3.bmp");
	return 0;
}

void profile(char input[], char output1[], char output2[], char output3[]) {
	int x[] = {40, 20, 34, 224, 12,13,20001};
	int i;
	char buf[2];
	FILE *inIm;
	if ( (inIm= fopen(input,"rb")) == NULL ) {
		cout<<"ouch, inim\n";
		return;
	}

	FILE *outIm;
	if ( (outIm = fopen(output1,"wb")) == NULL ) {
		cout<<"ouch, outim1\n";
		return;
	}

	int total = 0;

	int size;
	while(total<=54 && (size=fread(buf,1,1,inIm)>0)) {
		total++;
		fwrite(buf,1,size,outIm);
	}

	clock_t begin = clock();

	while((size=fread(buf,1,1,inIm)>0)) {
		*buf = ((*buf) >> 5) << 5;
		fwrite(buf,1,size,outIm);
	}

	clock_t end = clock();

	cout << "Done with >><<, time:  " << double(end - begin) / CLOCKS_PER_SEC << "\n";

	fclose(outIm);
	fclose(inIm);
	if ( (inIm= fopen(input,"rb")) == NULL ) {
		cout<<"ouch, inim\n";
		return;
	}

	if ( (outIm = fopen(output2,"wb")) == NULL ) {
		cout<<"ouch, outim2\n";
		return;
	}
	total = 0;
	while(total<=54 && (size=fread(buf,1,1,inIm)>0)) {
		total++;
		fwrite(buf,1,size,outIm);
	}

	begin = clock();

	while((size=fread(buf,1,1,inIm)>0)) {
		*buf = ((*buf) & x[3]);
		fwrite(buf,1,size,outIm);
	}

	end = clock();
	cout << "Done with array access, time:  " << double(end - begin) / CLOCKS_PER_SEC << "\n";

	fclose(outIm);
	fclose(inIm);
	if ( (inIm= fopen(input,"rb")) == NULL ) {
		cout<<"ouch, inim\n";
		return;
	}

	if ( (outIm = fopen(output3,"wb")) == NULL ) {
		cout<<"ouch, outim2\n";
		return;
	}

	total = 0;
	while(total<=54 && fread(buf,1,1,inIm)>0) {
		total++;
		fwrite(buf,1,1,outIm);
	}

	begin = clock();

	while(fread(buf,1,1,inIm)>0) {
		*buf = ((*buf) & 224);
		fwrite(buf,1,1,outIm);
	}

	end = clock();
	cout << "Done with direct &, time:  " << double(end - begin) / CLOCKS_PER_SEC;

}

void test() {
	unsigned char buf[2];
	buf[0] = 170;
//	cout<<(unsigned int)*buf;
//	(*buf) = (((*buf) >> 3) << 3) | 4;
//	cout << (unsigned int)*buf;
//	(*buf) |= (207 >> (8-2));
//	cout << (unsigned int)*buf;

	unsigned char ch = 170;
	int textCharsLeft = 6;
	int numPlanes = 3;
	cout<<(int)ch<<"\n";
	cout <<((unsigned char)(ch << (8 - textCharsLeft)) >> (8 - numPlanes))<<'\n';

	int picCharsLeft = 5; textCharsLeft = 4;
	cout<<(int)(((unsigned char)(((unsigned char)(ch << (8-textCharsLeft)))>>(8-picCharsLeft))))<<'\n';
}
