#ifndef BOX_H
#define BOX_H

#include <string>
#include <vector>
#include <utility>

class Box
{
private:
  std::vector<std::string> upper_bounds;
  std::vector<std::string> lower_bounds;

public:
  typedef std::vector<std::string>::size_type size_type;

  Box( std::vector<std::string> input_lower_bounds, std::vector<std::string> input_upper_bounds );
  Box( std::vector< std::pair<std::string,std::string> > input_bounds );
  Box( std::string input_lower_bounds[], std::string input_upper_bounds[], size_type dimensions );
  Box( std::pair<std::string,std::string> input_bounds[], size_type dimensions );
  Box( char* input_lower_bounds[], char* input_upper_bounds[], size_type dimensions );

  size_type dimensions();

  std::pair<std::string,std::string> getDimensionsBounds( size_type dimension );
  std::string getDimensionUpperBound( size_type dimension );
  std::string getDimensionLowerBound( size_type dimension );


};

#endif
