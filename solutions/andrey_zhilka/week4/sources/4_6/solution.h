#ifndef _TASK4_6_SOLUTION_H_
#define _TASK4_6_SOLUTION_H_

#include <vector>
#include <string>
#include <map>
#include <stack>
#include <utility>

namespace task4_6
{
	typedef std::vector< std::string > lines;

	class solution
	{
	private :
		enum Result
		{
			ERROR_NON_DEFINED,
			ERROR_REDEFINITION,
			ERROR_INVALID_EXP,
			ERROR_ZERO_DIVISION,
			STATUS_OK
		};

		mutable std::map< const std::string, int > variables_;

		
		int evaluate( const char, int, int );
		std::string error_message( Result, const std::string& );
		Result calculate( std::string );
		void trim( std::string& );
		std::pair< Result, int > poliz_( const std::string& );	
		bool is_operator( const char );
		void next_operation( std::stack< int >&, std::stack< char >& );

	public:
		explicit solution( const lines& calulator_lines );
		int result_for( const std::string& key ) const;
	};
}

#endif // _TASK4_5_SOLUTION_H_