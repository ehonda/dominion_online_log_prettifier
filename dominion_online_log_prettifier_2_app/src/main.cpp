#include <iostream>

#include "../inc/log_prettifier_application.h"

int main(int argc, char* argv[]) {
	dominion_online::LogPrettifierApplication app(std::cout);
	app.run(argc, argv);
	return 0;
}
