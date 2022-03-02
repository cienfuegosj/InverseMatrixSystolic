#pragma once
#include <vector>

class MSGRBoundaryCell
{
public:
	MSGRBoundaryCell(int id);
	~MSGRBoundaryCell();
	void Reset();
	void Execute(const float* win, const float* yin, bool flag, float* xout);
	void DumpHistory();
private:
	float m_R;
	float m_XOUT;
	int id;
	std::vector<float> RegisterHistory;
	std::vector<float> XOutputHistory;

	void Log();
};

