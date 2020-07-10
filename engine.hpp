#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <functional>

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
	 * @class decisions can be made at each dialog of the game tree
	 */
	class Decision
	{
	private:
		const std::string id_; // id of the decision
		std::string message_;  // message to be displayed
		std::string link_;	   // link to another dialog of the tree or another tree
		bool enabled_;		   // enabled status
		int score_;			   // how much that decision worth

	public:
		/**
		 * construct a choice
		 * @param _id id of the decision
		 * @param _message message to be displayed
		 * @param _link link to another dialog of the tree or another tree, default = null (no link)
		 * @param _enabled enabled status, default = true
		 * @param _score how much that decision worth, default = 0
		 */
		Decision(const std::string &_id, const std::string &_message, const std::string &_link = "", bool _enabled = true, int _score = 0)
			: id_(_id), message_(_message), link_(_link), enabled_(_enabled), score_(_score)
		{
		}

		/** get the decision id */
		inline const std::string &id() const { return id_; }

		/** set the decision message */
		inline void message(const std::string &_message) { message_ = _message; }

		/** get the decision message */
		inline std::string message() { return message_; }

		/** set the decision link */
		inline void link(const std::string &_link) { link_ = _link; }

		/** get the decision link */
		inline std::string link() { return link_; }

		/** set the enabled status */
		inline void enabled(bool _enable) { enabled_ = _enable; }

		/** get the enabled status */
		inline bool enabled() { return enabled_; }

		/** set the score (how much the decision worth) */
		inline void score(int _score) { score_ = _score; }

		/** get the score (how much the decision worth) */
		inline int score() { return score_; }
	};

	/**
	 * @class each dialog (event) of a game tree
	 */
	class Dialog
	{
	private:
		std::vector<Decision> decisions_; // choices can be made at the diaglog
		const std::string id_;			  // id of the dialog
		std::string message_;			  // message to be displayed
		std::string link_;				  // link to another dialog or another tree
	public:
		/**
		 * create a new dialog
		 * @param _id id of the dialog
		 * @param _message message to be displayed
		 * @param _link link to another dialog or another tree, default = null (no link)
		 */
		Dialog(const std::string &_id, const std::string &_message, const std::string &_link = "")
			: message_(_message), link_(_link)
		{
		}

		/** get the dialog id */
		inline const std::string &id() const { return id_; }

		/** set the dialog message */
		inline void message(const std::string &_message) { message_ = _message; }

		/** get the dialog message */
		inline std::string message() { return message_; }

		/** set the dialog link, the engine will ignore this if te dialog contains different choices */
		inline void link(const std::string &_link) { link_ = _link; }

		/** get the dialog link */
		inline std::string link() { return link_; }

		/**
		 * insert new decision to the dialog node
		 * @param _decision_link link for the decision
		 * @param _decision_message message for the decision
		 */
		inline Decision decision(const std::string &_decision_link, const std::string &_decision_message)
		{
			decisions_.push_back(Decision(_decision_link, _decision_message));
			return decisions_.back();
		}

		/**
		 * get a decision with a specific link
		 * @param _decision_link link for the decision
		 * @return the first decision with that link
		 */
		Decision decision(const std::string &_decision_link)
		{
			// loop through the vector
			for (unsigned int i = 0; i < decisions_.size(); i++)
				if (decisions_[i].link() == _decision_link)
					return decisions_[i];

			// throw exception if no decisions can be found
			throw exception("cannot find the decision with the link: " + _decision_link);
		}

		/** get the decision vector */
		inline std::vector<Decision> allDecisions() { return decisions_; }
	};

	/**
	 * @class the game tree, recommend 1 tree per level, can be linked with other trees
	 */
	class Tree
	{
	private:
		int score_;
		const std::string link_;
		const std::string id_;
	public:
		/**
		 * construct a game tree
		 * @param _link link to the first dialog in the tree
		 * @param _initial_score the starting score of the tree
		 */
		Tree(const std::string &_link, int _initial_score = 0)
			: link_(_link), score_(_initial_score)
		{
		}
	};

	/**
	 * @class parse a data file and create a game tree
	 */
	class Parser
	{
	private:
		static Dialog parseDialog(Tree &tree, std::string &line, std::fstream &file)
		{
		}

		static Dialog parseDecision(Tree &tree, std::string &line, std::fstream &file)
		{
		}

	public:
		static Tree create(std::fstream &_file)
		{
			std::string line;

			getline(_file, line); // parse the first line, contain the link to the first dialog
			Tree tree(line);

			// parse the entire file, or until an error is found
			while (getline(_file, line))
			{
				// parse a node
				if (line[0] == '-' && line[1] == ' ')
					parseDialog(tree, line, _file);

				// parse a choice
				else if (line[0] == '+' && line[1] == ' ')
					parseDecision(tree, line, _file);

				// else, return false
				else
					throw exception("unknown parsing error");
			}

			return tree;
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
			std::vector<Tree> gameTrees;
			
			for (int i = 0; i < files.size(); i++)
			{

			}
		}

		bool run()
		{
			while (1)
			{

			}
		}
	};
} // namespace textengine