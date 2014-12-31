#pragma once

class LSBSteganography
{
	static const int BMP_HEADER_SIZE = 54;
public:
	LSBSteganography(void);
	~LSBSteganography(void);
	void stegify(char[], char[], char[], int);
	void improvedStegify(char[], char[], char[], int);
	void destegify(char[], int);
	void copyImage(char[], char[]);
	void sliceLowerPlane(char[], char[]);
};
