#include "engine.hpp"

int main()
{
	std::string sentence1 = "parse $[this]     sentence   with tree link $T[tree] and \n     incomplete link $ddd and $a[] and escaped dialog link $$d[dialog]";
	std::string::const_iterator it = sentence1.begin();
	
	textengine::configure config;
	textengine::Parser::Token token;
	token = textengine::Parser::parseLine(config, token, sentence1, it);

	std::cout << token.id << "\n" << token.link << "\n" << token.text << std::endl;
}