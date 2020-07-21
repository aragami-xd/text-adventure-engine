#pragma once
#include <string>
#include <map>
#include <iostream>
#include <fstream>

namespace textengine
{
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

	/**
	 * @class config engine components: console, engine, parser...
	 */
	struct configure
	{
		/** 
		 * console config
		 */

		// indentation for subsequent statements in console::out() function
		// default = 2 spaces, use "" for no indent
		std::string output_indent = "  ";

		// set console log level
		// 0 - log nothing
		// 1 - errors only (default)
		// 2 - errors and warnings
		// 3 - log everything
		int log_level = 1;

		/** 
		 * console config
		 */

		// control whether to trim the whitespaces after a marker
		// true - all whitespaces after a marker up until a text will be trimmed (default)
		bool trim_whitespaces_behind_markers = true;

		/** 
		 * console config
		 */

		// control whether disabled decisions will be displayed or not
		// true - the decision is shown but cannot be chosen (default)
		// false - the decision will not be shown and cannot be chosen
		bool display_disabled_decisions = true;
	};

	/**
	 * handling console i/o
	 */
	class console
	{
	private:
		static std::string indent_; // indentation string
		static int level_;			// log level

	public:
		/**
		 * set console output indentation
		 */
		inline static void indent(const std::string &_indent) { indent_ = _indent; }

		/**
		 * set console log level
		 * @param 0 log nothing
		 * @param 1 errors only
		 * @param 2 errors and warnings
		 * @param 3 log everything
		 */
		inline static void level(int _level) { level_ = _level; }

		/**
		 * output function, can print any ostream compatible data types
		 * @param arg first argument to be displayed, not indented
		 * @param ... subsequent arguments, indented with console class indent value
		 */
		template <typename Arg, typename... Args>
		static void out(Arg &&arg, Args &&... args)
		{
			std::cout << std::forward<Arg>(arg);
			((std::cout << "\n"
						<< indent_ << std::forward<Args>(args)),
			 ...);
			std::cout << std::endl;
		}

		/**
		 * log to console
		 * @param level level of logging:
		 * @param 1 errors, will throw exception with the arg as message
		 * @param 2 warnings
		 * @param 3 info
		 * @param arg first argument to be displayed, not indented
		 * @param ... subsequent arguments, indented with console class indent value
		 */
		template <typename Arg, typename... Args>
		static void log(int level, Arg &&arg, Args &&... args)
		{
			if (level == 1)
				throw exception(arg);
			else if (level < 1 || level > level_)
				return;
			else
				out(arg, args...);
		}
	};

	/**
	 * @class decisions can be made at each dialog of the game tree
	 */
	class Decision
	{
	private:
		const std::string id_;
		std::string message_;
		std::string link_; // link to another dialog or tree, the final character can be ('D', 'd', 'T', 't', corresponding to link type)
		bool enabled_;	   // whether the decision can be chosen
		int score_;		   // how much the decision worth

	public:
		/**
		 * construct a choice
		 * @param _id id of the decision
		 * @param _message message to be displayed
		 * @param _link link to another dialog or tree, default = null (no link)
		 * @param _enabled whether the decision can be chosen, default = true
		 * @param _score how much the decision worth, default = 0
		 */
		Decision(const std::string &_id, const std::string &_message, const std::string &_link = "", bool _enabled = true, int _score = 0)
			: id_(_id), message_(_message), link_(_link), enabled_(_enabled), score_(_score)
		{
		}

		/** 
		 * get decision id
		 */
		inline const std::string &id() const { return id_; }

		/**
		 *  set decision message 
		 */
		inline void message(const std::string &_message) { message_ = _message; }

		/** 
		 * get decision message 
		 */
		inline std::string message() { return message_; }

		/** 
		 * set decision link
		 */
		inline void link(const std::string &_link) { link_ = _link; }

		/**
		 * get decision link 
		 */
		inline std::string link() { return link_; }

		/**
		 * set enabled status 
		 */
		inline void enabled(bool _enable) { enabled_ = _enable; }

		/** 
		 * get enabled status
		 */
		inline bool enabled() { return enabled_; }

		/** 
		 * set score
		 */
		inline void score(int _score) { score_ = _score; }

		/** 
		 * get score
		 */
		inline int score() { return score_; }
	};

	/**
	 * @class each dialog (event) of a game tree
	 */
	class Dialog
	{
	private:
		std::map<std::string, Decision *> decisions_;
		const std::string id_;
		std::string message_;
		std::string link_; // link to another dialog or tree

	public:
		/**
		 * create a new dialog
		 * @param _id id of the dialog
		 * @param _message message to be displayed
		 * @param _link link to another dialog or another tree, default = null (no link)
		 */
		Dialog(const std::string &_id, const std::string &_message, const std::string &_link = "")
			: id_(_id), message_(_message), link_(_link)
		{
		}

		/** 
		 * get dialog id
		 */
		inline const std::string &id() const { return id_; }

		/**
		 * set dialog message
		 */
		inline void message(const std::string &_message) { message_ = _message; }

