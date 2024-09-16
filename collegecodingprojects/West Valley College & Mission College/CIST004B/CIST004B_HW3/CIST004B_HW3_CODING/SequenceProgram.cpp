// Caden Roberts CIST004B 2/16/23
#include "sequence1.h"
#include "assert.h"
using namespace main_savitch_3;
sequence::sequence() {
    used = 0;
    current_index = 0;
}
// MODIFICATION MEMBER FUNCTIONS
void sequence::start() {
    current_index = 0;
}
void sequence::advance() {
    current_index += 1;
}
void sequence::insert(const sequence::value_type& entry) {
    assert(size() < CAPACITY);
    for (size_type i = used; i > current_index; --i) data[i] = data[i - 1];
    data[current_index] = entry;
    used++;
}
void sequence::attach(const sequence::value_type& entry) {
    assert(size() < CAPACITY);
    for (size_type i = used; i > current_index + 1; --i) data[i] = data[i - 1];
    if (is_item()) ++current_index;
    data[current_index] = entry;
    used++;
}
void sequence::remove_current() {
    assert(is_item());
    for (size_type i = current_index; i < used; i++) data[i] = data[i + 1];
    used--;
}
// CONSTANT MEMBER FUNCTIONS
sequence::size_type sequence::size() const {
    return used;
}
bool sequence::is_item() const {
    return (current_index < used);
}
sequence::value_type sequence::current() const {
    return data[current_index];
}