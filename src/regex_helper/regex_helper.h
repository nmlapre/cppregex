#ifndef REGEX_HELPER_H
#define REGEX_HELPER_H

#include <iostream>
#include <memory>
#include <regex>
#include <vector>

#include <regex_options.h>
#include <results.h>

// Regex_helper is a class intended to do the heavy lifting of std::regex algorithms.
// Given the target text sequence, the text of the regex, and the Regex_options, the class
// constructor reads the options and executes the regex as specified by the arguments.
// The class gives access to its properties directly if needed. The pretty_print funciton
// writes a JSON representation of the query and results to standard output by calling
// the relevant operator<< functions on the members of Regex_helper, then parsing and
// serializing via nlohmann's C++ json library, which handles indentation and ensures
// adherence to the JSON specification. The class only allows move construction, as
// the strings may be quite large and therefore undesirable to copy.
class Regex_helper
{
  // Properties populated on construction.
  std::string              _text          {""};
  std::string              _regex         {""};
  Regex_options            _regex_options {};
  std::shared_ptr<Results> _results       {nullptr};

public:
  Regex_helper() = delete;
  Regex_helper(std::string   && text,
               std::string   && regex,
               Regex_options && regex_options);

  std::string              const& text()          const { return _text;          };
  std::string              const& regex()         const { return _regex;         };
  Regex_options            const& regex_options() const { return _regex_options; };
  std::shared_ptr<Results> const& results()       const { return _results;       };

  // Write formatted JSON to standard output.
  void pretty_print() const;

  friend std::ostream& operator<<(std::ostream& os, Regex_helper const& regex_helper);

private:  
  std::regex _construct_regex() const;

  // Helpers for each algorithm that call std::regex_match/search/replace and populate the results.
  void _execute_regex_match();
  void _execute_regex_search();
  void _execute_regex_replace();

  // Helper for Algorithm::Search.
  std::vector<Match> _get_matches_from_iterator(std::sregex_iterator begin,
                                                std::string          format_string) const;
};

// Read in command line arguments and create a Regex_helper reflecting those arguments.
Regex_helper create_helper_from_args(int argc, char* const argv[]);

#endif /* REGEX_HELPER_H */
