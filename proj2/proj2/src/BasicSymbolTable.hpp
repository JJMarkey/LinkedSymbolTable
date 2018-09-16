
/***************************************************************
<John Markey>
<BasicSymbolTable.hpp>
<Project 2>
<Implements all the methods defined in SymbolTable.hpp>
***************************************************************/
#ifndef BASIC_SYMBOL_TABLE_H
#define BASIC_SYMBOL_TABLE_H

#include <algorithm>
#include <deque>

#include "SymbolTable.hpp"

template <typename Key, typename Value>
class BasicSymbolTable : public SymbolTable<Key, Value>
{
protected:
	struct NodePair
	{
		Key _key;
		Value _value;

		NodePair(const Key& key = Key{}, const Value& value = Value{}) : _key{ key }, _value{ value } {}
	};

	// The container of the <key, value> pairs
	std::deque<NodePair> _pairs;

	// Key value comparison (less than)
	bool keyLessThan(const NodePair& lhs, const NodePair& rhs) const { return lhs._key < rhs._key; }
	bool keyLessThan(const Key& lhs, const Key& rhs) const { return lhs < rhs; }

	// Equality of key values
	bool keyEquals(const NodePair& lhs, const NodePair& rhs) const { return lhs._key == rhs._key; }
	bool keyEquals(const Key& lhs, const Key& rhs) const { return lhs == rhs; }

	// Equality of key values
	bool keyLessThanOrEquals(const NodePair& lhs, const NodePair& rhs) const
	{
		return keyEquals(lhs, rhs) || keyLessThan(lhs, rhs);
	}
	bool keyLessThanOrEquals(const Key& lhs, const Key& rhs) const
	{
		return keyEquals(lhs, rhs) || keyLessThan(lhs, rhs);
	}

public:

	//DESTRUCTOR
	virtual ~BasicSymbolTable()
	{
		clear();
	}

	//puts key-value pair into table
	void put(const Key& key, const Value& val = Value{})
	{
		if(empty())
			_pairs.push_back(NodePair(key, val));

		else 
		{
			_pairs.push_back(NodePair(key, val));
			//sorts deque upon entering using insertion sort
			for (int i = 0; i < size(); i++)
			{
				int j = i;

				while (j > 0 && keyLessThan(_pairs.at(j)._key, _pairs.at(j - 1)._key))
				{
					NodePair temp = _pairs[j];
					_pairs[j] = _pairs[j - 1];
					_pairs[j - 1] = temp;
					j--;
				}
			}
		}

	}

	// acquire the value paired with key
	bool get(const Key& key, Value& val = Value{}) const
	{
		if (empty())
		{
			return false;
		}
		else
		{
			//runs through all elements in deque
			for (int i = 0; i < size(); i++)
			{
				//checks if key is equal to a key in the deque
				if (keyEquals(key, _pairs.at(i)))
				{
					val = _pairs.at(i)._value;
					return true;
				}
			}
			return false;
		}
	}

	//remove key(and its value) from table
	void remove(const Key& key)
	{		
		//runs through every element of the deque to search for the key and its value from the table
		for (int i = 0; i < size(); i++)
		{
			//if found, erases key and value
			if (keyEquals(key, _pairs.at(i)._key))
			{
				_pairs.erase(_pairs.begin() + i);
				break;
			}
		}
	}

	//Is there a value paired with key?
	bool contains(const Key& key) const
	{
		if (empty())
		{
			return false;
		}
		else
		{
			//runs through the pairs deque
			for (unsigned i = 0; i < _pairs.size(); i++)
			{
				//if key equals current key, return true
				if (keyEquals(key, _pairs[i]))
					return true;
			}
			return false;
		}
	}

	// Is the table empty?
	bool empty() const
	{
		return _pairs.empty();
	}

	//Number of key-value pairs.
	int size() const
	{
		return _pairs.size();
	}

	// Smallest key
	bool min(Key& key = Key{}) const
	{
		if (empty())
			return false;
		key = _pairs.front()._key;
		return true;
	}

	// Largest key
	bool max(Key& key = Key{}) const
	{
		if (empty())
			return false;
		key = _pairs.at(size() -1)._key;
		return true;
	}

