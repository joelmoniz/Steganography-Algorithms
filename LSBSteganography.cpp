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

void LSBSteganography::stegify(char input[], char output[], char text[], int numPlanes){
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
	while((size=fread(buf,1,(int)(ceil((float)(8.0/numPlanes))),inIm))>0) {
		if(total>=51 && size==(int)(ceil((float)(8.0/numPlanes))) && !isDone)	
			//for (int i = 0; i<size;i++) 
		{
			//buf[0] = (buf[0] & 240);
			//buf[1] = buf[1] & 240;
			for(int j=0; j<(int)(ceil((float)(8.0/numPlanes))); j++)
				buf[j] &= 255 << numPlanes;
			//fin>>ch;
			//ch=getc(f);
			if ((temp=getc(f))==EOF){//(fin.eof()) {
				ch = 0;
				isDone = 1;
			}
			else
				ch = (unsigned char)temp;
			//if (!fin.eof()) {
				//cout<<ch<<" "<<(int)ch<<"  "<<(int)(( ch & 240 )/16)<<" "<<(int)( ch & 15 )<<"\n";
				//buf[0] += ( ch & 240 )/16;
				//buf[1] += ( ch & 15 );
			//cout<<'\n'<<ch<<" "<<(int)ch<<" ";
				for (int j=0; j<(int)(ceil((float)(8.0/numPlanes))); j++) {//cout<<(int)buf[j]<<" ";
					if (8-((j+1)*numPlanes) > 0)
						buf[j] |= ((ch >> (8-((j+1)*numPlanes))) & ~(255 << numPlanes));
					else if (8-((j+1)*numPlanes) < 0)
						buf[j] |= ((ch << (((j+1)*numPlanes)-8)) & ~(255 << numPlanes));
					else
						buf[j] |= (ch  & ~(255 << numPlanes));
					//cout<<(int)buf[j]<<" ";
				}
					
			//}
			//else if (!isDone) {
			//	ch = 0;
		//		buf[0] += ( ch & 240 )/16;
	//			buf[1] += ( ch & 15 );
		//		isDone = 1;
		//	}
		}

		total+=size;
		fwrite(buf,1,size,outIm);
	}
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
		if(total>=51 && size==(int)(ceil((float)(8.0/numPlanes))))
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