#pragma once
#include <vector>

class IAMBoundaryCell
{
public:
	IAMBoundaryCell(int id);
	~IAMBoundaryCell();
	void Reset();
	void Execute(const float* xin, bool flag);
	void DumpHistory();
	const float* get_YOUT();
private:
	float m_R;
	float m_YOUT;
	int id;
	std::vector<float> RegisterHistory;
	std::vector<float> YOutputHistory;
	
	void Log();
};

