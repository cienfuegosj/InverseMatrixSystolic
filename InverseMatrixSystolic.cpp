// InverseMatrixSystolic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <limits.h>
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <bitset>
#include "IAMBoundaryCell.h"
#include "IAMInternalCell.h"
#include "MSGRBoundaryCell.h"
#include "MSGRInternalCell.h"
#include "Matrix.h"


struct IAMSystolicArray {
public:
	IAMSystolicArray(int matrix_size) : N(matrix_size) {
		_ASSERT(N % 2 == 0);
		// Create IAMBoundary Cells
		for (int i = 0; i < N; i++) {
			boundary_cells.push_back(new IAMBoundaryCell(i));
		}
		// Create IAMInternal Cells
		for (int i = 0; i < N*(N - 1) / 2; i++) {
			internal_cells.push_back(new IAMInternalCell(i));
		}
	};
	~IAMSystolicArray() {
		for (int i = 0; i < boundary_cells.size(); i++) {
			delete boundary_cells[i];
		}
		for (int j = 0; j < internal_cells.size(); j++) {
			delete internal_cells[j];
		}
	}
	int Execute(Matrix<float>* IntegratedMatrix, Matrix<bool> flags);

	void Debug() {
		// IAMBoundary Cell Statistic Dump
		for (int i = 0; i < boundary_cells.size(); i++) {
			boundary_cells[i]->DumpHistory();
		}

		// IAMInternal Cell Statistic Dump
		for (int i = 0; i < internal_cells.size(); i++) {
			internal_cells[i]->DumpHistory();
		}
	}

	std::vector<IAMBoundaryCell*> boundary_cells;
	std::vector<IAMInternalCell*> internal_cells;

private:
	int N;
	bool done = false;
};

struct MSGRSystolicArray {
public:
	MSGRSystolicArray(int matrix_size) : N(matrix_size) {
		_ASSERT(matrix_size % 2 == 0);
		// Create MSGR Boundary Cells
		for (int i = 0; i < N; i++) {
			boundary_cells.push_back(new MSGRBoundaryCell(i));
		}
		// Create MSGR Internal Cells
		for (int j = 0; j < N*(N - 1) / 2; j++) {
			internal_cells.push_back(new MSGRInternalCell(j));
		}
	}
	~MSGRSystolicArray() {
		// Deallocate all memory allocated
		for (int i = 0; i < boundary_cells.size(); i++) {
			delete boundary_cells[i];
		}

		for (int j = 0; j < internal_cells.size(); j++) {
			delete internal_cells[j];
		}
	}

	int Execute(Matrix<float>* A, Matrix<float>* I, Matrix<bool> flags);

	void Debug() {
		// Print out the boundary cell history
		for (int i = 0; i < N; i++) {
			boundary_cells[i]->DumpHistory();
		}
		// Print out the internal cell hBistory
		for (int j = 0; j < N*(N - 1) / 2; j++) {
			internal_cells[j]->DumpHistory();
		}
	}
	std::vector<MSGRBoundaryCell*> boundary_cells;
	std::vector<MSGRInternalCell*> internal_cells;
private:
	int N;
	bool done = false;
};

struct SystolicSystem {
	std::vector < std::vector<float>> MSGR_INPUT;
	std::vector < std::vector<float>> MSGR_OUTPUT;
	std::vector < std::vector<float>> IAM_OUTPUT;
	struct MSGRSystolicArray MSGR;
	struct IAMSystolicArray IAM;
};

// Individual systolic array simulation tests
void RunIAMSimulation(IAMSystolicArray* SA, Matrix<float>* U, Matrix<float>* O, Matrix<bool> flags);
void RunMSGRSimulation(MSGRSystolicArray* SA, Matrix<float>* A, Matrix<float>* I, Matrix<bool>* F);

// Overall systolic system simulation test
void RunSimulation(SystolicSystem* S);

//  Helper function in rotation the input matrix by 90 degress anti-clockwise
Matrix<float>* GetFeedMatrixIAM(Matrix<float>* U, Matrix<float>* O);

