#include "engine.hpp"

int main(int args, char *argv[])
{
	textengine::Engine *engine = new textengine::Engine();
	for (int i = 1; i < args; i++)
		engine->parseScriptFile(std::string(argv[i]));
	auto const &trees = engine->tree();
	delete engine;

	for (auto const &t : trees)
	{
		textengine::console::out("tree");
		auto dialogs = t.second->allDialogs();
		for (auto const &d : dialogs)
		{
			textengine::console::out(d.second->message());
			auto decisions = d.second->allDecisions();
			for (auto de : decisions)
				textengine::console::out(de.second->message());
		}
		textengine::console::out("\n\n\n");
	}
}