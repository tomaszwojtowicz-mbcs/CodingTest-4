/***************************************************************************
* © 2019 Cadence Design Systems, Inc.  All rights reserved.                *
* This work may not be copied, modified, re-published, uploaded, executed, *
* or distributed in any way, in any medium, whether in whole or in part,   *
* without prior written permission from Cadence Design Systems, Inc.       *
****************************************************************************/

#include <iostream>
#include <limits>
#include <set>
#include "test.h"

// Store a collection of integers
// * On contruction the collection should be empty.
// * The same entry may be stored more than once so that {1, 5, 5, 9} is a collection of 4 entries
class IntegerCollection {
public:
    // Insert one entry with the value x
    void Insert(int x);

    // Erase one entry with the value x, if one exists
    void Erase(int x);

    // Erase all entries, x, from <= x < to
    void Erase(int from, int to);

    // Return the count of all entries, x, from <= x < to
    size_t Count(int from, int to) const;

private:
    std::multiset<int> container;
};

void IntegerCollection::Insert(int x) {
    container.insert(x);
}

void IntegerCollection::Erase(int x) {
    auto it = container.find(x);
    if (container.end() != it) {
        container.erase(it);
    }
}

void IntegerCollection::Erase(int from, int to) {
    container.erase(container.lower_bound(from), container.lower_bound(to));
}

size_t IntegerCollection::Count(int from, int to) const {
    return std::distance(container.lower_bound(from), container.lower_bound(to));
}


int main(int, char **) {
    {
        TEST("empty on construction");
        IntegerCollection empty;
        ASSERT_EQ(empty.Count(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()), size_t(0));
    }

    {
        TEST("collection contains 5 ones");
        IntegerCollection fiveOnes;
        for (int v : {1, 1, 1, 1, 1}) {
            fiveOnes.Insert(v);
        }
        ASSERT_EQ(fiveOnes.Count(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()), size_t(5));
        ASSERT_EQ(fiveOnes.Count(1,std::numeric_limits<int>::max()), size_t(5));
        ASSERT_EQ(fiveOnes.Count(std::numeric_limits<int>::min(), 2), size_t(5));
        ASSERT_EQ(fiveOnes.Count(1,2), size_t(5));
    }

    {
        TEST("collection contains 3 twos");
        IntegerCollection threeTwos;
        for (int v : {-3, 2, 7, 2, 1, 2}) {
            threeTwos.Insert(v);
        }
        ASSERT_EQ(threeTwos.Count(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()), size_t(6));
        ASSERT_EQ(threeTwos.Count(2,3), size_t(3));
    }

    {
        TEST("collection contains 2 fives");
        IntegerCollection twoFives;
        for (int v : {5, 2, 5, 5, 3, 2}) {
            twoFives.Insert(v);
        }
        for (int v : {1, 5, 3, 1}) {
            twoFives.Erase(v);
        }
        ASSERT_EQ(twoFives.Count(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()), size_t(4));
        ASSERT_EQ(twoFives.Count(5,6), size_t(2));
    }

    {
        TEST("range erase");
        IntegerCollection noTeens;
        for (int v : {-1, 5, 11, 14, 14, 19, 20, 21, 30}) {
            noTeens.Insert(v);
        }
        ASSERT_EQ(noTeens.Count(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()), size_t(9));
        noTeens.Erase(13,20);
        ASSERT_EQ(noTeens.Count(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()), size_t(6));
    }

    // Added this test to ensure that a left-closed range is being erased correctly
    {
        TEST("left-closed range erase");
        IntegerCollection noTeens;
        for (int v : {-1, 5, 11, 13, 14, 14, 19, 20, 21, 30}) {
            noTeens.Insert(v);
        }
        ASSERT_EQ(noTeens.Count(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()), size_t(10));
        noTeens.Erase(13,20);
        ASSERT_EQ(noTeens.Count(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()), size_t(6));
    }

    {
        TEST("range count");
        IntegerCollection bagOfNumbers;
        for (int v : {-1, 5, 11, 14, 14, 19, 20, 21, 30}) {
            bagOfNumbers.Insert(v);
        }
        ASSERT_EQ(bagOfNumbers.Count(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()), size_t(9));
        ASSERT_EQ(bagOfNumbers.Count(-10,-1), size_t(0));
        ASSERT_EQ(bagOfNumbers.Count(-10,0), size_t(1));
        ASSERT_EQ(bagOfNumbers.Count(10,11), size_t(0));
        ASSERT_EQ(bagOfNumbers.Count(11,11), size_t(0));
        ASSERT_EQ(bagOfNumbers.Count(11,12), size_t(1));
        ASSERT_EQ(bagOfNumbers.Count(11,14), size_t(1));
        ASSERT_EQ(bagOfNumbers.Count(11,15), size_t(3));
        ASSERT_EQ(bagOfNumbers.Count(14,15), size_t(2));
        ASSERT_EQ(bagOfNumbers.Count(14,100), size_t(6));
    }

    {
        TEST("Insert/count/erase significant quantity of random data");
        IntegerCollection bagOfNumbers;
        std::vector<int> numbers;
        const size_t size = 500000;
        numbers.reserve(size);
        for (size_t ii = 0 ; ii < size; ++ii) {
            numbers.push_back(ii);
        }
        RandomizeVectorOrder(numbers);
        for (int i : numbers) {
            bagOfNumbers.Insert(i);
        }
        ASSERT_EQ(bagOfNumbers.Count(0, size), size);
        int total = 0;
        for (int i : numbers) {
            total += bagOfNumbers.Count(i, i+1);
        }
        ASSERT_EQ(total, size);
        for (int i : numbers) {
            bagOfNumbers.Erase(i);
        }
        ASSERT_EQ(bagOfNumbers.Count(0, size), size_t(0));
    }

    return 0;
}


