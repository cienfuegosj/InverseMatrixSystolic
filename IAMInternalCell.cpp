#include "pch.h"
#include "IAMInternalCell.h"
#include <iostream>


IAMInternalCell::IAMInternalCell(int id) : id(id)
{
	m_R = 0.0;
	m_XOUT = 0.0;
	m_YOUT = 0.0;
}


IAMInternalCell::~IAMInternalCell()
{
}

void IAMInternalCell::Reset() {
	m_R = 0.0;
}

const float* IAMInternalCell::get_XOUT() { return &m_XOUT; }
const float* IAMInternalCell::get_YOUT() { return &m_YOUT; }

void IAMInternalCell::Execute(const float* xin, const float* yin, const bool flag) {
	if (flag) {
		m_YOUT = 0.0;
		m_XOUT = *xin;
		m_R = *yin / *xin;
	}
	else {
		m_YOUT = *yin - m_R*(*xin);
		m_XOUT = *xin;
		m_R = m_R;
	}
	Log();
}

void IAMInternalCell::DumpHistory() {
	std::cout << "IAMInternal Cell Statistics" << std::endl;
	std::cout << "IC." << id << std::endl;
	std::cout << std::endl;

	std::cout << "================ Y OUT ==================" << std::endl;
	for (int i = 0; i < YOutputHistory.size(); i++) {
		std::cout << YOutputHistory[i] << "\t";
	}

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

	std::cout << "End of IAMInternal Cell Statistics for IC." << id << std::endl << std::endl;
}

void IAMInternalCell::Log() {
	XOutputHistory.push_back(m_XOUT);
	YOutputHistory.push_back(m_YOUT);
	RegisterHistory.push_back(m_R);
}
