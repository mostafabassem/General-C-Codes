
//	Author 			: Mostafa Bassem Heiba
// 	Description 		: Advanced Encryption ALgorithm (AES) 
//  	Date			: 24/12/2015

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <time.h> 

using namespace std;
uint16_t MixColumns_LookUp[256] = {
	0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e,
	0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e,
	0x40,0x42,0x44,0x46,0x48,0x4a,0x4c,0x4e,0x50,0x52,0x54,0x56,0x58,0x5a,0x5c,0x5e,
	0x60,0x62,0x64,0x66,0x68,0x6a,0x6c,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e,
	0x80,0x82,0x84,0x86,0x88,0x8a,0x8c,0x8e,0x90,0x92,0x94,0x96,0x98,0x9a,0x9c,0x9e,
	0xa0,0xa2,0xa4,0xa6,0xa8,0xaa,0xac,0xae,0xb0,0xb2,0xb4,0xb6,0xb8,0xba,0xbc,0xbe,
	0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,0xd0,0xd2,0xd4,0xd6,0xd8,0xda,0xdc,0xde,
	0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xee,0xf0,0xf2,0xf4,0xf6,0xf8,0xfa,0xfc,0xfe,
	0x1b,0x19,0x1f,0x1d,0x13,0x11,0x17,0x15,0x0b,0x09,0x0f,0x0d,0x03,0x01,0x07,0x05,
	0x3b,0x39,0x3f,0x3d,0x33,0x31,0x37,0x35,0x2b,0x29,0x2f,0x2d,0x23,0x21,0x27,0x25,
	0x5b,0x59,0x5f,0x5d,0x53,0x51,0x57,0x55,0x4b,0x49,0x4f,0x4d,0x43,0x41,0x47,0x45,
	0x7b,0x79,0x7f,0x7d,0x73,0x71,0x77,0x75,0x6b,0x69,0x6f,0x6d,0x63,0x61,0x67,0x65,
	0x9b,0x99,0x9f,0x9d,0x93,0x91,0x97,0x95,0x8b,0x89,0x8f,0x8d,0x83,0x81,0x87,0x85,
	0xbb,0xb9,0xbf,0xbd,0xb3,0xb1,0xb7,0xb5,0xab,0xa9,0xaf,0xad,0xa3,0xa1,0xa7,0xa5,
	0xdb,0xd9,0xdf,0xdd,0xd3,0xd1,0xd7,0xd5,0xcb,0xc9,0xcf,0xcd,0xc3,0xc1,0xc7,0xc5,
	0xfb,0xf9,0xff,0xfd,0xf3,0xf1,0xf7,0xf5,0xeb,0xe9,0xef,0xed,0xe3,0xe1,0xe7,0xe5
};                // Lookup table for Mix Columns - New Feature
uint16_t SBox[16][16] = {

	{ 0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76 } ,
	{ 0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0 } ,
	{ 0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15 } ,
	{ 0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75 } ,
	{ 0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84 } ,
	{ 0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF } ,
	{ 0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8 } ,
	{ 0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2 } ,
	{ 0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73 } ,
	{ 0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB } ,
	{ 0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79 } ,
	{ 0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08 } ,
	{ 0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A } ,
	{ 0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E } ,
	{ 0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF } ,
	{ 0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16 }
};
unsigned char RC[10] = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36 };
unsigned char PlainText[32]; // Input PlainText
unsigned char Key[32]; // Input Key
unsigned char TextRef[16]; // Input PlainText after Refactoring
unsigned char KeyRef[16]; // Input Key after Refactoring
uint32_t KeyExp[44]; // ExpansionKey
unsigned char ReturnText[16]; // Returned Text After MixColumns *
int encryption_round;
int encryption_numbers;

