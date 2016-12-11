#include <system/application.h>

int main( int argc, char* argv[] )
{
	Application application;
	bool success = application.init();

	if( success )
	{
		while( !application.done() )
		{
			application.frame();
		}
	}

	application.shutdown();
	return 0;
}
