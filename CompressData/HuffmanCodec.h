#pragma once
#include <vector>
#include <queue>

class CHuffmanCodec
{
public:
	CHuffmanCodec(void)  {}
	~CHuffmanCodec(void) {}

	void encode(const std::vector<std::pair<float, float>>& vInput, std::vector<bool>& voOutput);

private:
	struct HuffmanTreeNode
	{
		int NodeIndex, Left, Right, Parent;
	};

	struct QueueElement
	{
		int Frequency;
		int NodeIndex;

		friend bool operator<(const QueueElement& vLeft, const QueueElement& vRight) {return vLeft.Frequency > vRight.Frequency;}
	};

	struct CodeTableElement
	{
		std::pair<float, float> Data;
		int Frequency;
		std::vector<bool> Code;
	};

	std::vector<HuffmanTreeNode>  m_HuffmanTree;
	std::vector<CodeTableElement> m_CodeTable;

	void __countFrequency(const std::pair<float, float>& vData, std::vector<CodeTableElement>& voCodeTable);
	void __generateFrequencyTable(const std::vector<std::pair<float, float>>& vInput, std::vector<CodeTableElement>& voCodeTable);
	void __generateHuffmanCode(std::vector<CodeTableElement>& voCodeTable);
	void __generateHuffmanTree(std::priority_queue<QueueElement>& vPriorityQueue);
	void __initPriorityQueue(const std::vector<CodeTableElement>& vFrequencyTable, std::priority_queue<QueueElement>& voQueue);
	void __generateEncodedBitStream(const std::vector<std::pair<float, float>>& vInput, const std::vector<CodeTableElement>& vCodeTable, std::vector<bool>& voOutput);
};
