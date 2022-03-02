#include "pch.h"
#include <limits.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <bitset>
#include "Matrix.h"

// Forward declares
void internal_cell(float xin, float yin, bool flag, float* xout, float* yout, float *R);
void boundary_cell(float xin, bool flag, float* yout, float* R);

// Convert the 32-bit binary encoding into hexadecimal
int Binary2Hex(std::string Binary)
{
	std::bitset<32> set(Binary);
	int hex = set.to_ulong();

	return hex;
}

// Convert the 32-bit binary into the decimal
float GetFloat32(std::string Binary)
{
	int HexNumber = Binary2Hex(Binary);

	bool negative = !!(HexNumber & 0x80000000);
	int  exponent = (HexNumber & 0x7f800000) >> 23;
	int sign = negative ? -1 : 1;

	// Subtract 127 from the exponent
	exponent -= 127;

	// Convert the mantissa into decimal using the
	// last 23 bits
	int power = -1;
	float total = 0.0;
	for (int i = 0; i < 23; i++)
	{
		int c = Binary[i + 9] - '0';
		total += (float)c * (float)pow(2.0, power);
		power--;
	}
	total += 1.0;

	float value = sign * (float)pow(2.0, exponent) * total;

	return value;
}

// Get 32-bit IEEE 754 format of the decimal value
std::string GetBinary32(float value)
{
	union
	{
		float input;   // assumes sizeof(float) == sizeof(int)
		int   output;
	}    data;

	data.input = value;

	std::bitset<sizeof(float) * CHAR_BIT>   bits(data.output);

	std::string mystring = bits.to_string<char,
		std::char_traits<char>,
		std::allocator<char> >();

	return mystring;
}

void internal_cell_test_wrapper(const std::vector<float> X, const std::vector<float> Y, const std::vector<bool> flags) {
	float xin, yin = 0;
	float xout, yout = 0;
	bool flag = true;
	float R = 0;

	_ASSERT(X.size() == Y.size() == flags.size());

	for (int i = 0; i < X.size(); i++) {
		xin = X[i];
		yin = Y[i];
		flag = flags[i];
		std::cout << "xin: " << xin << " " << std::hex << Binary2Hex(GetBinary32(xin)) << std::endl;
		std::cout << "yin: " << yin << " " << std::hex << Binary2Hex(GetBinary32(yin)) << std::endl;
		std::cout << "flag: " << flag << std::endl;
		internal_cell(xin, yin, flag, &xout, &yout, &R);
		std::cout << "xout: " << xout << " " << std::hex << Binary2Hex(GetBinary32(xout)) << std::endl;
		std::cout << "yout: " << yout << " " << std::hex << Binary2Hex(GetBinary32(yout)) << std::endl;
		std::cout << "R : " << R << " " << std::hex << Binary2Hex(GetBinary32(R)) << std::endl;
		std::cout << "----------------------------------------------------" << std::endl;
		//flag = !flag;
	}
}

void boundary_cell_test_wrapper(const std::vector<float> X, const std::vector<bool> flags) {
	float xin = 0;
	float yout = 0;
	bool flag = true;
	float R = 0;

	_ASSERT(X.size() == flags.size());

	for (int i = 0; i < X.size(); i++) {
		xin = X[i];
		flag = flags[i];
		std::cout << "xin: " << xin << " " << std::hex << Binary2Hex(GetBinary32(xin)) << std::endl;
		std::cout << "flag: " << flag << std::endl;
		boundary_cell(xin, flag, &yout, &R);
		std::cout << "yout: " << yout << " " << std::hex << Binary2Hex(GetBinary32(yout)) << std::endl;
		std::cout << "R : " << R << " " << std::hex << Binary2Hex(GetBinary32(R)) << std::endl;
		std::cout << "----------------------------------------------------" << std::endl;
		//flag = !flag;
	}
}

void internal_cell(float xin, float yin, bool flag, float* xout, float* yout, float *R) {
	if (flag) {
		*yout = 0.0;
		*xout = xin;
		*R = yin / xin;
	}
	else {
		*yout = yin - (*R)*xin;
		*xout = xin;
	}
}

void boundary_cell(float xin, bool flag, float* yout, float* R) {
	if (flag) {
		*R = float(1) / xin;
		*yout = 0.0;
	}
	else {
		*R = *R;
		*yout = (-1.0)*(*R)*xin;
	}
}