inline void InputRefactoring(unsigned char(Text)[32], unsigned char(Key)[32], unsigned char(&TextRef)[16], unsigned char(&KeyRef)[16]); // Done
inline void SubstituteBytes(unsigned char(&Text)[16]); // Done
inline void ShiftRows(unsigned char (&Text)[16]); // Done
inline void MixColumns(unsigned char(Text)[16], unsigned char(&ReturnText)[16]); // Done
inline void AddRoundKey(unsigned char (&Text)[16],unsigned char(&Key)[16]); // Done
inline unsigned char GFMultiplication(unsigned char a, unsigned char b); // Done
inline void KeyExpansion(unsigned char(Key)[16], uint32_t(&KeyExp)[44]); // Done
inline void AESEncryption(unsigned char(&Text)[16], uint32_t(&Key)[44]); // Done

int main()
{
	long t1, t2;
	cin >> encryption_numbers;
	for (int i = 0;i < encryption_numbers;i++)
	{
		for (int i = 0; i < 32; i++)
		{
			cin >> PlainText[i];
		}
		for (int i = 0; i < 32; i++)
		{
			cin >> Key[i];
		}
		cin >> encryption_round;
		t1 = clock();
		InputRefactoring(PlainText, Key, TextRef, KeyRef);
		KeyExpansion(KeyRef, KeyExp);
		for (int i = 0;i < encryption_round;i++)
		{
			AESEncryption(TextRef, KeyExp);
		}
		for (int i = 0;i < 16;i++)
		{
			printf("%02X", TextRef[i]);
		}
		printf("\n");

	}
	t2 = clock();
	cout << "Total Time:" << t2 - t1 << endl;
	system("pause");
	return 0;
}
inline void InputRefactoring(unsigned char(Text)[32], unsigned char(Key)[32], unsigned char(&TextRef)[16], unsigned char(&KeyRef)[16])
{
	
	for (int i = 0;i < 31;i+=2)
	{
		if (Text[i] >= '0' && Text[i] <='9' )
		{
			TextRef[i / 2] = (Text[i] - '0');
			TextRef[i / 2] = TextRef[i / 2] << 4;
		}
		else if (Text[i] >= 'A' && Text[i] <= 'F')
		{
			TextRef[i/2] = (Text[i] - 55);
			TextRef[i / 2] = TextRef[i / 2] << 4;
		}
		else if (Text[i] >= 'a'&& Text[i] <= 'f')
		{
			TextRef[i / 2] = (Text[i] - 87);
			TextRef[i / 2] = TextRef[i / 2] << 4;
		}
		else
			;//exit(1);
		if (Text[i+1] >= '0' && Text[i+1] <= '9')
		{
			TextRef[i/2] |= Text[i+1] - '0';
		}
		else if (Text[i + 1] >='A' && Text[i + 1] <='F')
		{
			TextRef[i/2] |= Text[i + 1] - 55;
		}
		else if (Text[i + 1] >= 'a'&& Text[i + 1] <= 'f')
		{
			TextRef[i / 2] |= Text[i + 1] - 87;
		}
		else
			;//exit(1);
		/////////////////
		if (Key[i] >= '0' && Key[i] <= '9')
		{
			KeyRef[i / 2] = (Key[i] - '0');
			KeyRef[i / 2] = KeyRef[i / 2] << 4;
		}
		else if (Key[i] >= 'A' && Key[i] <= 'F')
		{
			KeyRef[i / 2] = (Key[i] - 55);
			KeyRef[i / 2] = KeyRef[i / 2] << 4;
		}
		else if (Key[i] >= 'a'&& Key[i] <= 'f')
		{
			KeyRef[i / 2] = (Key[i] - 87);
			KeyRef[i / 2] = KeyRef[i / 2] << 4;
		}
		else
			;//exit(1);
		if (Key[i + 1] >= '0' && Key[i + 1] <= '9')
		{
			KeyRef[i / 2] |= Key[i + 1] - '0';
		}
		else if (Key[i + 1] >= 'A' && Key[i + 1] <= 'F')
		{
			KeyRef[i / 2] |= Key[i + 1] - 55;
		}
		else if (Key[i + 1] >= 'a'&& Key[i + 1] <= 'f')
		{
			KeyRef[i / 2] |= Key[i + 1] - 87;
		}
		else
			;//exit(1);


	}
}
inline void SubstituteBytes(unsigned char(&Text)[16])
{
	for (int count = 0;count < 16;count++)
	{
		int row = (Text[count] & (0xF0)) >> 4;
		int col = Text[count] & (0x0F);
		Text[count] = SBox[row][col];
	}
}
inline void ShiftRows(unsigned char(&Text)[16])
{
	unsigned char temp1;
	unsigned char temp2;
	// row 2
	temp1 = Text[1];
	Text[1] = Text[5];
	Text[5] = Text[9];
	Text[9] = Text[13];
	Text[13] = temp1;
	// row 3
	temp1 = Text[2];
	temp2 = Text[6];
	Text[2] = Text[10];
	Text[6] = Text[14];
	Text[10] = temp1;
	Text[14] = temp2;
	// row 4
	temp1 = Text[15];
	Text[15] = Text[11];
	Text[11] = Text[7];
	Text[7] = Text[3];
	Text[3] = temp1;
	
}
inline void MixColumns(unsigned char(Text)[16],unsigned char(&ReturnText)[16])
{
	for (int count = 0;count < 16;count++)
	{
		if (count % 4 == 0)
		{
			// S0,j code
			unsigned char temp = Text[count] ^ Text[count + 1] ^ Text[count + 2] ^ Text[count + 3];
			//ReturnText[count] = Text[count] ^ temp ^ (GFMultiplication(2,(Text[count] ^ Text[count+1])));           // Old Implementation - Computational with no LookUp Table
			ReturnText[count] = Text[count] ^ temp ^ (MixColumns_LookUp[(Text[count] ^ Text[count + 1])]);            // New Implementation - Faster Lookup table

		}
		else if (count % 4 == 1)
		{
			//S1,j code
			unsigned char temp = Text[count - 1] ^ Text[count] ^ Text[count + 1] ^ Text[count + 2];
			//ReturnText[count] = Text[count] ^ temp ^ (GFMultiplication(2, (Text[count] ^ Text[count + 1])));           // Old Implementation - Computational with no LookUp Table
			ReturnText[count] = Text[count] ^ temp ^ (MixColumns_LookUp[(Text[count] ^ Text[count + 1])]);            // New Implementation - Faster Lookup table
		}
		else if (count % 4 == 2)
		{
			//S2,j code
			unsigned char temp = Text[count - 2] ^ Text[count - 1] ^ Text[count] ^ Text[count + 1];
			//ReturnText[count] = Text[count] ^ temp ^ (GFMultiplication(2, (Text[count] ^ Text[count + 1])));           // Old Implementation - Computational with no LookUp Table
			ReturnText[count] = Text[count] ^ temp ^ (MixColumns_LookUp[(Text[count] ^ Text[count + 1])]);            // New Implementation - Faster Lookup table
		}
		else if (count % 4 == 3)
		{
			//S3,j code
			unsigned char temp = Text[count - 3] ^ Text[count - 2] ^ Text[count - 1] ^ Text[count];
			//ReturnText[count] = Text[count] ^ temp ^ (GFMultiplication(2, (Text[count] ^ Text[count - 3])));           // Old Implementation - Computational with no LookUp Table
			ReturnText[count] = Text[count] ^ temp ^ (MixColumns_LookUp[(Text[count] ^ Text[count - 3])]);            // New Implementation - Faster Lookup table
		}
	}

}
inline void AddRoundKey(unsigned char(&Text)[16], unsigned char(&Key)[16])
{
	for (int count = 0;count < 16;count++)
	{
		Text[count] = Text[count] ^ Key[count];
	}
}
inline unsigned char GFMultiplication(unsigned char a, unsigned char b) {
	unsigned char p = 0; /* the product of the multiplication */
	while (b) {
		if (b & 1) /* if b is odd, then add the corresponding a to p (final product = sum of all a's corresponding to odd b's) */
			p ^= a; /* since we're in GF(2^m), addition is an XOR */

		if (a & 0x80) /* GF modulo: if a >= 128, then it will overflow when shifted left, so reduce */
			a = (a << 1) ^ 0x11b; /* XOR with the primitive polynomial x^8 + x^4 + x^3 + x + 1 -- you can change it but it must be irreducible */
		else
			a <<= 1; /* equivalent to a*2 */
		b >>= 1; /* equivalent to b // 2 */
	}
	return p;
}
inline void KeyExpansion(unsigned char(Key)[16], uint32_t(&KeyExp)[44])
{
	uint32_t temp;
	for (int count = 0;count < 4;count++)
	{
		KeyExp[count] = (Key[count * 4] << 24) | (Key[count * 4 + 1] << 16) | (Key[count * 4 + 2] << 8) | Key[count * 4 + 3];
	}
	for (int count = 4;count < 44;count++)
	{
		temp = KeyExp[count - 1];
		if (count % 4 == 0)
		{
			unsigned char B0 = (temp >> 24);
			unsigned char B1 = (temp >> 16);
			unsigned char B2 = (temp >> 8);
			unsigned char B3 = temp;
			int row = (B0 & (0xF0)) >> 4;
			int col = B0 & (0x0F);
			B0 = SBox[row][col];
			row = (B1 & (0xF0)) >> 4;
			col = B1 & (0x0F);
			B1 = SBox[row][col];
			row = (B2 & (0xF0)) >> 4;
			col = B2 & (0x0F);
			B2 = SBox[row][col];
			row = (B3 & (0xF0)) >> 4;
			col = B3 & (0x0F);
			B3 = SBox[row][col];
			temp = B1 << 24 | B2 << 16 | B3 << 8 | B0;
			unsigned char tempRCon = RC[(count / 4) - 1];
			uint32_t RCON = tempRCon << 24;
			temp = temp^RCON;

		}
		KeyExp[count] = KeyExp[count - 4] ^ temp;
	}
}
inline void AESEncryption(unsigned char(&Text)[16], uint32_t (&Key)[44])
{
	unsigned char TempText[16];
	// Pre Rounds
	unsigned char temp[16];
	for (int count = 0;count < 4;count++)
	{
		temp[count*4] = Key[count] >> 24;
		temp[count*4 + 1] = Key[count] >> 16;
		temp[count*4 + 2] = Key[count] >> 8;
		temp[count*4 + 3] = Key[count];
		Text[count*4] = Text[count*4] ^ temp[count*4];
		Text[count*4 + 1] = Text[count*4 + 1] ^ temp[count*4 + 1];
		Text[count*4 + 2] = Text[count*4 + 2] ^ temp[count*4 + 2];
		Text[count*4 + 3] = Text[count*4 + 3] ^ temp[count*4 + 3];
	}

	// 9 Rounds

	for (int Round = 0; Round < 9; Round++)
	{
		for (int count = 0;count < 4;count++)
		{
			temp[count * 4] = Key[Round * 4 + 4 + count] >> 24;
			temp[count * 4 + 1] = Key[Round * 4 + 4 + count] >> 16;
			temp[count * 4 + 2] = Key[Round * 4 + 4 + count] >> 8;
			temp[count * 4 + 3] = Key[Round * 4 + 4 + count];
		}
		SubstituteBytes(Text);
		ShiftRows(Text);
		MixColumns(Text, TempText);
		AddRoundKey(TempText, temp);
		for (int count = 0;count < 16;count++)
		{
			Text[count] = TempText[count];
		} // returning Text Value to Text Array

	}

	// 10th Round
	for (int count = 0;count < 4;count++)
	{
		temp[count * 4] = Key[count + 40] >> 24;
		temp[count * 4 + 1] = Key[count + 40] >> 16;
		temp[count * 4 + 2] = Key[count + 40] >> 8;
		temp[count * 4 + 3] = Key[count + 40];
	}
	SubstituteBytes(Text);
	ShiftRows(Text);
	AddRoundKey(Text, temp);
	/*for (int i = 0;i < 16;i++)
	{
		printf("%02X", Text[i]);
	}*/

}