	// Largest key less than or equal to key
	bool floor(const Key& key, Key& floorKey) const
	{
		if (empty())
			return false;
		if (keyLessThan(key, _pairs[0]._key))
			return false;
		else
		{
			//does a search for largest key lower or equal to input key
			for (int i = size() - 1; i >= 0; i--)
			{
				if (keyLessThanOrEquals(_pairs[i]._key, key))
				{
					floorKey = _pairs[i]._key;
					break;
				}
			}
			return true;
		}
	}

	// Smallest key greater than or equal to key
	bool ceiling(const Key& key, Key& ceilingKey) const
	{
		if (empty())
			return false;
		if (keyLessThan(_pairs[size()-1]._key, key))
			return false;
		else
		{
			//does a search for smallest key greater than or equal to key
			for (int i = 0; i < size(); i++)
			{
				if (keyLessThanOrEquals(key, _pairs[i]._key))
				{
					ceilingKey = _pairs[i]._key;
					break;
				}
			}
			return true;
		}
	}

	// Number of keys less than key
	int rank(const Key& key) const
	{
		int keyCount = 0;

		//runs through all elements in _pairs deque, if any keys are less than key 
		//keyCount increments
		for (int i = 0; i < _pairs.size(); i++)
		{
			if (keyLessThan(_pairs.at(i)._key, key))
				keyCount++;
		}
		return keyCount;
	}

	// key of rank k
	bool select(int k = 0, Key& key = Key{}) const
	{
		for (int i = 0; i < size(); i++)
		{
			if (rank(_pairs[i]._key) == k)
			{
				key = _pairs[i]._key;
				return true;
			}
		}
		return false;
	}

	// Delete the smallest key
	bool deleteMin()
	{
		if (empty())
			return false;
		_pairs.pop_front();
		return true;
	}

	// Delete the largest key
	bool deleteMax()
	{
		if (empty())
			return false;
		_pairs.pop_back();
		return true;
	}

	// number of keys in [low, high] (including low, high)
	int size(const Key& low, const Key& high) const
	{
		int keyCount = 0;

		//runs through entirety of deque
		for (int i = 0; i < size(); i++)
		{
			//if low <= current and current <= high, increment keyCount
			if (keyLessThanOrEquals(low, _pairs[i]) && keyLessThanOrEquals(_pairs[i], high))
			{
				keyCount++;
			}
		}
		return keyCount;
	}
	
	// keys in [low, high] (including low, high), in sorted order
	virtual std::vector<Key> keys(const Key& low, const Key& high) const
	{
		std::vector<Key> keySet;
		int i = 0;
		while (i < size())
		{
			//if key <= current and current <= high, push back current key
			if (keyLessThanOrEquals(low, _pairs[i]) && keyLessThanOrEquals(_pairs[i], high))
			{
				keySet.push_back(_pairs[i]._key);
			}
			i++;
		}
		return keySet;
	}

	// all keys in the table, in sorted order
	virtual std::vector<Key> keys() const
	{
		std::vector<Key> keySet;
		if (empty())
		{
			return keySet;
		}
		else
		{
			keySet = keys(_pairs.at(0)._key, _pairs.at(size() -1)._key);
			return keySet;
		}
		
	}

	// Removes all elements from the table
	virtual void clear()
	{
		_pairs.clear();
	}

    //
    ///////////////////////////////////////////////////////////////////////////////
    // Check integrity of the vector data structure.
    ///////////////////////////////////////////////////////////////////////////////
    //
    bool check() const
    {
        bool ordered = isOrdered();
        bool rankConsistent = isRankConsistent();

        if (!ordered)        std::cout << "Not in symmetric order" << std::endl;
        if (!rankConsistent) std::cout << "Ranks inconsistent" << std::endl;

        return ordered && rankConsistent;
    }

  private:
    //
    // does this container satisfy symmetric order?
    //
    bool isOrdered() const
    {
        if (size() <= 1) return true;

        for (unsigned index = 0; index < _pairs.size() - 1; index++)
        {
            if (keyLessThan(_pairs[index + 1], _pairs[index])) return false;
        }
        return true;
    }

    // check that ranks are consistent
    bool isRankConsistent() const
    {
        // The i th node should be rank i
        for (int i = 0; i < size(); i++)
        {
            Key key;
            select(i, key);
            if (i != rank(key)) return false;
        }

        // All keys must equate to the key acquired at its rank 
        for (Key key : keys())
        {
            Key acquired;
            select(rank(key), acquired);

            if (!keyEquals(key, acquired)) return false;
        }

        return true;
    }
};

#endif