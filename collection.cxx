#include <iostream>
#include <limits>
#include <set>

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
