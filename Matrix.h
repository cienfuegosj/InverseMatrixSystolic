#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <queue>

template <typename T> class Matrix;

template <typename T>
std::istream& operator>>(std::istream& in, Matrix<T>& matrix);

template <typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& matrix);


template <typename T>
class Matrix {
	/*
		`elements` is a 2D array that is
		 a vector of the rows of the matrix.
	*/
	std::vector< std::vector<T> > elements;

public:
	Matrix(std::vector< std::vector<T> >& elem)
		: elements(elem) { 
		cols = elements[0].size();
		rows = elements.size();
	}
	Matrix(std::vector< std::vector<T> >&& elem)
		: elements(std::move(elem)) {}

	int cols;
	int rows;
	/*
		COPY and MOVE constructors
	*/
	Matrix(Matrix& m)
		: elements(m.elements) {}
	Matrix(Matrix&& m)
		: elements(std::move(m.elements)) {}

	/*
		BAREBONES constructor
	*/
	Matrix(const int rows, const int cols)
		: elements(std::vector< std::vector<T> >(rows, std::vector<T>(cols))) {}

	/*
		DEFAULT destructor, nothing to do.
	*/
	~Matrix() {}

	/*
		BEGIN FUNCTION PROTOTYPES
	*/
	void operator=(Matrix<T>& other);
	void operator=(Matrix<T>&& other);

	// column/row num getters
	int num_cols();
	int num_rows();

	// get all elements
	const std::vector< std::vector<T> >& get_elements();

	// subscript operator,
	// @param rhs [int] an 0 <= index < num_cols
	// @return a vector
	std::vector<T>& operator[](const int &rhs);
	/*
		Matrix Arithmetic Methods
	*/


	// ADDITION:

	// Binary operator to add two matrices together.
	// Does not modify either matrix.

	// @param other the Matrix on the right side of +

	// @return a Matrix, whose elements are the sums of
	// the corresponding elements of `this` and `other`

	Matrix<T> operator+(Matrix<T> &other);

	// SUBTRACTION:

	// Binary operator to subtract two matrices.
	// Does not modify either matrix.

	// @param other the Matrix on the right side of -

	// @return a Matrix, whose elements are the differences of
	// the corresponding elements of `this` and `other`

	Matrix<T> operator-(Matrix<T> &other);

	// MATRIX*MATRIX MULTIPLICATION

	// Binary operator to multiple two matrices.
	// Does not modify either matrix.

	// @param other the Matrix on the right side of *

	// @return a Matrix, whose elements are the products of
	// the corresponding elements of `this` and `other`, following
	// the accepted way of multiplying matrices.

	Matrix<T> operator*(Matrix<T> &other);

	// MATRIX*SCALAR MULTIPLICATION

	// Multiplies a Matrix by a scalar value.

	// @param other the scalar value

	// @return the result of scaling `this` by `other`

	Matrix<T> operator*(const T& other);

	// Matrix Power Method
	// Powers >= 1 only!

	// @param p an unsigned integer >= 1 to which the Matrix will
	// be raised.

	// @return the Matrix raised to the `p` power

	Matrix<T> power(const unsigned int &p);

	/*
		Matrix Determinant
	*/
	T determinant(void);

	// Get matrix row by index
	std::vector<T> get_Row(int row_index);

	Matrix<T> get_Transpose();

	// I/O functions
	friend std::istream& operator>> <>(std::istream& in, Matrix<T>& matrix);
	friend std::ostream& operator<< <>(std::ostream& out, const Matrix<T>& matrix);
};

#include "pch.h"
#include "Matrix.h"

template <typename T>
std::istream& operator>>(std::istream& in, Matrix<T>& matrix) {
	for (auto& row : matrix.elements) {
		for (auto& elem : row) {
			in >> elem;
		}
	}
	return in;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& matrix) {
	for (const auto& row : matrix.elements) {

		out << "| ";

		for (const auto& elem : row)
			out << elem << " ";

		out << "|" << std::endl;
	}

	return out;
}

template <typename T>
void Matrix<T>::operator=(Matrix<T>& other) {
	elements = other.elements;
}

template <typename T>
void Matrix<T>::operator=(Matrix<T>&& other) {
	elements = std::move(other.elements);
}

template <typename T>
int Matrix<T>::num_cols() {
	return elements[0].size();
}

template <typename T>
int Matrix<T>::num_rows() {
	return elements.size();
}

