#pragma once
#include <vector>

class CQuantizer
{
public:
	CQuantizer(void);
	~CQuantizer(void);

	void quantize(const std::vector<float>& vInput, unsigned int vNumInterval, bool vIsUniform, std::vector<float>& vOutput);

private:
	struct Interval
	{
		float Min, Max, Value;
	};

	void __generateUniformInterval(const std::vector<float>& vInput, unsigned int vNumInterval, const std::pair<float, float>& vRange);
	void __generateNonuniformInterval(const std::vector<float>& vInput, unsigned int vNumInterval, const std::pair<float, float>& vRange);
	void __quantizeData(const std::vector<float>& vInput, std::vector<float>& vOutput);

	std::pair<float, float> CQuantizer::__computeRange(const std::vector<float>& vData);

	std::vector<Interval> m_QuantizeIntervals;
};
