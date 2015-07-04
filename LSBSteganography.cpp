#include "LSBSteganography.h"
#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;

const int sil = 2;

LSBSteganography::LSBSteganography(void)
{
}

LSBSteganography::~LSBSteganography(void)
{
}


void LSBSteganography::print_int_in_binary(unsigned char a) {
	int x = 0;
	for (int i=0;i<sizeof(unsigned char)*8; i++) {
		x <<= 1;
		x |= a & 1;
		a >>= 1;
	}
	for (int i=0;i<sizeof(unsigned char)*8; i++) {
		cout<<(x&1);
		x >>= 1;
	}
}

/**
 * Reads in characters from text, reads in an image from input, and
 * writes an image with steganographically embedded text into output.
 *
 * The disadvantage of this function is that for every byte of text, it uses
 * ceil(8/numPlanes) bytes, i.e., it wastes ceil(8/numPlanes)*numPlanes - 8 bits
 */
void LSBSteganography::stegify(char input[], char output[], char text[], int numPlanes){

	// File pointer to read from image
	FILE *inIm;
	if ( (inIm= fopen(input,"rb")) == NULL ) {
		cout<<"ouch, inim\n";
		return;
	}

	// File pointer to write to new image
	FILE *outIm;
	if ( (outIm = fopen(output,"wb")) == NULL ) {
		cout<<"ouch, outim\n";
		return;
	}

	unsigned char buf[8];
	size_t size;

	int total=0;
	int isDone = 0;

	ifstream fin(text);
	FILE *f;
	if ((f=fopen(text,"r"))==NULL) {
		cout<<"ouch, text\n";
		return;
	}

	char temp;
	unsigned char ch;

	// read as many bytes of image required to store 1 byte of text into buffer
	while((size=fread(buf,1,(int)(ceil((float)(8.0/numPlanes))),inIm))>0) {

		// start "steganographic encoding" only if we are neither in the image header nor
		// the last few bytes of the image, and if all the text to be hidden has not yet been
		// hidden
		if(total>BMP_HEADER_SIZE && size==(int)(ceil((float)(8.0/numPlanes))) && !isDone)
		{
			// make lower numPlanes number of bits 0
			for(int j=0; j<(int)(ceil((float)(8.0/numPlanes))); j++)
				buf[j] &= 255 << numPlanes;

			if ((temp=getc(f))==EOF){
				// write in a byte with value 0 to indicate end of text
				ch = 0;
				isDone = 1;
			}
			else
				ch = (unsigned char)temp;

			//
			for (int j = 0; j < (int) (ceil((float) (8.0 / numPlanes))); j++) {
				// if there are more than numPlanes chars left to be written
				if (8 - ((j + 1) * numPlanes) > 0)
					buf[j] |= ((ch >> (8 - ((j + 1) * numPlanes)))
							& ~(255 << numPlanes));
				// if there are less than numPlanes chars left to be written
				else if (8 - ((j + 1) * numPlanes) < 0)
					buf[j] |= ((ch << (((j + 1) * numPlanes) - 8))
							& ~(255 << numPlanes));
				// if there are exactly numPlanes chars left to be written
				else
					buf[j] |= (ch & ~(255 << numPlanes));
			}
		}

		total+=size;
		fwrite(buf,1,size,outIm);
	}
	fin.close();
	fclose(inIm);
	fclose(outIm);
}

