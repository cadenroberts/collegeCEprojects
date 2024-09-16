#ifndef MAIN_SAVITCH_SEQUENCE_H
#define MAIN_SAVITCH_SEQUENCE_H
#include <cstdlib>  // Provides size_t
namespace main_savitch_3
{
    class sequence
    {
    public:
        // TYPEDEFS and MEMBER CONSTANTS
        typedef double value_type;
        typedef std::size_t size_type;
        static const size_type CAPACITY = 30;
        // CONSTRUCTOR
        sequence();
        // MODIFICATION MEMBER FUNCTIONS
        void start();
        void advance();
        void insert(const value_type& entry);
        void attach(const value_type& entry);
        void remove_current();
        // CONSTANT MEMBER FUNCTIONS
        size_type size() const;
        bool is_item() const;
        value_type current() const;
    private:
        value_type data[CAPACITY];
        size_type used;
        size_type current_index;
    };
}
#endif