#if 0
int IAM_main()
{
	std::vector<std::vector<float>> O= { {2, 2}, {0.5, -0.5} };
	std::vector<std::vector<float>> U = { {8, 10}, {0, 0.5 } };
	std::vector<std::vector<bool>> flags;

	flags.push_back({ false, false, false, true });
	flags.push_back({ false, false, true, false });

	Matrix<float> m_U(U);
	Matrix<float> m_O(O);
	Matrix<bool> m_F(flags);

	int log_dummy;
	int matrix_size = 2;
	struct IAMSystolicArray *IAM_SA = new IAMSystolicArray(matrix_size);
	RunIAMSimulation(IAM_SA, &m_U, &m_O, m_F);
	IAM_SA->Debug();

	std::cin >> log_dummy;
	delete IAM_SA;
	
	return 0;
}
#endif

int MSGR_main() {
	std::vector<std::vector<float>> A = { {2, 3}, {2, 2} };
	std::vector<std::vector<float>> I = { {1, 0}, {0, 1} };
	std::vector<std::vector<bool>> flags;

	flags.push_back({true, false, true, false});
	flags.push_back({false, false, false, false});

	int matrix_size = 2;
	Matrix<float> m_A(A);
	Matrix<float> m_I(I);
	Matrix<bool> m_F(flags);

	struct MSGRSystolicArray *MSGR_SA = new MSGRSystolicArray(matrix_size);
	RunMSGRSimulation(MSGR_SA, &m_A, &m_I, &m_F);
	return 1;
}

int main() {
	// Implement entire systolic system in here...
	return 0;
}

#if 0
void RunIAMSimulation(IAMSystolicArray* SA, Matrix<float>* U, Matrix<float>* O, Matrix<bool> flags) {
	
	float yout_reg;
	float xout_reg;
	float output1_reg;
	float output2_reg;
	std::vector<float> output1;
	std::vector<float> output2;

	// Get the real feed-in matrix
	Matrix<float>* F = GetFeedMatrixIAM(U, O);

	SA->boundary_cells[0]->Execute(&(*F)[0][(*F).cols - 1], flags[0][(*F).cols-1], &yout_reg);

	SA->internal_cells[0]->Execute(&(*F)[1][(*F).cols - 1], &yout_reg, flags[1][(*F).cols - 1], &xout_reg, &output1_reg);
	output1.push_back(output1_reg);
	SA->boundary_cells[0]->Execute(&(*F)[0][(*F).cols - 2], flags[0][(*F).cols - 2], &yout_reg);

	SA->boundary_cells[1]->Execute(&xout_reg, flags[1][(*F).cols - 1], &output2_reg);
	output2.push_back(output2_reg);
	SA->internal_cells[0]->Execute(&(*F)[1][(*F).cols - 2], &yout_reg, flags[1][(*F).cols - 2], &xout_reg, &output1_reg);
	output1.push_back(output1_reg);
	SA->boundary_cells[0]->Execute(&(*F)[0][(*F).cols - 3], flags[0][(*F).cols - 3], &yout_reg);


	SA->boundary_cells[1]->Execute(&xout_reg, flags[1][(*F).cols - 2], &output2_reg);
	output2.push_back(output2_reg);
	SA->internal_cells[0]->Execute(&(*F)[1][(*F).cols - 3], &yout_reg, flags[1][(*F).cols - 3], &xout_reg, &output1_reg);
	output1.push_back(output1_reg);
	SA->boundary_cells[0]->Execute(&(*F)[0][(*F).cols - 4], flags[0][(*F).cols - 4], &yout_reg);

	SA->boundary_cells[1]->Execute(&xout_reg, flags[1][(*F).cols - 3], &output2_reg);
	output2.push_back(output2_reg);
	SA->internal_cells[0]->Execute(&(*F)[1][(*F).cols - 4], &yout_reg, flags[1][(*F).cols - 4], &xout_reg, &output1_reg);
	output1.push_back(output1_reg);

	SA->boundary_cells[1]->Execute(&xout_reg, flags[1][(*F).cols - 4], &output2_reg);
	output2.push_back(output2_reg);


	std::cout << "Output 1 Statistics" << std::endl;
	for (int i = 0; i < output1.size(); i++) {
		std::cout << output1[i] << "\t";
	}

	std::cout << std::endl;
	std::cout << "Output 2 Statistics" << std::endl;
	for (int i = 0; i < output2.size(); i++) {
		std::cout << output2[i] << "\t";
	}
	
	std::cout << std::endl;

	delete F;

	return;
	
}
#endif

