#include "Message.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>

int maxT = 0;

void filterMessages();
bool filter( const Message& );

int main()
{	
	filterMessages();

	return 0;
}

void filterMessages() {
	struct stat file_statistics;
	const char* input_file_path = SOURCE_DIR "/tests/2_4/2.4_example.in";
	const char* output_file_path = SOURCE_DIR "/tests/2_4/2.4_output.out";
	
	std::ifstream input( input_file_path, std::ios::binary );
	std::ofstream output( output_file_path, std::ios::binary );

	if ( !input.is_open() || !output.is_open() ) {
		std::cerr << "Failed to open input/output file\n";
		exit(1);
	}
	
	stat( input_file_path, &file_statistics );
	int remaining_bytes = file_statistics.st_size;
	Message new_message;

	while ( remaining_bytes != 0 ) {
		input >> new_message;

		if ( !filter( new_message ) ) {
			output << new_message;
			output.write("\0", sizeof(char) );
		}

		remaining_bytes -= 3 * sizeof( unsigned ) + new_message.getLength();
	}
}

bool filter( const Message& message ) {	
	if ( message.getTime() <= maxT - 2 ) {
		return true;
	} 
	else {
		maxT = ( maxT < message.getTime() ) ? message.getTime() : maxT;
		return false;
	}
}