template <typename T>
const std::vector< std::vector<T> >& Matrix<T>::get_elements() {
	return elements;
}

template <typename T>
std::vector<T>& Matrix<T>::operator[](const int &rhs) {
	if (abs(rhs) > num_rows()) {
		throw std::invalid_argument("operator[] in Matrix out of bounds");
	}

	if (rhs >= 0) {
		return elements[rhs];
	}
	else {
		return elements[num_rows() + rhs];
	}

}

template <typename T>
Matrix<T> Matrix<T>::operator+(Matrix<T> &other) {
	// Matrices must be same dimensions to
	// be addable
	if (this->num_rows() != other.num_rows() || this->num_cols() != other.num_cols())
		throw std::invalid_argument("these Matrices cannot be added/subtracted");

	// make matrix based on `this`
	Matrix<T> resultant(*this);

	for (int i = 0, n = this->num_rows(); i < n; i++) {
		for (int j = 0, o = this->num_cols(); j < o; j++) {
			// set each value in the resultant equal
			// to the sum of the corresponding values
			// in `this` and `other`
			resultant[i][j] = (*this)[i][j] + other[i][j];
		}
	}
	return resultant;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(Matrix<T> &other) {
	if (this->num_rows() != other.num_rows() || this->num_cols() != other.num_cols())
		throw std::invalid_argument("these Matrices cannot be added/subtracted");

	Matrix<T> resultant(*this);

	for (int i = 0, n = this->num_rows(); i < n; i++) {
		for (int j = 0, o = this->num_cols(); j < o; j++) {
			resultant[i][j] = (*this)[i][j] - other[i][j];
		}
	}
	return resultant;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(Matrix<T> &other) {
	Matrix<T> resultant(this->num_rows(), other.num_cols());

	if (this->num_cols() != other.num_rows())
		throw std::invalid_argument("these Matrices cannot be multiplied");

	// go through all rows of the left Matrix
	for (int i = 0, n = this->num_rows(); i < n; i++) {

		// go through all columns of right Matrix for each row
		// of the left Matrix

		for (int j = 0, o = other.num_cols(); j < o; j++) {

			T tmp = 0;

			int x = 0;
			while (x < n) {
				tmp += ((*this)[i][x] * other[x][j]);
				x++;
			}

			resultant[i][j] = tmp; // put the tmp into the resultant
		}
	}
	return resultant;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T& other) {
	Matrix<T> resultant(*this);

	for (int i = 0, n = this->num_rows(); i < n; i++) {
		for (int j = 0, o = this->num_cols(); j < o; j++) {
			resultant.elements[i][j] *= other;
		}
	}

	return resultant;
}

template <typename T>
Matrix<T> Matrix<T>::power(const unsigned int &p) {
	if (p == 1)
		return *this;
	else if (p <= 0)
		throw std::out_of_range("can't raise to a negative power right now");

	Matrix m(*this);

	m = m.power(p / 2);
	m = m * m;

	if (p % 2 == 1)
		m = m * (*this);

	return m;
}

template <typename T>
T Matrix<T>::determinant(void) {

	if (this->num_cols() != this->num_rows())
		throw std::logic_error("Finding the determinant of a matrix is undefined for non-square matrices.");

	if (this->num_cols() == 1)
		return elements[0][0];
	else if (this->num_cols() == 2)
		return elements[0][0] * elements[1][1] - elements[0][1] * elements[1][0];

	T det = 0;
	bool neg = false;
	for (size_t i = 0; i < this->num_cols(); ++i, neg = !neg) {
		Matrix m(this->num_rows() - 1, this->num_cols() - 1);

		for (size_t bm_row = 1, sm_row = 0; bm_row < this->num_rows() && sm_row < m.num_rows(); ++bm_row, sm_row++) {
			for (size_t bm_col = 0, sm_col = 0; bm_col < this->num_cols() && sm_col < m.num_cols(); ++bm_col) {
				if (bm_col == i) continue;
				m.elements[sm_row][sm_col++] = this->elements[bm_row][bm_col];
			}
		}
		det += this->elements[0][i] * m.determinant() * (neg ? -1 : 1);
	}
	return det;
}

template <typename T>
Matrix<T> Matrix<T>::get_Transpose() {
	Matrix<T> m(cols, rows);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			m.elements[i][j] = this->elements[j][i];
		}
	}
	return m;
}

template <typename T>
std::vector<T> Matrix<T>::get_Row(int row_index) {
	return elements[row_index];
}