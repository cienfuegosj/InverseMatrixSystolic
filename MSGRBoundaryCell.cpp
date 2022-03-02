#include "pch.h"
#include "MSGRBoundaryCell.h"
#include <iostream>

MSGRBoundaryCell::MSGRBoundaryCell(int id) : id(id) {
	m_R = 0.0;
	m_XOUT = 0.0;
}

void MSGRBoundaryCell::Reset() {
	m_R = 0.0;
}

void MSGRBoundaryCell::Execute(const float* win, const float* yin, bool flag, float* xout) {
	if (flag) {
		*xout = (*yin)*(*yin)*(*win);
		if ((*yin) == 0.0) {
			m_R = 1.0;
		}
		else {
			m_R = *yin;
		}
	}
	else {
		*xout = m_R * (*yin)*(*win);
		m_R = m_R;
	}
	m_XOUT = *xout;
	Log();
}

void MSGRBoundaryCell::DumpHistory() {
	std::cout << "MSGRBoundary Cell Statistics" << std::endl;
	std::cout << "BC." << id << std::endl;
	std::cout << std::endl;


	std::cout << "================ X OUT ==================" << std::endl;
	for (int i = 0; i < XOutputHistory.size(); i++) {
		std::cout << XOutputHistory[i] << "\t";
	}

	std::cout << std::endl;

	std::cout << "================ Register ==================" << std::endl;
	for (int i = 0; i < RegisterHistory.size(); i++) {
		std::cout << RegisterHistory[i] << "\t";
	}

	std::cout << std::endl;

	std::cout << "End of MSGRBoundary Cell Statistics for IC." << id << std::endl << std::endl;
}

void MSGRBoundaryCell::Log() {
	XOutputHistory.push_back(m_XOUT);
	RegisterHistory.push_back(m_R);
}


MSGRBoundaryCell::~MSGRBoundaryCell()
{
}
