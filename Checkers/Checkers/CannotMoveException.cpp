#include "stdafx.h"
#include <exception>
#include <stdexcept>
#include <sstream>
 
using namespace std;

class CannotMoveException: public runtime_error{
public:
	CannotMoveException();
};