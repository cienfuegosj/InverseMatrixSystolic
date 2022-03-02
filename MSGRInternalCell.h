#pragma once
#include <vector>

class MSGRInternalCell
{
public:
	MSGRInternalCell(int id);
	~MSGRInternalCell();
	inline void Reset();
	void Execute(
		const float* yin,
		const float* win,
		const float* xin,
		bool flag,
		float* xout,
		float* yout,
		float* wout);
	void DumpHistory();
private:
	
	float m_C;
	float m_S;
	float m_Z;
	float m_XOUT;
	float m_YOUT;
	float m_WOUT;
	int id;

	std::vector<float> CRegisterHistory;
	std::vector<float> SRegisterHistory;
	std::vector<float> ZRegisterHistory;
	std::vector<float> XOutputHistory;
	std::vector<float> YOutputHistory;
	std::vector<float> WOutputHistory;

	void Log();
};

