#pragma once
#include <vector>

class IAMInternalCell
{
public:
	IAMInternalCell(int id);
	~IAMInternalCell();
	void Execute(const float* xin, const float* yin, const bool flag);
	void Reset();
	void DumpHistory();
    const float* get_XOUT();
	const float* get_YOUT();
private:
	float m_R;
	float m_XOUT;
	float m_YOUT;
	int id;
	std::vector<float> RegisterHistory;
	std::vector<float> XOutputHistory;
	std::vector<float> YOutputHistory;
	void Log();
};