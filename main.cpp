#include "application.h"

int main( int argc, char* argv[] )
{
	Application application;
	application.init();

	while( !application.done() )
	{
		application.frame();
	}

	application.shutdown();
	return 0;
}