#include "pch.h"
#include "IAMBoundaryCell.h"
#include <iostream>


IAMBoundaryCell::IAMBoundaryCell(int id) : id(id)
{
	m_R = 0.0;
	m_YOUT = 0.0;
}


IAMBoundaryCell::~IAMBoundaryCell()
{
}

const float* IAMBoundaryCell::get_YOUT() { return &m_YOUT; }

void IAMBoundaryCell::Reset() { m_R = 0.0; }

void IAMBoundaryCell::Execute(const float* xin, bool flag) {
	if (flag) {
		m_R = float(1) / *xin;
		m_YOUT = 0.0;
	}
	else {
		m_R = m_R;
		m_YOUT = (m_R)*(*xin);
	}
	Log();
}

void IAMBoundaryCell::DumpHistory() {
	std::cout << "IAMBoundary Cell Statistics" << std::endl;
	std::cout << "BC." << id << std::endl;
	std::cout << std::endl;

	std::cout << "================ Y OUT ==================" << std::endl;
	for (int i = 0; i < YOutputHistory.size(); i++) {
		std::cout << YOutputHistory[i] << "\t";
	}

	std::cout << std::endl;

	std::cout << "================ Register ==================" << std::endl;
	for (int i = 0; i < RegisterHistory.size(); i++) {
		std::cout << RegisterHistory[i] << "\t";
	}

	std::cout << std::endl;

	std::cout << "End of IAMBoundary Cell Statistics for BC." << id << std::endl << std::endl;
}

void IAMBoundaryCell::Log() {
	YOutputHistory.push_back(m_YOUT);
	RegisterHistory.push_back(m_R);
}