void LSBSteganography::improvedStegify(char input[], char output[], char text[], int numPlanes){

	FILE *inIm;
	if ( (inIm= fopen(input,"rb")) == NULL ) {
		cout<<"ouch, inim\n";
		return;
	}
//	else
//		cout<<inIm<<"\n";

	FILE *outIm;
	if ( (outIm = fopen(output,"wb")) == NULL ) {
		cout<<"ouch, outim\n";
		return;
	}
	
	size_t size;

	unsigned char buf[2];

	int total=0;

	ifstream fin(text);
	FILE *f;
	if ((f=fopen(text,"r"))==NULL) {
		cout<<"ouch, text\n";
		return;
	}

	char temp;
	unsigned char ch;

	int picCharsLeft = 0;//numPlanes;
	int textCharsLeft = 0;

	while(total<=BMP_HEADER_SIZE && (size=fread(buf,1,1,inIm))>0) {
		total++;
		fwrite(buf,1,size,outIm);
		// cout<<"steg inout: ";print_int_in_binary(*buf);cout<<"\n";
	}
	
	bool isDone = false, endWritten = false;

	cout<<"Oof ";print_int_in_binary(7);cout<<"\n";

	while (!isDone) {
		if (!isDone) {
			if (textCharsLeft == 0) {
				if ((temp = getc(f)) == EOF) {
					isDone = true;
					ch = 0;
				}
				else {
					ch = (unsigned char) temp;
					// cout<<"plain in: ";print_int_in_binary(ch);cout<<"\n";
				}
				textCharsLeft = 8;
			}

			if (picCharsLeft == 0) {
				if (fread(buf, 1, 1, inIm) <= 0) {
					isDone = true;
					break;
				}
				else {
					*buf = ((unsigned char)((unsigned char)(*buf >> numPlanes)) << numPlanes);
					picCharsLeft = numPlanes;
					// cout<<"Steg in: ";print_int_in_binary(*buf);cout<<"\n";
				}
			}
		}
		else if (textCharsLeft == 0) {
			if (picCharsLeft != 0) {
				fwrite(buf,1,1,outIm);
				// cout<<"Last steg out: ";print_int_in_binary(*buf);cout<<"\n";
			}
			break;
		}
		else { // Shouldn't happen
			cout<<"Clearly I'm not all that good at verifying program correctness\n";
		}

		if (textCharsLeft < picCharsLeft) {
			*buf |= (unsigned char)(ch << (picCharsLeft - textCharsLeft));
			picCharsLeft -= textCharsLeft;
			textCharsLeft = 0;
		}
		else if (picCharsLeft < textCharsLeft) {
			// cout<<"Shift op ";print_int_in_binary(ch);cout<< "<<"<<(8 - textCharsLeft)<<">>"<<(8-picCharsLeft)<<"=";print_int_in_binary((ch << (8 - textCharsLeft)) >> (8 - picCharsLeft )); cout << "\n";
			*buf |= (unsigned char)((unsigned char)(ch << (8 - textCharsLeft)) >> (8 - picCharsLeft ));
			textCharsLeft -= picCharsLeft;
			picCharsLeft = 0;
			ch = (unsigned char)((unsigned char)(ch << (8-textCharsLeft)) >> (8-textCharsLeft));
			fwrite(buf,1,1,outIm);
			// cout<<"Steg out: ";print_int_in_binary(*buf);cout<<"\n";
		}
		else {
			*buf |= ch;
			fwrite(buf,1,1,outIm);
			// cout<<"Steg out: ";print_int_in_binary(*buf);cout<<"\n";
			picCharsLeft = textCharsLeft = 0;
		}
	}
	
	while (fread(buf,1,1,inIm)>0) {
		fwrite(buf,1,1,outIm);
		// cout<<"Nosteg inout: ";print_int_in_binary(*buf);cout<<"\n";
	}

	fin.close();
	fclose(inIm);
	fclose(outIm);
}

void LSBSteganography::improvedDestegify(char input[], int numPlanes){

	FILE *inIm;
	if ( (inIm= fopen(input,"rb")) == NULL ) {
		cout<<"ouch, inim\n";
		return;
	}

	size_t size;

	unsigned char buf[2];

	int total=0;

	unsigned char ch = 0;

	int picCharsLeft = 0;
	int textCharsLeft = 8;

	while(total<=BMP_HEADER_SIZE && (size=fread(buf,1,1,inIm))>0) {
		total++;
	}
	
	bool isDone = false, endWritten = false;

	cout<<"\nMessage:"<<"\n";

	while (!(textCharsLeft == 0 && ch == 0)) {
		if (textCharsLeft == 0) {
			cout<<ch;
			// cout<<"Ans out:";print_int_in_binary(ch);cout<<"\n";
			ch = 0;
			textCharsLeft = 8;
		}

		if (picCharsLeft == 0) {
			if (fread(buf, 1, 1, inIm) <= 0) {
				break;
			}
			else {
				picCharsLeft = numPlanes;
				// cout<<"Im in:";print_int_in_binary(*buf);cout<<"\n";
			}
		}

		ch |= ((unsigned char)(*buf << (8 - picCharsLeft))) >> (8 - textCharsLeft);

		if (textCharsLeft < picCharsLeft) {
			picCharsLeft -= textCharsLeft;
			textCharsLeft = 0;
		}
		else if (picCharsLeft < textCharsLeft) {
			textCharsLeft -= picCharsLeft;
			picCharsLeft = 0;
		}
		else {
			picCharsLeft = textCharsLeft = 0;
		}
	}
	
	fclose(inIm);
}