void RunMSGRSimulation(MSGRSystolicArray* SA, Matrix<float>* A, Matrix<float>* I, Matrix<bool>* F) {

}

void RunSimulation(SystolicSystem* S) {

}

Matrix<float>* GetFeedMatrixIAM(Matrix<float>* U, Matrix<float>* O) {
	/*
	Helper function that attains the matrix format used in the format paper.
	i.e the individual matrices rotated by 90 degrees and then concatenated
	together into one integrated matrix fed to the systolic array.
	*/

	std::vector<std::vector<float>> tempU, tempO;

	for (int i = 0; i < U->rows; i++) {
		std::vector<float> tempRow;
		for (int j = U->rows-1; j >= 0; j--) {
			tempRow.push_back((*U)[i][j]);
		}
		tempU.push_back(tempRow);
	}

	for (int i = 0; i < O->rows; i++) {
		std::vector<float> tempRow;
		for (int j = O->rows - 1; j >= 0; j--) {
			tempRow.push_back((*O)[i][j]);
		}
		tempO.push_back(tempRow);
	}

	std::vector<std::vector<float>> integratedMatrix;

	for (int i = 0; i < U->rows; i++) {
		std::vector<float> integratedRow;
		for (int j = 0; j < O->cols; j++) {
			integratedRow.push_back(tempO[i][j]);
		}
		for (int j = 0; j < U->cols; j++) {
			integratedRow.push_back(tempU[i][j]);
		}
		integratedMatrix.push_back(integratedRow);
	}

	return new Matrix<float>(integratedMatrix);
}


int IAMSystolicArray::Execute(Matrix<float>* IntegratedMatrix, Matrix<bool> flags) {
	if (done) return -1;
	_ASSERT(IntegratedMatrix->cols == IntegratedMatrix->rows);

	this->boundary_cells[0]->Execute(&(*IntegratedMatrix)[0][(*IntegratedMatrix).cols - 1], flags[0][(*IntegratedMatrix).cols - 1]);
	
	this->internal_cells[0]->Execute(&(*IntegratedMatrix)[1][(*IntegratedMatrix).cols - 1], boundary_cells[0]->get_YOUT(), flags[1][(*IntegratedMatrix).cols - 1]);
	this->boundary_cells[0]->Execute(&(*IntegratedMatrix)[0][(*IntegratedMatrix).cols - 2], flags[0][(*IntegratedMatrix).cols - 2]);

	this->boundary_cells[1]->Execute(internal_cells[0]->get_XOUT(), flags[1][(*IntegratedMatrix).cols - 1]);
	this->internal_cells[0]->Execute(&(*IntegratedMatrix)[1][(*IntegratedMatrix).cols - 2], boundary_cells[0]->get_YOUT(), flags[1][(*IntegratedMatrix).cols - 2]);
	this->boundary_cells[0]->Execute(&(*IntegratedMatrix)[0][(*IntegratedMatrix).cols - 3], flags[0][(*IntegratedMatrix).cols - 3]);

	 
		
	this->boundary_cells[1]->Execute(internal_cells[0]->get_XOUT(), flags[1][(*IntegratedMatrix).cols - 2]);
	this->internal_cells[0]->Execute(&(*IntegratedMatrix)[1][(*IntegratedMatrix).cols - 3], boundary_cells[0]->get_YOUT(), flags[1][(*IntegratedMatrix).cols - 3]);
	this->boundary_cells[0]->Execute(&(*IntegratedMatrix)[0][(*IntegratedMatrix).cols - 4], flags[0][(*IntegratedMatrix).cols - 4]);

	this->boundary_cells[1]->Execute(internal_cells[0]->get_XOUT(), flags[1][(*IntegratedMatrix).cols - 3]);
	this->internal_cells[0]->Execute(&(*IntegratedMatrix)[1][(*IntegratedMatrix).cols - 4], boundary_cells[0]->get_YOUT(), flags[1][(*IntegratedMatrix).cols - 4]);

	this->boundary_cells[1]->Execute(internal_cells[0]->get_XOUT(), flags[1][(*IntegratedMatrix).cols - 4]);

}