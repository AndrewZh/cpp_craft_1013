#include "solution.h"
#include <sstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

task4_6::solution::solution( const lines& calculator_lines )
{
	for (size_t i = 0; i < calculator_lines.size(); i++ )
	{
		std::string working_copy = calculator_lines[i];
		trim( working_copy );
		Result result = calculate ( working_copy );

		if ( result != STATUS_OK )
		{
			throw new std::logic_error
		}
	}
}

int task4_6::solution::result_for( const std::string& key ) const
{
	return variables_[key];
}

task4_6::solution::Result task4_6::solution::calculate( std::string exp )
{
	std::vector< std::string > var_exp;
	boost::split( var_exp, exp, boost::is_any_of("=") );
	
	if ( var_exp.size() != 2 )
	{
		return ERROR_INVALID_EXP;
	}

	if ( variables_.find( var_exp[0] ) != variables_.end() )
	{
		
	}

	
	std::pair< Result, int > result = poliz_( var_exp[1] );

	if ( result.first == STATUS_OK )
	{
		variables_[ var_exp[0] ] = result.second;
	}

	return result.first;
}

std::pair< task4_6::solution::Result, int > task4_6::solution::poliz_( const std::string& exp )
{
	size_t cur_index = 0;
	std::stack< int > operands;
	std::stack< char > operators;

	boost::regex upper_str( "[A-Z]+" );
	boost::regex numeric( "[0-9]+" );
	boost::smatch found_matches;

	std::vector< std::string > str_operands;
	
	boost::split( str_operands, exp, boost::is_any_of( "+-*/()" ) );

	for ( size_t i = 0; i < str_operands.size(); i++ )
	{
		if ( !boost::regex_match( str_operands[i], found_matches, upper_str) 
			&& !boost::regex_match( str_operands[i], found_matches, numeric) )
		{
			return std::make_pair( ERROR_INVALID_EXP, 0 );
		}

		if ( variables_.find( str_operands[i] ) != variables_.end() )
		{
			operands.push( variables_[ str_operands[i] ] );
		}
		else 
		{
			try
			{
				int operand = boost::lexical_cast< int > (str_operands[i] );
				operands.push( operand );
			}
			catch ( boost::bad_lexical_cast x )
			{
				return std::make_pair( ERROR_NON_DEFINED, 0 );
			}
		}

		cur_index += str_operands[i].length();

		if ( exp.at( cur_index ) != ')' )
		{
			operators.push( exp.at(cur_index) );
			cur_index++;
			if ( is_operator ( exp.at( cur_index ) ) )
			{
				return std::make_pair( ERROR_INVALID_EXP, 0 );
			}
		}
		else 
		{
			while ( !operators.empty() || operators.top() != '(' )
			{
				try
				{
					next_operation( operands, operators );
				}
				catch ( std::logic_error e )
				{
					return std::make_pair( ERROR_ZERO_DIVISION, 0 );
				}
			}
			if ( operators.empty() )
			{
				return std::make_pair( ERROR_INVALID_EXP, 0 );
			}
			else 
			{
				operators.pop(); // remove '('
			}
		}
	}

	while ( !operators.empty() )
	{
		try
		{
			next_operation( operands, operators );
		}
		catch ( std::logic_error e )
		{
			return std::make_pair( ERROR_ZERO_DIVISION, 0 );
		}
	}

	if ( operands.size() != 1 )
	{
		return std::make_pair( ERROR_INVALID_EXP, 0 );
	}
	else 
	{
		return std::make_pair( STATUS_OK, operands.top() );
	}
	
}

void task4_6::solution::next_operation( std::stack< int >& operands, std::stack< char >& operators )
{
	char operation = operators.top();
	operators.pop();
	int operand2 = operands.top();
	operands.pop();
	int operand1 = operands.top();
	operands.pop();
	operands.push( evaluate( operation, operand1, operand2 ) );
}

bool task4_6::solution::is_operator( const char c )
{
	return ( c == '+' || c == '-' || c == '*' || c == '/');
}

int task4_6::solution::evaluate( const char operation, int operand1, int operand2 )
{
	int result;

	switch( operation )
	{
	case '*':
		result = operand1 * operand2;
	case '/':
		{
			if ( operand2 == 0 )
			{
				throw new std::logic_error("");
			}
			double res = double(operand1) / operand2;
			result = (int) res;
			result += (res - result > 0.5) ? 1 : 0;
		}
	case '+':
		result = operand1 + operand2;
	case '-':
		result = operand1 - operand2;
	}	

	return result;
}

std::string task4_6::solution::error_message(task4_6::solution::Result error_code,
											 const std::string& var_name = "")
{
	std::stringstream message;

	switch( error_code )
	{
	case ERROR_INVALID_EXP :
		message << "not correct expression at line";
		break;
	case ERROR_NON_DEFINED :
		message << "'" << var_name << "' variable not defined at line ";
		break;
	case ERROR_REDEFINITION :
		message << "such variable '" << var_name << "' already exists";
		break;
	case ERROR_ZERO_DIVISION :
		message << "zero div ";
		break;
	default :
		message << "undefined exception at line ";
	}

	return message.str();
}

void task4_6::solution::trim( std::string& source )
{
	source.erase( std::remove( source.begin(), source.end(), ' '), source.end() );
}