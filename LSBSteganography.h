#pragma once

class LSBSteganography
{
public:
	LSBSteganography(void);
	~LSBSteganography(void);
	void stegify(char[], char[], char[], int);
	void destegify(char[], int);
	void copyImage(char[], char[]);
	void sliceLowerPlane(char[], char[]);
};
