#ifndef MAIN_SAVITCH_SEQUENCE_H
#define MAIN_SAVITCH_SEQUENCE_H
#include <cstdlib>  // Provides size_t
using namespace std;
namespace main_savitch_4
{
	class sequence
	{
	public:
		// TYPEDEFS and MEMBER CONSTANTS
		typedef double value_type;

		static const size_t DEFAULT_CAPACITY = 30;
		// CONSTRUCTORS and DESTRUCTOR
		sequence(size_t initial_capacity = DEFAULT_CAPACITY);
		sequence(const sequence& source);
		~sequence();
		// MODIFICATION MEMBER FUNCTIONS
		void resize(size_t new_capacity);
		void start();
		void advance();
		void insert(const value_type& entry);
		void attach(const value_type& entry);
		void remove_current();
		void operator =(const sequence& source);
		// CONSTANT MEMBER FUNCTIONS
		size_t size() const;
		size_t max_size() const;
		bool is_item() const;
		value_type current() const;
	private:
		value_type* data;
		size_t used;
		size_t current_index;
		size_t capacity;
	};
}

#endif
