#ifndef STREAM_INSERT_OVERLOADS_H
#define STREAM_INSERT_OVERLOADS_H

#include <regex_helper.h>

// This, operator<< overloads, write out program objects in a JSON format.
// The format is defined as follows:

/*
Regex_helper:
{
  Regex_options:
  {
    Syntax_options: {},
    Match_options: {}
  },

  // Possible result types:
  Search_results: {
    [
      Match: {
        [Submatch]
      }
    ]
  }

  // -OR-
  Match_results: {
    Match: {}
  }

  // -OR-
  Replace_results: {}
}
*/

std::ostream& operator<<(std::ostream& os, Algorithm              algorithm);
std::ostream& operator<<(std::ostream& os, Grammar                grammar);
std::ostream& operator<<(std::ostream& os, Match           const& match);
std::ostream& operator<<(std::ostream& os, Match_options   const& match_options);
std::ostream& operator<<(std::ostream& os, Match_results   const& match_results);
std::ostream& operator<<(std::ostream& os, Regex_helper    const& regex_helper);
std::ostream& operator<<(std::ostream& os, Regex_options   const& regex_options);
std::ostream& operator<<(std::ostream& os, Replace_results const& replace_results);
std::ostream& operator<<(std::ostream& os, Results         const& results);
std::ostream& operator<<(std::ostream& os, Search_results  const& search_results);
std::ostream& operator<<(std::ostream& os, Submatch        const& submatch);
std::ostream& operator<<(std::ostream& os, Syntax_options  const& syntax_options);

#endif /* STREAM_INSERT_OVERLOADS_H */
