// FILE: table2.tpp.h
// TEMPLATE CLASS IMPLEMENTED: Table (see table2.h for documentation)
// INVARIANT for the Table ADT:
// 1. The number of records in the Table is in the member variable
// total_records.
// 2. The actual records of the table are stored in a chained hash table.
// An entry with key i is stored on the linked list with the head
// pointer at data[hash(i)].

#include <assert.h> // Provides assert

template <class RecordType>
size_t Table<RecordType>::hash_function(int key) const
{
    return (key % TABLE_SIZE);
}

template <class RecordType>
Table<RecordType>::Table()
{
    total_records = 0;
    for (size_t i = 0; i < TABLE_SIZE; i++) hash_table[i] = nullptr;
}

template <class RecordType>
Table<RecordType>::~Table()
{
    total_records = 0;
    for (size_t i = 0; i < TABLE_SIZE; i++) {
        Node<RecordType>* current = hash_table[i];
        while (current != nullptr) {
            Node<RecordType>* temp = current;
            current = current->link;
            delete temp;
        }
    }
}

template <class RecordType>
Table<RecordType>::Table(const Table<RecordType>& source)
{
    for (size_t i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = nullptr;
        Node<RecordType>* cursor = source.hash_table[i];
        Node<RecordType>* precursor = nullptr;
        while (cursor != nullptr) {
            Node<RecordType>* new_node = new Node<RecordType>{cursor->data, cursor->link};
            if (precursor == nullptr) hash_table[i] = new_node;
            else precursor->link=new_node;
            precursor = new_node;
            cursor = cursor->link;
        }
    }
    total_records = source.total_records;
}

template <class RecordType>
void Table<RecordType>::insert(const RecordType& entry)
{
    assert(entry.key >= 0);
    if (find_node(entry.key) == nullptr) {
        hash_table[hash_function(entry.key)] = new Node<RecordType>{entry, hash_table[hash_function(entry.key)]};
        total_records++;
    } else find_node(entry.key)->data=entry;
}

template<class RecordType>
void Table<RecordType>::remove(int key)
{
    assert(key >= 0);
    Node<RecordType>* cursor = hash_table[hash_function(key)], * precursor = nullptr;
    while (cursor != nullptr && cursor->data.key != key)
    {
        precursor = cursor;
        cursor = cursor->link;
    }
    if (cursor == nullptr) return;
    if (precursor == nullptr) hash_table[hash_function(key)] = cursor->link;
    else precursor->link = cursor->link;
    delete cursor;
    total_records--;
}

template <class RecordType>
void Table<RecordType>::operator =(const Table<RecordType>& source)
{
    if (this != &source) {
        for (size_t i = 0; i < TABLE_SIZE; i++) {
            while (hash_table[i] != nullptr) {
                Node<RecordType>* head = hash_table[i];
                hash_table[i] = hash_table[i]->link;
                delete head;
            }
            Node<RecordType>* cursor = source.hash_table[i], * precursor = nullptr;
            while (cursor != nullptr) {
                precursor = new Node<RecordType>{cursor->data, hash_table[i]};
                hash_table[i] = precursor;
                cursor = cursor->link;
            }
        }
        total_records = source.total_records;
    }
}

template <class RecordType>
bool Table<RecordType>::is_present(int key) const
{
    return (find_node(key)!=nullptr);
}

template <class RecordType>
Node<RecordType>* Table<RecordType>::find_node(int key) const
{
    assert(key >= 0);
    for (Node<RecordType>* cursor = hash_table[hash_function(key)]; ; cursor = cursor->link) if (cursor==nullptr||(cursor->data.key==key)) return cursor ;
    
}

template <class RecordType>
void Table<RecordType>::find(int key, bool& found, RecordType& result) const
{
    found = is_present(key);
    if (found) result = find_node(key)->data;
    
}



