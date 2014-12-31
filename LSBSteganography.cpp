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
	int textCharsLeft = 8;

	while(total<=BMP_HEADER_SIZE && (size=fread(buf,1,1,inIm))>0) {
		total++;
		fwrite(buf,1,size,outIm);
	}
	
	bool isDone = false;

	while (!isDone) {
		if ((temp = getc(f)) != EOF)
			ch = (unsigned char) temp;
		else {
			isDone = true;
			ch = 0;
		}
		
		textCharsLeft = 8;
		
		while (textCharsLeft > 0) {
			if (picCharsLeft != 0) {
				(*buf) |=  (unsigned char)(ch >> (8-picCharsLeft));
				textCharsLeft = 8 - picCharsLeft;
				picCharsLeft = 0;
				fwrite(buf,1,1,outIm);
			}
			else if (numPlanes > textCharsLeft) {
				if (fread(buf, 1, 1, inIm) <= 0) {
					isDone = true;
					break;
				}
				picCharsLeft = numPlanes - textCharsLeft;
				(*buf) = ((unsigned char)(((*buf)>>numPlanes)<<numPlanes) | (unsigned char)((unsigned char)(ch << (8-textCharsLeft))>>(8-picCharsLeft)));
				textCharsLeft = 0;
				if (isDone)
					fwrite(buf,1,1,outIm);
			}
			else {
				if (fread(buf, 1, 1, inIm) <= 0) {
					isDone = true;
					break;
				}
				(*buf) = ((unsigned char)(((*buf)>>numPlanes)<<numPlanes) | (unsigned char)((unsigned char)(ch << (8 - textCharsLeft)) >> (8 - numPlanes)));
				textCharsLeft -= numPlanes;
				fwrite(buf,1,1,outIm);
			}
		}
	}
	
	while (fread(buf,1,1,inIm)>0) {
		fwrite(buf,1,1,outIm);
	}

//	while(fread(buf,1,1,inIm)>0) {
//		if(total>BMP_HEADER_SIZE && !isDone)
//		{
//			*buf &= 255 << numPlanes;
//			if ((temp = getc(f)) == EOF) {
//				ch = 0;
//				isDone = 1;
//			}
//			else
//				ch = (unsigned char) temp;
//			
//			textCharsLeft = picCharsLeft = 8;
//			
//			
////			for (int j = 0; j < (int) (ceil((float) (8.0 / numPlanes))); j++) {
////				if (8 - ((j + 1) * numPlanes) > 0)
////					buf[j] |= ((ch >> (8 - ((j + 1) * numPlanes)))
////							& ~(255 << numPlanes));
////				else if (8 - ((j + 1) * numPlanes) < 0)
////					buf[j] |= ((ch << (((j + 1) * numPlanes) - 8))
////							& ~(255 << numPlanes));
////				else
////					buf[j] |= (ch & ~(255 << numPlanes));
////			}
//			
//		}
//		total++;
//		fwrite(buf,1,1,outIm);
//	}
	fin.close();
	fclose(inIm);
	fclose(outIm);
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
