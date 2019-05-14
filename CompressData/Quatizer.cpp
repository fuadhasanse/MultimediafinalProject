#include "Quatizer.h"
#include <algorithm>

CQuantizer::CQuantizer(void)
{
}

CQuantizer::~CQuantizer(void)
{
}

//******************************************************************
//FUNCTION:
void CQuantizer::quantize(const std::vector<float>& vInput, unsigned int vNumInterval, bool vIsUniform, std::vector<float>& vOutput)
{
	_ASSERT(!vInput.empty());

	std::pair<float, float> Range = __computeRange(vInput);

	if (vIsUniform)
		__generateUniformInterval(vInput, vNumInterval, Range);
	else
		__generateNonuniformInterval(vInput, vNumInterval, Range);

	__quantizeData(vInput, vOutput);
}

//******************************************************************
//FUNCTION:
void CQuantizer::__quantizeData(const std::vector<float>& vInput, std::vector<float>& vOutput)
{
	vOutput.clear();
	for (unsigned int i=0; i<vInput.size(); i++)
	{
		int IntervalIndex;
		for (IntervalIndex=0; IntervalIndex<m_QuantizeIntervals.size(); IntervalIndex++)
		{
			if ((vInput[i] >= m_QuantizeIntervals[IntervalIndex].Min) && (vInput[i] < m_QuantizeIntervals[IntervalIndex].Max)) break;
		}
		_ASSERT(IntervalIndex != m_QuantizeIntervals.size());
		vOutput.push_back(m_QuantizeIntervals[IntervalIndex].Value);
	}
}

//******************************************************************
//FUNCTION:
void CQuantizer::__generateUniformInterval(const std::vector<float>& vInput, unsigned int vNumInterval, const std::pair<float, float>& vRange)
{
	m_QuantizeIntervals.clear();

	float StepSize = (vRange.second - vRange.first) / vNumInterval;

	float t = vRange.first;
	Interval m;
	for (unsigned int i=0; i<vNumInterval; i++)
	{
		m.Min = t;
		m.Max = t + StepSize;
		m.Value = (m.Min + m.Max) / 2;
		t = m.Max;
		m_QuantizeIntervals.push_back(m);
	}
	m_QuantizeIntervals[m_QuantizeIntervals.size()-1].Max = vRange.second;
}

//******************************************************************
//FUNCTION:
void CQuantizer::__generateNonuniformInterval(const std::vector<float>& vInput, unsigned int vNumInterval, const std::pair<float, float>& vRange)
{
	m_QuantizeIntervals.clear();

	std::vector<float> SortedData = vInput;
	int NumDataInInterval = vInput.size() / vNumInterval;

	std::sort(SortedData.begin(), SortedData.end());
	Interval m;
	float t = vRange.first;
	for (unsigned int i=0; i<vNumInterval-1; i++)
	{
		m.Min = t;
		m.Max = SortedData[(i+1)*NumDataInInterval];
		
		float Average = 0;
		for (unsigned int k=i*NumDataInInterval; k<(i+1)*NumDataInInterval; k++)
		{
			Average += SortedData[k];
		}
		m.Value = Average / NumDataInInterval;
		m_QuantizeIntervals.push_back(m);

		t = m.Max;
	}

	m.Min = t;
	m.Max = vRange.second;
	float Average = 0;
	int Counter = 0;
	for (unsigned int k=(vNumInterval-1)*NumDataInInterval; k<vInput.size(); k++)
	{
		Average += SortedData[k];
		Counter++;
	}
	m.Value = Average / Counter;
	m_QuantizeIntervals.push_back(m);
}

//******************************************************************
//FUNCTION:
std::pair<float, float> CQuantizer::__computeRange(const std::vector<float>& vData)
{
	float Max = -FLT_MAX;
	float Min = FLT_MAX;

	for (int i=0; i<vData.size(); i++)
	{
		if (vData[i] > Max) Max = vData[i];
		if (vData[i] < Min) Min = vData[i];
	}
	return std::make_pair(Min, Max+(Max-Min)/100000);
}
