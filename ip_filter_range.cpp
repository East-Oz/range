#include <iostream>
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

void show_ip_vector( IP_STORAGE& ip_pool_out )
{
	ranges::for_each( ip_pool_out, []( std::vector<unsigned char> ip_part )
	{
		size_t i = 0;
		size_t ip_size = ip_part.size();
		ranges::for_each( ip_part, [ & ]( unsigned char c )
		{
			std::cout << std::to_string( c );
			if( i < (ip_size - 1) )
				std::cout << '.';
			++i;
		} );
		std::cout << std::endl;
	} );
}


void show_ip_from_vector( const IPT& val )
{
	size_t i = 0;
	size_t ip_size = val.size();

	for(auto _v: val )
	{
		std::cout << std::to_string( _v );
		if( i < (ip_size - 1) )
			std::cout << '.';
		++i;
	}
	std::cout << std::endl;
}

int main( int, char** )
{
	using ip_addr_vec = std::vector<std::vector<unsigned char>>;

	try
	{
		ip_addr_vec ip_pool;

		for( std::string line; std::getline( std::cin, line );)
		{
			std::vector<std::string> v = split( line, '\t' );
			ip_pool.push_back( get_vector_ip( v.at( 0 ), '.' ) );
		}

		// reverse lexicographically sort 
		ranges::sort( ip_pool, std::greater<>() );


		// show
		show_ip_vector( ip_pool );

		// show first condition elements
		auto first_confition = ip_pool | ranges::view::filter( [&]( const IPT& val )
		{ 
			try
			{
				if( val.at( 0 ) == 1 )	return true;
				else return false;
			}
			catch( ... ) { return false; }
		} );
		for( const auto i : first_confition )
		{
			show_ip_from_vector( i );
		}

		// show second condition elements
		auto second_confition = ip_pool | ranges::view::filter( [ & ]( const IPT& val )
		{
			try
			{
				if( (val.at( 0 ) == 46)
					&& (val.at( 1 ) == 70) )
					return true;
				else
					return false;
			}
			catch( ... ) { return false; }
		} );
		for( const auto i : second_confition )
		{
			show_ip_from_vector( i );
		}


		// show third condition elemants
		auto third_confition = ip_pool | ranges::view::filter( [ & ]( const IPT& val )
		{
			try
			{
				if( ( val.at( 0 ) == 46 )
					|| ( val.at( 1 ) == 46 )
					|| ( val.at( 2 ) == 46 )
					|| ( val.at( 3 ) == 46 )
					)
					return true;
				else
					return false;
			}
			catch( ... ) { return false; }
		} );
		for( const auto i : third_confition )
		{
			show_ip_from_vector( i );
		}



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
