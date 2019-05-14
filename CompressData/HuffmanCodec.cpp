#include "HuffmanCodec.h"


//******************************************************************
//FUNCTION:
void CHuffmanCodec::encode(const std::vector<std::pair<float, float>>& vInput, std::vector<bool>& voOutput)
{
	_ASSERT(!vInput.empty());

	__generateFrequencyTable(vInput, m_CodeTable);

	m_HuffmanTree.clear();
	std::priority_queue<QueueElement> PriorityQueue;

	__initPriorityQueue(m_CodeTable, PriorityQueue);
	__generateHuffmanTree(PriorityQueue);

	__generateHuffmanCode(m_CodeTable);
	__generateEncodedBitStream(vInput, m_CodeTable, voOutput);
}

//******************************************************************
//FUNCTION:
void CHuffmanCodec::__generateHuffmanCode(std::vector<CodeTableElement>& voCodeTable)
{
	for (unsigned int i = 0; i < voCodeTable.size(); i++)
	{
		std::vector<bool> ReverseCode;

		HuffmanTreeNode Node = m_HuffmanTree[i];
		HuffmanTreeNode ParentNode;
		while (Node.Parent != -1)
		{
			ParentNode = m_HuffmanTree[Node.Parent];
			_ASSERT((ParentNode.Left == Node.NodeIndex) || (ParentNode.Right == Node.NodeIndex));
			ReverseCode.push_back((ParentNode.Left == Node.NodeIndex) ? true : false);
			Node = ParentNode;
		}

		for (int k = ReverseCode.size() - 1; k >= 0; k--)
		{
			voCodeTable[i].Code.push_back(ReverseCode[k]);
		}
		_ASSERT(voCodeTable[i].Code.size() == ReverseCode.size());
	}
}

//******************************************************************
//FUNCTION:
void CHuffmanCodec::__generateEncodedBitStream(const std::vector<std::pair<float, float>>& vInput, const std::vector<CodeTableElement>& vCodeTable, std::vector<bool>& voOutput)
{
	voOutput.clear();

	for (unsigned int i = 0; i < vInput.size(); i++)
	{
		std::pair<float, float> t = vInput[i];
		for (unsigned int k = 0; k < vCodeTable.size(); k++)
		{
			if (t == vCodeTable[k].Data)
			{
				voOutput.insert(voOutput.end(), vCodeTable[k].Code.begin(), vCodeTable[k].Code.end());
				break;
			}
		}
	}
}

//******************************************************************
//FUNCTION:
void CHuffmanCodec::__generateHuffmanTree(std::priority_queue<QueueElement>& vPriorityQueue)
{
	HuffmanTreeNode Node;
	QueueElement t, Left, Right;
	while (vPriorityQueue.size() >= 2)
	{
		Left = vPriorityQueue.top();
		vPriorityQueue.pop();
		Right = vPriorityQueue.top();
		vPriorityQueue.pop();

		t.Frequency = Left.Frequency + Right.Frequency;
		t.NodeIndex = m_HuffmanTree.size();
		vPriorityQueue.push(t);

		Node.Left = Left.NodeIndex;
		Node.Right = Right.NodeIndex;
		Node.NodeIndex = t.NodeIndex;
		Node.Parent = -1;
		m_HuffmanTree.push_back(Node);

		m_HuffmanTree[Left.NodeIndex].Parent = Node.NodeIndex;
		m_HuffmanTree[Right.NodeIndex].Parent = Node.NodeIndex;
	}
}

//******************************************************************
//FUNCTION:
void CHuffmanCodec::__initPriorityQueue(const std::vector<CodeTableElement>& vFrequencyTable, std::priority_queue<QueueElement>& voQueue)
{
	_ASSERT(!vFrequencyTable.empty());
	QueueElement t;
	HuffmanTreeNode Node;
	for (unsigned int i = 0; i < vFrequencyTable.size(); i++)
	{
		t.Frequency = vFrequencyTable[i].Frequency;
		t.NodeIndex = i;
		voQueue.push(t);

		Node.Left = Node.Right = Node.Parent = -1;
		Node.NodeIndex = i;
		m_HuffmanTree.push_back(Node);
	}
}

//******************************************************************
//FUNCTION:
void CHuffmanCodec::__generateFrequencyTable(const std::vector<std::pair<float, float>>& vInput, std::vector<CodeTableElement>& voCodeTable)
{
	voCodeTable.clear();
	for (unsigned int i = 0; i < vInput.size(); i++)
	{
		__countFrequency(vInput[i], voCodeTable);
	}
}

//******************************************************************
//FUNCTION:
void CHuffmanCodec::__countFrequency(const std::pair<float, float>& vData, std::vector<CodeTableElement>& voCodeTable)
{
	for (unsigned int i = 0; i < voCodeTable.size(); i++)
	{
		if (vData == voCodeTable[i].Data)
		{
			voCodeTable[i].Frequency++;
			return;
		}
	}

	CodeTableElement t;
	t.Data = vData;
	t.Frequency = 1;
	voCodeTable.push_back(t);
}
