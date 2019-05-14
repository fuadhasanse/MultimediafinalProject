#pragma once
#include <vector>

class CKMeans
{
public:
	CKMeans(void) : m_StopThreshold(0.02f) {}
	~CKMeans(void) {}

	void executeKMeans(const std::vector<std::pair<float, float>>& vInput, unsigned int vNumCluster, std::vector<std::pair<float, float>>& voCluterCenter);

private:
	void __initClusterCenter(const std::vector<std::pair<float, float>>& vInput, unsigned int vNumCluster, std::vector<std::pair<float, float>>& voCluterCenter);
	void __updateClusterIndex(const std::vector<std::pair<float, float>>& vInput, const std::vector<std::pair<float, float>>& vClusterCenter, std::vector<int>& vClusterIndex);
	void __updateClusterCenter(const std::vector<std::pair<float, float>>& vInput, const std::vector<int>& vClusterIndex, std::vector<std::pair<float, float>>& vClusterCenter);

	float __computeAvgError(const std::vector<std::pair<float, float>>& vInput, const std::vector<std::pair<float, float>>& vClusterCenter, const std::vector<int>& vClusterIndex);
	float __computeDis(const std::pair<float, float>& vLeft, const std::pair<float, float>& vRight);

	int __findClosestClusterCenter(const std::pair<float, float>& vData, const std::vector<std::pair<float, float>> vClusterCenter);

	const float m_StopThreshold;
};