void LSBSteganography::copyImage(char input[], char output[]) {
	FILE *inIm;
	if ( (inIm= fopen(input,"rb")) == NULL ) {
		cout<<"ouch, inim\n";
		return;
	}
	FILE *outIm;
	if ( (outIm = fopen(output,"wb")) == NULL ) {
		cout<<"ouch, outim\n";
		return;
	}
	unsigned char buf[256];
	size_t size;
	//	int total=0;
	while((size=fread(buf,1,sil,inIm))>0) {
		//		if(total!=0 && size==sil)	
		//			for (int i = 0; i<size;i++)
		//				buf[i] -= buf[i]%32;
		//		else
		//			for (int i = 29; i<sil;i++)
		//				buf[i] -= buf[i]%32;
		//		total++;
		fwrite(buf,1,size,outIm);
	}
	fclose(inIm);
	fclose(outIm);
}



void LSBSteganography::sliceLowerPlane(char input[], char output[]) {
	FILE *inIm;
	if ( (inIm= fopen(input,"rb")) == NULL ) {
		cout<<"ouch, inim\n";
		return;
	}
	FILE *outIm;
	if ( (outIm = fopen(output,"wb")) == NULL ) {
		cout<<"ouch, outim\n";
		return;
	}
	unsigned char buf[256];
	size_t size;
	int total=0;
	while((size=fread(buf,1,sil,inIm))>0) {
		if(total!=0 && size==sil)	
			for (int i = 0; i<size;i++)
				buf[i] -= buf[i]%16;
		else
			for (int i = 29; i<sil;i++)
				buf[i] -= buf[i]%16;
		total++;
		fwrite(buf,1,size,outIm);
	}
	fclose(inIm);
	fclose(outIm);
}




void LSBSteganography::destegify(char input[], int numPlanes){
	FILE *inIm;
	if ( (inIm= fopen(input,"rb")) == NULL ) {
		cout<<"ouch, inim\n";
		return;
	}

	unsigned char buf[8];
	size_t size;

	int total=0;
	int isDone = 0;

	unsigned char ch;
	while((size=fread(buf,1,(int)(ceil((float)(8.0/numPlanes))),inIm))>0 && !isDone) {
		if(total>BMP_HEADER_SIZE && size==(int)(ceil((float)(8.0/numPlanes))))
		{
			//buf[0] = (buf[0] & 15);
			//buf[1] = (buf[1] & 15);

			if (!isDone) {
				//ch = buf[0] << 4;
				//ch = ch | buf[1];
				
				//cout<<ch<<" "<<(int)ch<<"  "<<(int)(buf[0] << 4)<<" "<<(int)(buf[1])<<"\n";

				for (int i=0; i<(int)(ceil((float)(8.0/numPlanes))); i++) { 
					buf[i] &= ~(255<<(numPlanes));//(((unsigned int)255) >> (8-numPlanes));
					if (i==0) {
						ch = buf[i];
					}
					else if (i==((int)(ceil((float)(8.0/numPlanes)))-1)) {
						ch = ch << (numPlanes-(numPlanes*(int)(ceil((float)(8.0/numPlanes)))-8));
						ch |= buf[i]>>(numPlanes*(int)(ceil((float)(8.0/numPlanes)))-8);
					}
					else {
						ch = ch << numPlanes;
						ch |= buf[i];
					}
				} 
				if (ch==0)
					isDone = 1;
				else
					cout<<ch;
			}
		}
		total+=size;
	}
	fclose(inIm);
}
