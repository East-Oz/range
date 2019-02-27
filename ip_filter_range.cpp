#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 
#include <fstream>
#include <sstream>
#include <chrono>
#include <range/v3/all.hpp>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]

#define	SAVE_IN_FILE	1 

typedef std::vector<unsigned char> IPT;
typedef std::vector<IPT> IP_STORAGE;

std::vector<std::string> split( const std::string &str, char d )
{
	std::vector<std::string> r;

	std::string::size_type start = 0;
	std::string::size_type stop = str.find_first_of( d );
	while( stop != std::string::npos )
	{
		r.push_back( str.substr( start, stop - start ) );
		start = stop + 1;
		stop = str.find_first_of( d, start );
	}
	r.push_back( str.substr( start ) );
	return r;
}

unsigned char string_to_char( std::string val )
{
	unsigned char out_value = 0;
	const char *start = val.c_str();
	char c;
	int n = 0;
	size_t size = val.size();
	size_t count = 0;
	while( count < size )
	{
		c = *start;
		++start;
		if( c >= '0' && c <= '9' )
		{
			n *= 10;
			n += c - '0';
		}
		++count;
	}
	out_value = n;
	return out_value;
}

std::vector<unsigned char> get_vector_ip( std::string str, char d )
{
	std::vector<unsigned char> out;

	std::string::size_type start = 0;
	std::string::size_type stop = str.find_first_of( d );
	while( stop != std::string::npos )
	{
		out.push_back( string_to_char( str.substr( start, stop - start ) ) );
		start = stop + 1;
		stop = str.find_first_of( d, start );
	}
	out.push_back( string_to_char( str.substr( start ) ) );

	return out;
}

bool find_elements_function( const std::vector<unsigned char>& val )
{
	try
	{
		if( val.at( 0 ) == 1 )
			return true;
		else if( (val.at( 0 ) == 46)
			&& (val.at( 1 ) == 70) )
			return true;
		else if( (val.at( 0 ) == 46)
			|| (val.at( 1 ) == 46)
			|| (val.at( 2 ) == 46)
			|| (val.at( 3 ) == 46)
			)
			return true;
		else
			return false;
	}
	catch( ... ) { return false; }
}

bool find_elem_cond3( const std::vector<unsigned char>& val )
{
	try
	{
		if( ( val.at( 0 ) == 46 )
			|| ( val.at( 1 )  == 46 )
			|| ( val.at( 2 )  == 46 )
			|| ( val.at( 3 ) == 46 )
			)
			return true;
		else
			return false;
	}
	catch( ... ) { return false; }
}

template <typename T, typename U, typename K>
bool compare( T& _src, U count, K val )
{
	if( count > 3 ) { return false; }
	if( _src.at( count ) != val )
	{
		return false;
	}
	else
	{
		return true;
	}
}

template<typename T, typename U, typename K, typename ...Args>
bool compare( T& _src, U count, K val, Args ... args )
{
	bool bResult = false;
	if( count > 3 ) { return false; }
	if( _src.at( count ) != val )
	{
		return false;
	}
	else
	{
		bResult = true;
		++count;
		bResult &= compare( _src, count, args... );
	}
	return bResult;
}

template<typename T, typename ...Args>
auto copy_collection( T& _src, T& _dst, Args ... args )
{
	bool bResult = false;
	IP_STORAGE::iterator dst_it = _dst.begin();
	for( IP_STORAGE::iterator it = _src.begin(); it != _src.end(); ++it )
	{
		size_t count = 0;
		bResult = compare( *it, count, args... );

		if( bResult )
		{
			*dst_it++ = *it;
		}
	}
	return dst_it;
}

