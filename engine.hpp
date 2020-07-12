#pragma once
#include <string>
#include <map>
#include <vector>
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
	 * handling console i/o
	 */
	class console
	{
	private:
		static std::string indent_; // indentation string, default = 2 spaces
		static int level_;			// log level, default = 1

	public:
		/**
		 * set console output indentation, program default = 2 spaces
		 */
		inline static void indent(const std::string &_indent) { indent_ = _indent; }

		/**
		 * set console log level
		 * @param 0 log nothing
		 * @param 1 errors only (program default level)
		 * @param 2 errors and warnings
		 * @param 3 log everything
		 */
		inline static void logLevel(int _level) { level_ = _level; }

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
		 * @param level level of logging. level 1 (error) will throw an exception with message as the arg param
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
		std::string link_; // link to another dialog or tree
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
		std::map<std::string, Decision> decisions_;
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
		inline std::map<std::string, Decision> allDecisions() { return decisions_; }

		/** 
		 * insert a decision
		 * @exception duplicate decision id
		 */
		void insertDecision(const Decision &_decision)
		{
			if (decisions_.insert({_decision.id(), _decision}).second == false)
				console::log(1, "duplicate decision id: " + _decision.id());
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
			if (decisions_.insert({_id, Decision(_id, _message, _link, _enabled, _score)}).second == false)
				console::log(1, "duplicate decision id: " + _id);
		}

		/** 
		 * get a decision with a specific id 
		 * @exception cannot find decision with id
		 */
		Decision &decision(const std::string &_id)
		{
			auto it = decisions_.find(_id);
			if (it == decisions_.end())
				console::log(1, "cannot find decision with id: " + _id);
			return it->second;
		}
	};

	/**
	 * @class the game tree, recommend 1 tree per level, can be linked with other trees
	 */
	class Tree
	{
	private:
		std::map<std::string, Dialog> dialogs_;
		const std::string root_; // root node of the tree
		const std::string id_;
		int score_;

	public:
		/**
		 * construct a game tree
		 * @param _link link to the first dialog in the tree
		 * @param _initial_score the starting score of the tree
		 */
		Tree(const std::string &_root, int _initial_score = 0)
			: root_(_root), score_(_initial_score)
		{
		}

		/**
		 * insert dialog into the tree
		 * @exception duplicate dialog id
		 */
		void insertDialog(const Dialog &_dialog)
		{
			if (dialogs_.insert({_dialog.id(), _dialog}).second == false)
				console::log(1, "duplicate dialog id: " + _dialog.id());
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
			if (dialogs_.insert({_id, Dialog(_id, _message, _link)}).second == false)
				console::log(1, "duplicate dialog id: " + _id);
		}

		/** 
		 * get a dialog with a specific id 
		 * @exception cannot find dialog with id
		 */
		Dialog &dialog(const std::string &_id)
		{
			auto it = dialogs_.find(_id);
			if (it == dialogs_.end())
				console::log(1, "cannot find dialog with id: " + _id);
			return it->second;
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
	};

	/**
	 * @class parse a data file and create a game tree
	 */
	class Parser
	{
	private:
		static Tree currTree;
		static Dialog currDialog;
		static Decision currDecision;
		static std::string currText;

		static std::string parseMarker(const std::string &line, std::string::const_iterator &it)
		{
			// parse the marker
			std::string marker;
			while (it != line.end() && *it != ']')
			{
				marker.push_back(*it);
				it++;
			}

			// trim whitespaces after a marker
			while (it != line.end() && *it != ' ')
				it++;
			return marker;
		}

		static void parseLine(const std::string &line, std::string::const_iterator &it)
		{
			std::string id, treeLink, dialogLink;
			bool hasLink = false;
			while (it != line.end())
			{
				// if marker character `$` is found
				if (*it == '$')
				{
					it++;

					// `$[` creates id marker
					if (*it == '[')
						id = parseMarker(line, it);

					// if no link is parsed, try to parse tree and dialog marker
					// `$T[` creates tree marker and `$D[` creates dialog marker
					else if (!hasLink && (*it == 'T' || *it == 'D'))
					{
						char linkType = *it;
						it++;

						// if tree marker is completed
						if (*it == '[' && linkType == 'T')
						{
							treeLink = parseMarker(line, it);
							hasLink = true;
						}

						// if dialog marker is completed
						else if (*it == '[' && linkType == 'D')
						{
							dialogLink = parseMarker(line, it);
							hasLink = true;
						}

						// if neither is completed (not a marker), restore the text
						else
							currText += std::string("$" + linkType + *it);
					}

					// if no marker is completed, restore the text
					else
						currText += std::string("$" + *it);

					it++;
				}

				// if not marker character, it's normal text
				else
				{
					currText.push_back(*it);
					it++;
				}
			}
		}

		static void parseDialog();
		static void parseDecision();

	public:
		static Tree create(std::fstream &file)
		{
		}
	};

	class Engine
	{
	private:
		std::map<std::string, Tree> trees_;

		struct EngineConfig
		{
			std::string outputIndent = "  ";
			int logLevel = 1;

			bool displayDisabledDialogs = true;
		};
		const EngineConfig config_;

	public:
		Engine(const EngineConfig &_config)
		{
		}

		void parsePlotScripts(const std::vector<std::string> &_files)
		{
			for (unsigned int i = 0; i < _files.size(); i++)
			{
				std::fstream file(_files[i]);
				if (!file.is_open())
					console::log(1, "cannot open file: " + _files[i]);
				else
					trees_.insert({_files[i], Parser::create(file)});
			}
		}
	};
} // namespace textengine

// static initialization
std::string textengine::console::indent_ = "  ";
int textengine::console::level_ = 1;