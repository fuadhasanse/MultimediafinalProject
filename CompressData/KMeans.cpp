#include "KMeans.h"

//***********************************************************************
//FUNCTION: 
std::pair<float, float> operator + (const std::pair<float, float>& vLeft, const std::pair<float, float>& vRight)
{
	return std::make_pair(vLeft.first + vRight.first, vLeft.second + vRight.second);
}


//***********************************************************************
//FUNCTION: 
std::pair<float, float> operator / (const std::pair<float, float>& vLeft, float vRight)
{
	return std::make_pair(vLeft.first / vRight, vLeft.second / vRight);
}

//*****************************************************************
//FUNCTION:
void CKMeans::executeKMeans(const std::vector<std::pair<float, float>>& vInput, unsigned int vNumCluster, std::vector<std::pair<float, float>>& voCluterCenter)
{
	voCluterCenter.clear();
	__initClusterCenter(vInput, vNumCluster, voCluterCenter);

	std::vector<int> ClusterIndex;
	ClusterIndex.resize(vInput.size());
	float LastAvgError = FLT_MAX;
	float CurrentAvgError = 0;
	int NumIterations = 0;
	while (NumIterations < 100)
	{
		__updateClusterIndex(vInput, voCluterCenter, ClusterIndex);
		__updateClusterCenter(vInput, ClusterIndex, voCluterCenter);
		CurrentAvgError = __computeAvgError(vInput, voCluterCenter, ClusterIndex);
		if (LastAvgError - CurrentAvgError < m_StopThreshold) break;
		LastAvgError = CurrentAvgError;
		NumIterations++;
	}
	_ASSERT(voCluterCenter.size() == vNumCluster);
}

//******************************************************************
//FUNCTION:
void CKMeans::__updateClusterCenter(const std::vector<std::pair<float, float>>& vInput, const std::vector<int>& vClusterIndex, std::vector<std::pair<float, float>>& vClusterCenter)
{
	std::vector<int> NumDataInCluster;
	std::vector<std::pair<float, float>> Sum;

	for (int i = 0; i < vClusterCenter.size(); i++)
	{
		NumDataInCluster.push_back(0);
		Sum.push_back(std::make_pair(0, 0));
	}

	for (int i = 0; i < vInput.size(); i++)
	{
		NumDataInCluster[vClusterIndex[i]]++;
		Sum[vClusterIndex[i]] = Sum[vClusterIndex[i]] + vInput[i];
	}

	for (int i = 0; i < vClusterCenter.size(); i++)
	{
		vClusterCenter[i] = Sum[i] / NumDataInCluster[i];
	}
}

//******************************************************************
//FUNCTION:
void CKMeans::__initClusterCenter(const std::vector<std::pair<float, float>>& vInput, unsigned int vNumCluster, std::vector<std::pair<float, float>>& voCluterCenter)
{
	for (unsigned int i = 0; i < vNumCluster; i++)
	{
		voCluterCenter.push_back(vInput[i]);
	}
}

//******************************************************************
//FUNCTION:
void CKMeans::__updateClusterIndex(const std::vector<std::pair<float, float>>& vInput, const std::vector<std::pair<float, float>>& vClusterCenter, std::vector<int>& vClusterIndex)
{
	for (int i = 0; i < vInput.size(); i++)
	{
		vClusterIndex[i] = __findClosestClusterCenter(vInput[i], vClusterCenter);
	}
}

//******************************************************************
//FUNCTION:
float CKMeans::__computeAvgError(const std::vector<std::pair<float, float>>& vInput, const std::vector<std::pair<float, float>>& vClusterCenter, const std::vector<int>& vClusterIndex)
{
	float Error = 0;

	for (int i = 0; i < vInput.size(); i++)
	{
		Error += __computeDis(vInput[i], vClusterCenter[vClusterIndex[i]]);
	}

	return Error / vInput.size();
}

//******************************************************************
//FUNCTION:
float CKMeans::__computeDis(const std::pair<float, float>& vLeft, const std::pair<float, float>& vRight)
{
	return sqrt((vLeft.first - vRight.first) * (vLeft.first - vRight.first) + (vLeft.second - vRight.second) * (vLeft.second - vRight.second));
}

//******************************************************************
//FUNCTION:
int CKMeans::__findClosestClusterCenter(const std::pair<float, float>& vData, const std::vector<std::pair<float, float>> vClusterCenter)
{
	float MinDis = FLT_MAX;
	int ClusterIndex = -1;
	for (int k = 0; k < vClusterCenter.size(); k++)
	{
		float d = __computeDis(vData, vClusterCenter[k]);
		if (d < MinDis)
		{
			MinDis = d;
			ClusterIndex = k;
		}
	}
	_ASSERT(ClusterIndex != -1);
	return ClusterIndex;
}


