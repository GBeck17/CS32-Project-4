// Dictionary.cpp

// This is a correct but horribly inefficient implementation of Dictionary
// functionality.  Your job is to change DictionaryImpl to a more efficient
// correct implementation.  You can change it any way you like, subject to
// restrictions in the spec (e.g., that the only C++ library container you
// are allowed to use are vector, list, stack, and queue (and string); if you
// want anything fancier, implement it yourself).

#include "Dictionary.h"
#include <string>
#include <list>
#include <cctype>
#include <algorithm>
#include <utility>  // for swap
using namespace std;

const int MaxPrimes = 26; 
const int hashSize = 50000; 

void removeNonLetters(string& s);
void generateNextPermutation(string& permutation);

  // This class does the real work of the implementation.

struct Node {
	Node(string myValue, int hashKey, unsigned long unique_key)
		: value(myValue), next(nullptr), key(hashKey), uniqueKey(unique_key) {}
	string value; 
	Node* next; 
	int key; 
	unsigned long uniqueKey; // Stores a unique identifier for the Node, which will connect anagrams so they can be identified easier
};

class DictionaryImpl {
  public:
	  DictionaryImpl();
	  ~DictionaryImpl();
    void insert(string word);
    void lookup(string letters, void callback(string));
  private:
	Node * hashTable[hashSize]; // Array of pointers that is the hash table 
	int hashFunction(string& word); // function that will return hash key, maps input to spot in hashTable
	unsigned long unique_value; // Holds the prime additive value that is used for uniqueKey in Node
};

DictionaryImpl::DictionaryImpl() {
	for (int i = 0; i < hashSize; i++)
		hashTable[i] = nullptr; 
}

DictionaryImpl::~DictionaryImpl() {
	for (int i = 0; i < hashSize; i++) {
		Node* iterator = hashTable[i]; 
		while (iterator != nullptr) {
			Node* deleter = iterator; 
			iterator = iterator->next; 
			delete deleter; 
		}
	}
}
void DictionaryImpl::insert(string word)
{
	int hashKey = hashFunction(word); 
	Node*& location = hashTable[hashKey]; // Set location in hash table for new value, does this create pointer to a point
	if (location == nullptr) 
		location = new Node(word, hashKey, unique_value);
	else {
		Node* temp = location;
		location = new Node(word, hashKey, unique_value);
		location->next = temp; 
	}
}

void DictionaryImpl::lookup(string letters, void callback(string)) 
{
    if (callback == nullptr)
        return;

    removeNonLetters(letters);
    if (letters.empty())
        return;
	int hashKey = hashFunction(letters); 
	Node* finder = hashTable[hashKey]; // Initialize iterator at start of linked list 
	while (finder != nullptr) {
		if (finder->uniqueKey == unique_value) // Check if unique key matches unique_value from impl class, this signifies anagram 
			callback(finder->value); 
		finder = finder->next; 
	}
}

int DictionaryImpl::hashFunction(string& word)
{
	unique_value = 1;
	int primeNums[MaxPrimes] = { 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101 };
	removeNonLetters(word); 
	for (int i = 0; i < word.length(); i++) {
		unique_value *= primeNums[word[i] - 'a']; // Get unique value by adding prime nums mapped to each char, subtracting from 'a' to get 0-25
	}
	int key = unique_value % hashSize; // Get key, which is unique value modulus the size of hash table 
	return key; 

}
    
void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop everything off from "to" to end.
} 

  // Each successive call to this function generates the next permutation of the
  // characters in the parameter.  For example,
  //    string s = "eel";
  //    generateNextPermutation(s);  // now s == "ele"
  //    generateNextPermutation(s);  // now s == "lee"
  //    generateNextPermutation(s);  // now s == "eel"
  // You don't have to understand this function's implementation.
void generateNextPermutation(string& permutation)
{
    string::iterator last = permutation.end() - 1;
    string::iterator p;

    for (p = last; p != permutation.begin()  &&  *p <= *(p-1); p--)
        ;
    if (p != permutation.begin())
    {
        string::iterator q;
        for (q = p+1; q <= last  &&  *q > *(p-1); q++)
            ;
        swap(*(p-1), *(q-1));
    }
    for ( ; p < last; p++, last--)
        swap(*p, *last);
}

//******************** Dictionary functions ********************************

// These functions simply delegate to DictionaryImpl's functions.
// You probably don't want to change any of this code

Dictionary::Dictionary()
{
    m_impl = new DictionaryImpl;
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters,callback);
}
