// CS 532, Seth Ortiz, Hw3 - Huffman Implementation, 2/21/2019
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <queue>

using namespace std;

struct node
{
	char data;
	int freq;
	node *left, *right;
};

struct comp
{
	bool operator() (node * l, node * r)
	{
		return l->freq > r->freq; // highest priority item has lowest freq 
	}
};

void getFreq(unsigned long long freq[256], const string & fileName);
void huffman(string code[256], const unsigned long long freq[256]);
void littleFile(const string code[256], const string & littleFileName, const string & inputFileName);
void bigFile(const string & bigFileName, const string & littleFileName, const unsigned long long freq[256]);
node * newNode(char newData, int newFreq, node * newLeft, node * newRight);
void traverseHuffman(node * root, string code[256]);
node * getHuffmanRoot(const unsigned long long freq[256]);
bool die(const string & msg);

int main()
{

	string inputFile, littleOutputFile, bigOutputFile, code[256];
	unsigned long long freq[256];

	cout << "Enter input file name: ";
	getline(cin, inputFile);

	getFreq(freq,inputFile);

	cout << "Enter little output file name: ";
	getline(cin, littleOutputFile);

	cout << "Enter big output file name: ";
	getline(cin, bigOutputFile);

	for (int i = 0; i < 256; i++)
		if (freq[i] != 0)
			cout << "Freq(" << i << ") = " << freq[i] << endl;
	
	huffman(code, freq);
	for (int i = 0; i < 256; i++)
		if (!code[i].empty())
			cout << i << " : " << code[i] << endl;
	
	littleFile(code, littleOutputFile, inputFile);
	bigFile(bigOutputFile, littleOutputFile, freq);

	return 0;
}

void getFreq(unsigned long long freq[256], const string & fileName)
{
	ifstream fin;
	int x;

	fin.open(fileName, ios::binary);

	for (int i = 0; i < 256; i++)
		freq[i] = 0;

	do {
		x = fin.get(); // get returns ascii value
		if (x == -1) break; // get returns -1 when end of file
		freq[x]++;
	} while (true);

	fin.seekg(0); // rest file pointer to 0
	fin.close();
}

void huffman(string code[256], const unsigned long long freq[256])
{
	priority_queue<node*, vector<node*>, comp> nodes; // creates priority queue: makes lowest frequence highest priority

	for (int i = 0; i < 256; i++)
		if (freq[i])
			nodes.push(newNode(i, freq[i], nullptr, nullptr));

	// BUILDS huffman tree
	while (nodes.size() != 1)
	{
		node *tempLeft = nodes.top(); // top: returns refrence of highest priority (lowest freq)
		nodes.pop();				  // pop: removes highest priority element from queue

		node *tempRight = nodes.top();
		nodes.pop();

		nodes.push(newNode(NULL, tempLeft->freq + tempRight->freq, tempLeft, tempRight)); // pushes new node, which is the sum of lowest freq, with refence to those two lowest freq
	}
	traverseHuffman(nodes.top(), code);
}

void littleFile(const string code[256], const string & littleFileName, const string & inputFileName)
{
	ifstream inputFile;
	ofstream littleFile;
	inputFile.open(inputFileName, ios::binary);
	littleFile.open(littleFileName, ios::binary);

	int x = inputFile.get();
	while (x != EOF)
	{
		littleFile.write((char*)&code[x][0], code[x].size());
		x = inputFile.get();
	}
	inputFile.close();
	littleFile.close();
}
void bigFile(const string & bigFileName, const string & littleFileName, const unsigned long long freq[256])
{
	ofstream bigFile;
	ifstream lilFile;

	bigFile.open("bFile.txt", ios::binary);
	lilFile.open("lFile.txt", ios::binary);


	node * root = getHuffmanRoot(freq);
	node * trav = root;
	char charCode;

	do
	{
		charCode = lilFile.get();
		if (lilFile.eof())
			break;
		if (charCode == '0')
		{
			trav = trav->left;
		}
		else
		{
			trav = trav->right;
		}
		if (trav->data != NULL)
		{
			bigFile.write(&trav->data, sizeof(char));
			trav = root;
		}

	} while (true);

	bigFile.close();
	lilFile.close();
}

node * newNode(char newData, int newFreq, node * newLeft, node * newRight)
{
	node * newNode = new node();

	newNode->data = newData;
	newNode->freq = newFreq;
	newNode->left = newLeft;
	newNode->right = newRight;

	return newNode; // returns created node's address
}

void traverseHuffman(node * root, string code[256]) // code holds ascii character's huffman code
{
	static string s;
	if (!root) // if root is null
		return;
	if (root->data != NULL)
		code[root->data] = s;

	s += '0';
	traverseHuffman(root->left, code);

	s[s.size() - 1] = '1';
	traverseHuffman(root->right, code);
	s.pop_back();
}

bool die(const string & msg)
{
	cout << "Fatal error: " << msg << endl;
	exit(EXIT_FAILURE);
}

node * getHuffmanRoot(const unsigned long long freq[256])
{
	priority_queue<node*, vector<node*>, comp> nodes; // creates priority queue: makes lowest frequence highest priority

	for (int i = 0; i < 256; i++)
		if (freq[i])
			nodes.push(newNode(i, freq[i], nullptr, nullptr));

	// BUILDS huffman tree
	while (nodes.size() != 1)
	{
		node *tempLeft = nodes.top(); // top: returns refrence of highest priority (lowest freq)
		nodes.pop();				  // pop: removes highest priority element from queue

		node *tempRight = nodes.top();
		nodes.pop();

		nodes.push(newNode(NULL, tempLeft->freq + tempRight->freq, tempLeft, tempRight)); // pushes new node, which is the sum of lowest freq, with refence to those two lowest freq
	}
	
	return nodes.top();
}