int main( int, char** )
{
	using ip_addr_vec = std::vector<std::vector<unsigned char>>;

	try
	{
		ip_addr_vec ip_pool;
		ip_addr_vec ip_pool_out;

		for( std::string line; std::getline( std::cin, line );)
		{
			std::vector<std::string> v = split( line, '\t' );
			ip_pool.push_back( get_vector_ip( v.at( 0 ), '.' ) );
		}

		// reverse lexicographically sort 
		std::sort( ip_pool.begin(), ip_pool.end(), std::greater<>() );

		// copy sorted vector to output vector
		ip_pool_out = ip_pool;

		// Create temp vector for all conditions
		ip_addr_vec ip_temp( ip_pool.size() );

		auto it = std::copy_if( ip_pool.begin(), ip_pool.end(), ip_temp.begin(), find_elements_function );
		ip_temp.resize( std::distance( ip_temp.begin(), it ) );

		// 1 condition first byte == 1
		ip_addr_vec ip_cond1( ip_temp.size() );


		auto it1 = copy_collection( ip_temp, ip_cond1, 1 );
		ip_cond1.resize( std::distance( ip_cond1.begin(), it1 ) );
		ip_pool_out.reserve( ip_pool_out.size() + ip_cond1.size() );
		ip_pool_out.insert( ip_pool_out.end(), ip_cond1.begin(), ip_cond1.end() );

		// 2 condition 46.70.*.*
		ip_addr_vec ip_cond2( ip_temp.size() );
		auto it2 = copy_collection( ip_temp, ip_cond2, 46, 70 );
		ip_cond2.resize( std::distance( ip_cond2.begin(), it2 ) );
		ip_pool_out.reserve( ip_pool_out.size() + ip_cond2.size() );
		ip_pool_out.insert( ip_pool_out.end(), ip_cond2.begin(), ip_cond2.end() );

		// 3 condition  any byte = 46
		ip_addr_vec ip_cond3( ip_temp.size() );
		auto it3 = std::copy_if( ip_temp.begin(), ip_temp.end(), ip_cond3.begin(), find_elem_cond3 );
		ip_cond3.resize( std::distance( ip_cond3.begin(), it3 ) );
		ip_pool_out.reserve( ip_pool_out.size() + ip_cond3.size() );
		ip_pool_out.insert( ip_pool_out.end(), ip_cond3.begin(), ip_cond3.end() );

#ifdef SAVE_IN_FILE
		std::ofstream file( "ip_out.txt" );
		std::string s;
#endif

		for( ip_addr_vec::const_iterator ip = ip_pool_out.cbegin(); ip != ip_pool_out.cend(); ++ip )
		{
#ifdef SAVE_IN_FILE
			s = "";
#endif
			for( std::vector<unsigned char>::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part )
			{
				if( ip_part != ip->cbegin() )
				{
					std::cout << ".";
#ifdef SAVE_IN_FILE
					s.append( "." );
#endif
				}
				std::cout << std::to_string( *ip_part );
#ifdef SAVE_IN_FILE
				s.append( std::to_string( *ip_part ) );
#endif
			}
			std::cout << std::endl;
#ifdef SAVE_IN_FILE
			file << s;
			file << std::endl;
#endif
		}
#ifdef SAVE_IN_FILE
		file.close();
#endif


		// 222.173.235.246
		// 222.130.177.64
		// 222.82.198.61
		// ...
		// 1.70.44.170
		// 1.29.168.152
		// 1.1.234.8

		// TODO filter by first byte and output
		// ip = filter(1)

		// 1.231.69.33
		// 1.87.203.225
		// 1.70.44.170
		// 1.29.168.152
		// 1.1.234.8

		// TODO filter by first and second bytes and output
		// ip = filter(46, 70)

		// 46.70.225.39
		// 46.70.147.26
		// 46.70.113.73
		// 46.70.29.76

		// TODO filter by any byte and output
		// ip = filter_any(46)

		// 186.204.34.46
		// 186.46.222.194
		// 185.46.87.231
		// 185.46.86.132
		// 185.46.86.131
		// 185.46.86.131
		// 185.46.86.22
		// 185.46.85.204
		// 185.46.85.78
		// 68.46.218.208
		// 46.251.197.23
		// 46.223.254.56
		// 46.223.254.56
		// 46.182.19.219
		// 46.161.63.66
		// 46.161.61.51
		// 46.161.60.92
		// 46.161.60.35
		// 46.161.58.202
		// 46.161.56.241
		// 46.161.56.203
		// 46.161.56.174
		// 46.161.56.106
		// 46.161.56.106
		// 46.101.163.119
		// 46.101.127.145
		// 46.70.225.39
		// 46.70.147.26
		// 46.70.113.73
		// 46.70.29.76
		// 46.55.46.98
		// 46.49.43.85
		// 39.46.86.85
		// 5.189.203.46
	}
	catch( const std::exception &e )
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}