		/** 
		 * get dialog message
		 */
		inline std::string message() { return message_; }

		/** 
		 * set dialog link
		 */
		inline void link(const std::string &_link) { link_ = _link; }

		/** 
		 * get dialog link
		 */
		inline std::string link() { return link_; }

		/**
		 * get all decisions of the dialog
		 */
		inline std::map<std::string, Decision *> allDecisions() { return decisions_; }

		/** 
		 * insert a decision
		 * @exception duplicate decision id
		 */
		void insertDecision(Decision *_decision)
		{
			if (decisions_.insert({_decision->id(), _decision}).second == false)
				console::log(1, "duplicate decision id: " + _decision->id());
		}

		/** 
		 * insert a decision
		 * @param _id id of the decision
		 * @param _message message of the decision
		 * @param _link link of the decision to another dialog or tree, default = null (empty)
		 * @param _enabled enabled status, default = true
		 * @param _score how much the decision worth, default = 0
		 * @exception duplicate decision id
		 */
		void insertDecision(const std::string &_id, const std::string &_message, const std::string &_link = "", bool _enabled = true, int _score = 0)
		{
			if (decisions_.insert({_id, new Decision(_id, _message, _link, _enabled, _score)}).second == false)
				console::log(1, "duplicate decision id: " + _id);
		}

		/** 
		 * get a decision with a specific id 
		 * @exception cannot find decision with id
		 */
		Decision *decision(const std::string &_id)
		{
			auto it = decisions_.find(_id);
			if (it == decisions_.end())
				console::log(1, "cannot find decision with id: " + _id);
			return it->second;
		}

		/**
		 * destructor, delete all decisions
		 */
		~Dialog()
		{
			for (auto d : decisions_)
				delete d.second;
		}
	};

	/**
	 * @class the game tree, recommend 1 tree per level, can be linked with other trees
	 */
	class Tree
	{
	private:
		std::map<std::string, Dialog *> dialogs_; // all dialogs
		const std::string id_;					  // id of the tree
		const std::string root_;				  // first dialog of the tree
		int score_;								  // current score of the tree

	public:
		/**
		 * construct a game tree
		 * @param _link link to the first dialog in the tree
		 * @param _initial_score the starting score of the tree
		 */
		Tree(const std::string &_id, const std::string &_root, int _initial_score = 0)
			: id_(_id), root_(_root), score_(_initial_score)
		{
		}

		/**
		 * insert dialog into the tree
		 * @exception duplicate dialog id
		 */
		void insertDialog(Dialog *_dialog)
		{
			if (dialogs_.insert({_dialog->id(), _dialog}).second == false)
				console::log(1, "duplicate dialog id: " + _dialog->id());
		}

		/** 
		 * insert dialog into the tree
		 * @param _id id of the dialog
		 * @param _message message of the dialog to be displayed
		 * @param _link link of the dialog to another dialog or tree
		 * @exception duplicate dialog id
		 */
		void insertDialog(const std::string &_id, const std::string &_message, const std::string &_link = "")
		{
			if (dialogs_.insert({_id, new Dialog(_id, _message, _link)}).second == false)
				console::log(1, "duplicate dialog id: " + _id);
		}

		/** 
		 * get a dialog with a specific id 
		 * @exception cannot find dialog with id
		 */
		Dialog *dialog(const std::string &_id)
		{
			auto it = dialogs_.find(_id);
			if (it == dialogs_.end())
				console::log(1, "cannot find dialog with id: " + _id);
			return it->second;
		}

		/**
		 * get all dialogs
		 */
		inline std::map<std::string, Dialog *> allDialogs()
		{
			return dialogs_;
		}

		/**
		 * increment score, use negative value to decrement
		 */
		inline void incrementScore(int _value) { score_ += _value; }

		/**
		 * set score to a specific value
		 */
		inline void score(int _value) { score_ = _value; }

		/**
		 * get score
		 */
		inline int score() { return score_; }

		/**
		 * destructor, delete all dialogs
		 */
		~Tree()
		{
			for (auto d : dialogs_)
				delete d.second;
		}
	};

	/**
	 * @class parse a data file and create a game tree
	 */
	class Parser
	{
	private:
		static int uniqueInt;

		/**
		 * @class parsing token
		 */
		struct Token
		{
			std::string text = "";

			std::string id = "";
			std::string link = "";
			bool isTreeLink = false;

			bool hasId = false;
			bool hasLink = false;
		};

		/**
		 * parse a marker (id, links...)
		 * @param line current line to be parsed
		 * @param it reference to line iterator at the point the function is called, return the reference at the closing bracket `]`
		 * @return the parsed marker value
		 */
		static std::string parseMarkerValue(const configure &config, const std::string &line, std::string::const_iterator &it)
		{
			// parse the marker
			std::string marker;
			while (++it != line.end() && *it != ']')
				marker.push_back(*it);

			// trim whitespaces after a marker (if option is enabled)
			if (config.trim_whitespaces_behind_markers)
				while (it != line.end() && *(it + 1) == ' ')
					it++;

			return marker;
		}

