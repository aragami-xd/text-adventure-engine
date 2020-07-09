#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>

/**
 * handling exception
 */
class exception : public std::exception
{
private:
	const std::string message;

public:
	exception(const std::string &error) : message(error) {}
	const char *what() const noexcept { return message.c_str(); }
};

namespace textengine
{
	/**
	 * @class decision can be made at each dialog of the game tree
	 */
	class Decision
	{
	private:
		std::string link_;	  // link to another dialog of the tree or another tree
		std::string message_; // message to be displayed
		bool enabled_;		  // either the option is enabled
		int score_;			  // how many scores the decision worth

	public:
		/**
		 * construct a choice
		 * @param _link link to another dialog of the tree or another tree
		 * @param _message the message for that choice
		 */
		Decision(const std::string &_link, const std::string &_message, bool _enabled = true)
			: link_(_link), message_(_message), enabled_(_enabled)
		{
		}

		inline std::string &link() { return link_; }
		inline std::string &message() { return message_; }

		inline bool enabled(bool _enable) { return enabled_ = _enable; }
		inline bool isEnabled() { return enabled_; }

		inline int score(int _score) { return score_ = _score; }
		inline int score() { return score_; }
	};

	/**
	 * @class each dialog (event) of a game tree
	 */
	class Dialog
	{
	private:
		std::vector<Decision> dialogs_; // choices can be made at the node
		std::string link_;				// link to another node
		std::string message_;			// message that the node contains
	public:
		Dialog(const std::string &_link, const std::string &_message)
			: link_(_link), message_(_message)
		{
		}

		inline std::string &link() { return link_; }
		inline std::string &message() { return message_; }

		/**
		 * insert new decision to the dialog node
		 * @param _choice_link link for the decision
		 * @param _choice_message message for the decision
		 */
		inline void insert(const std::string &_choice_link, const std::string &_choice_message)
		{
			dialogs_.push_back(Decision(_choice_link, _choice_message));
		}

		/**
		 * get a decision with a specific link
		 */
		// inline Decision
	};

	/**
	 * @class the game tree, recommend 1 tree per level, can be linked with other trees
	 */
	class Tree
	{
	private:
		int score_;
		std::unordered_map<std::string, Dialog> tree_;

	public:
		Tree() : score_(0)
		{
		}

		bool insert()
		{
		}
	};

	/**
	 * @class parse a data file and create a game tree
	 */
	class Parser
	{
	private:
		static Tree &parseDialog(Tree &tree, std::string &line, std::fstream &file)
		{
		}

		static Tree &parseDecision(Tree &tree, std::string &line, std::fstream &file)
		{
		}

	public:
		static Tree create(std::fstream &_file)
		{
			std::string line; // line to be parsed
			Tree tree;		  // game tree

			// parse the entire file, or until an error is found
			while (getline(_file, line))
			{
				// parse a node
				if (line[0] == '-' && line[1] == ' ')
					tree = parseDialog(tree, line, _file);

				// parse a choice
				else if (line[0] == '+' && line[1] == ' ')
					tree = parseDecision(tree, line, _file);

				// else, return false
				else
					throw exception("unknown parsing error");
			}
		}
	};

	/**
	 * @class the runtime engine
	 */
	class Engine
	{
	public:
		Engine(const std::vector<std::string> &files)
		{
		}

		bool run()
		{
		}
	};
} // namespace textengine