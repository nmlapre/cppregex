#ifndef MATCH_H
#define MATCH_H

#include <vector>
#include <regex>

// A Submatch describes a component of a Match. A Submatch occurs when the user,
// in a matching regex, has specified a capture group with parentheses(). Each
// captured group will appear as a Submatch. It knows its position in the larger text.
struct Submatch
{
  Submatch() = delete;
  Submatch(std::ssub_match const& submatch, size_t submatch_position);

  friend std::ostream& operator<<(std::ostream& os, Submatch const& submatch);

private:
  size_t      _length   {0};
  size_t      _position {0};
  std::string _text     {""};
};

// A Match describes a possibly-sucessful std::regex function result. It is constructed
// from a std::smatch, whose information is translated into Match format, and an optional
// format std::string, which will be used on construction to generate the formatted string.
struct Match
{
  Match() = default;
  Match(std::smatch&& match, std::string&& format_string = "");

  friend std::ostream& operator<<(std::ostream& os, Match const& match);

private:
  std::string           _formatted_string        {""};
  bool                  _match_successful        {false};
  size_t                _max_possible_submatches {0};
  size_t                _submatch_count          {0};
  std::vector<Submatch> _submatches              {};      // [full match, submatch 1, submatch 2, ...]
};

#endif /* MATCH_H */