		/**
		 * parse a line
		 * can parse id and link (either type) anywhere in the line
		 * @param config program configuration
		 * @param curr current token being parsed
		 * @param line the full text line
		 * @param it iterator of the line
		 */
		static Token parseLine(const configure &config, Token &curr, int line_num, const std::string &line, std::string::const_iterator &it)
		{
			while (++it != line.end())
			{
				// if marker character `$` is found
				if (*it == '$')
				{
					it++;
					// `$[` creates id marker
					if (*it == '[')
					{
						std::string id = parseMarkerValue(config, line, it);
						if ((curr.hasId = !curr.hasId)) // basically check if hasId is false by flipping it and see if it's true
							curr.id = id;
						else
							console::log(1, "line [" + std::to_string(line_num) + "]: found another id within token: " + id);
					}

					// if no link is parsed, try to parse tree and dialog marker
					// `$T[` or `$t[` creates tree marker and `$D[` or `$d[` creates dialog marker
					else if (*it == 'T' || *it == 't' || *it == 'D' || *it == 'd')
					{
						char linkType = *it;
						it++;

						// if marker is completed
						if (*it == '[')
						{
							std::string link = parseMarkerValue(config, line, it) + linkType;

							if ((curr.hasLink = !curr.hasLink)) // like hasId above
								curr.link = link;
							else
								console::log(1, "line [" + std::to_string(line_num) + "]: found another link within token: " + link);

							if (linkType == 'T' || linkType == 't')
								curr.isTreeLink = true;
							else
								curr.isTreeLink = false;
						}

						// if neither is completed (not a marker), restore the text
						else
							curr.text.append(std::string({'$', linkType, *it}));
					}

					// if 2 `$` are found, escape the phrase
					else if (*it == '$')
						curr.text.push_back('$');

					// if no marker is completed, restore the text
					else
						curr.text.append(std::string({'$', *it}));
				}

				// if not marker character, it's normal text
				else
					curr.text.push_back(*it);
			}
			return curr;
		}

		/**
		 * post processing the dialog
		 */
		static Tree *processDialog(const std::string &fname, Token &curr, Tree *tree, Dialog *dialog)
		{
			if (!curr.hasLink)
			{
				curr.link = std::to_string(++uniqueInt) + 'd';
				curr.isTreeLink = false;
				console::log(2, "no link found, create link to dialog: " + uniqueInt);
			}
			dialog = new Dialog(curr.id, curr.text, curr.link);
			std::cout << "new tree" << std::endl;

			// create the tree if it's not yet created (creating the tree requiring the link to the first dialog)
			if (tree == nullptr)
				tree = new Tree(fname, dialog->id(), 0);

			tree->insertDialog(dialog);
			return tree;
		}

	public:
		static Tree *create(const configure &config, const std::string &fname, std::fstream &file)
		{
			Tree *tree = nullptr;	  // current tree, will be created once the first dialog is successfully parsed
			Dialog *dialog = nullptr; // current dialog

			Token curr;			// current token
			int line_num = 0;	// line num, for debugging
			std::string line;	// current line to be parsed
			std::string concat; // concat all lines belong to the same token

			// parse till eof
			while (getline(file, line))
			{
				line_num++;
				if (line.empty()) // ignore empty lines
					continue;
				concat.append(line);

				// peek next line and see if it's a new token or not (or eof)
				// if true, start parsing the token
				if (!file.good() || (char)file.peek() == '-' || (char)file.peek() == '+')
				{
					std::string::const_iterator it = concat.begin() + 1; // ignore the first marking character
					while (it != concat.end() && *(it + 1) == ' ') 		 // trim whitespace
						it++;

					curr = parseLine(config, curr, line_num, concat, it);

					// if current token is a dialog
					if (concat.front() == '-')
						tree = processDialog(fname, curr, tree, dialog);

					// if currentt token is a decision, and there is a dialog to attach to
					// therefore, all decisions parsed before the first dialog is parsed in the file will be discarded
					else if (concat.front() == '+')
					{
						Decision *decision = new Decision(curr.id, curr.text, curr.link, true, 0);
						if (dialog != nullptr)
							dialog->insertDecision(decision);
					}
					else
						console::log(2, "found a decision cannot be attached to any dialog at line " + line_num);
				}
			}
			return tree;
		}
	};

	/**
	 * @class runtime engine
	 */
	class Engine
	{
	private:
		std::map<std::string, Tree *> trees;
		configure config;

	public:
		Engine() = default;

		void parseScriptFile(const std::string &fname)
		{
			std::fstream file(fname);
			if (!file.is_open())
				console::log(1, "cannot open file: " + fname);
			else
				trees.insert({fname, Parser::create(config, fname, file)});
		}

		inline std::map<std::string, Tree *> tree() { return trees; }

		~Engine()
		{
			for (auto t : trees)
				delete t.second;
		}
	};
} // namespace textengine

// static initialization
std::string textengine::console::indent_ = "  ";
int textengine::console::level_ = 4;
int textengine::Parser::uniqueInt = 3010299; // log 2, to create an unique id / link to dialog / decision