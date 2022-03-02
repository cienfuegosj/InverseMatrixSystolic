#include "pch.h"
#include "MSGRInternalCell.h"
#include <iostream>


MSGRInternalCell::MSGRInternalCell(int id) : id(id)
{
}

inline void MSGRInternalCell::Reset() {
	m_C = 0.0;
	m_S = 0.0;
	m_Z = 0.0;
}

void MSGRInternalCell::Execute(const float* yin,
	const float* win,
	const float* xin,
	bool flag,
	float* xout,
	float* yout,
	float* wout) {

	float temp;
	if (flag) {
		*xout = (*xin) + (*win)*(*yin)*(*yin);
		temp = *xout;
		*yout = 0.0;
		if (*xin == 0.0) {
			m_C = 1.0;
			m_Z = 0.0;
			*wout = 1.0;
			if (*yin == 0) {
				m_S = 1.0;
			}
			else {
				m_S = *yin;
			}
		}
		else {
			m_C = (*yin) / (*xin);
			m_S = *yin;
			m_Z = 1.0;
			*wout = (*win)*(*xin) / temp;
		}
	}
	else {
		*xout = (m_Z*(*xin)) + ((*win)*m_S*(*yin));
		*yout = (m_Z*(*yin)) - (m_C*(*xin));
		m_C = m_C;
		m_S = m_S;
		m_Z = m_Z;
		*wout = *wout;
	}
	m_XOUT = *xout;
	m_YOUT = *yout;
	m_WOUT = *wout;
	Log();
}

void MSGRInternalCell::DumpHistory() {
	std::cout << "MSGRInternal Cell Statistics" << std::endl;
	std::cout << "BC." << id << std::endl;
	std::cout << std::endl;


	std::cout << "================ X OUT ==================" << std::endl;
	for (int i = 0; i < XOutputHistory.size(); i++) {
		std::cout << XOutputHistory[i] << "\t";
	}

	std::cout << std::endl;

	std::cout << "================ Y OUT ==================" << std::endl;
	for (int i = 0; i < YOutputHistory.size(); i++) {
		std::cout << YOutputHistory[i] << "\t";
	}

	std::cout << std::endl;

	std::cout << "================ W OUT ==================" << std::endl;
	for (int i = 0; i < WOutputHistory.size(); i++) {
		std::cout << WOutputHistory[i] << "\t";
	}

	std::cout << std::endl;

	std::cout << "================ Register C ==================" << std::endl;
	for (int i = 0; i < CRegisterHistory.size(); i++) {
		std::cout << CRegisterHistory[i] << "\t";
	}

	std::cout << std::endl;

	std::cout << "================ Register S ==================" << std::endl;
	for (int i = 0; i < SRegisterHistory.size(); i++) {
		std::cout << SRegisterHistory[i] << "\t";
	}

	std::cout << std::endl;

	std::cout << "================ Register Z ==================" << std::endl;
	for (int i = 0; i < ZRegisterHistory.size(); i++) {
		std::cout << ZRegisterHistory[i] << "\t";
	}

	std::cout << std::endl;

	std::cout << "End of MSGRInternal Cell Statistics for IC." << id << std::endl << std::endl;
}

void MSGRInternalCell::Log() {
	CRegisterHistory.push_back(m_C);
	SRegisterHistory.push_back(m_S);
	ZRegisterHistory.push_back(m_Z);

	XOutputHistory.push_back(m_XOUT);
	YOutputHistory.push_back(m_YOUT);
	WOutputHistory.push_back(m_WOUT);
}

MSGRInternalCell::~MSGRInternalCell()
{
}
