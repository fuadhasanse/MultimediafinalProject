#include <stdlib.h>
#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include "HuffmanCodec.h"
#include "KMeans.h"

#define CODEBOOK_SIZE 8
#define PI 3.14

const std::string InputFileName = "NonUniformData.txt";

void readData(std::vector<std::pair<float, float>>& voData);
void representDataByClusterCenter(const std::vector<std::pair<float, float>>& vInputData, const std::vector<std::pair<float, float>>& vClusterCenter, std::vector<std::pair<float, float>>& voOutput);

float computeDis(const std::pair<float, float>& vLeft, const std::pair<float, float>& vRight);

void compressDataByKMeans(const std::vector<std::pair<float, float>>& vInputData, std::vector<bool>& vEncodedBitStream);

int main()
{
	std::vector<std::pair<float, float>> InputData;
	std::vector<bool> BitStream4Kmeans;

	readData(InputData);

	compressDataByKMeans(InputData, BitStream4Kmeans);

	return 0;
}

//******************************************************************
//FUNCTION:
void representDataByClusterCenter(const std::vector<std::pair<float, float>>& vInputData, const std::vector<std::pair<float, float>>& vClusterCenter, std::vector<std::pair<float, float>>& voOutput)
{
	_ASSERT(!vInputData.empty() && !vClusterCenter.empty());
	voOutput.clear();

	for (unsigned int i=0; i<vInputData.size(); i++)
	{
		int ClusterIndex = -1;
		float MinDis = FLT_MAX;
		for (unsigned int k=0; k<vClusterCenter.size(); k++)
		{
			float d = computeDis(vInputData[i], vClusterCenter[k]);
			if (d < MinDis)
			{
				MinDis = d;
				ClusterIndex = k;
			}
		}
		_ASSERT(ClusterIndex != -1);
		voOutput.push_back(vClusterCenter[ClusterIndex]);
	}
}

//******************************************************************
//FUNCTION:
void compressDataByKMeans(const std::vector<std::pair<float, float>>& vInputData, std::vector<bool>& vEncodedBitStream)
{
	std::vector<std::pair<float, float>> ClusterCenter, NewData;
	CKMeans KMeansCluster;
	CHuffmanCodec HuffmanCodec;

	KMeansCluster.executeKMeans(vInputData, CODEBOOK_SIZE, ClusterCenter);
	representDataByClusterCenter(vInputData, ClusterCenter, NewData);
	HuffmanCodec.encode(NewData, vEncodedBitStream);
}

//******************************************************************
//FUNCTION:
float computeDis(const std::pair<float, float>& vLeft, const std::pair<float, float>& vRight)
{
	return sqrt((vLeft.first - vRight.first) * (vLeft.first - vRight.first) + (vLeft.second- vRight.second) * (vLeft.second- vRight.second));
}

//******************************************************************
//FUNCTION:
void readData(std::vector<std::pair<float, float>>& voData)
{
	FILE *fp = fopen(InputFileName.c_str(), "r");
	
	int NumData;
	fscanf(fp, "%d\n", &NumData);
	for (int i=0; i<NumData; i++)
	{
		float Weight, Height;
		fscanf(fp, "%f %f\n", &Weight, &Height);
		voData.push_back(std::make_pair(Weight, Height));
	}
	fclose(fp);
}
