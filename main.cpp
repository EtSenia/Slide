#include <iostream>

#include "antlr4-runtime.h"
#include "SlideLexer.h"
#include "SlideParser.h"
#include "SlideDriver.h"

using namespace antlr4;
using namespace std;

int main(int argc, const char* argv[]) {
	std::ifstream stream;
	stream.open(argv[1]);
	ANTLRInputStream input(stream);
	SlideLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	SlideParser parser(&tokens);

	tree::ParseTree* tree = parser.prog();
	SlideDriver* driver = new SlideDriver();
	driver->visit(tree);


	return 0;
}
