#include <match.h>

Submatch::Submatch(std::ssub_match const& submatch, size_t submatch_position)
: _length   (submatch.length()),
  _position (submatch_position),
  _text     (submatch.str())
{}

Match::Match(std::smatch&& match, std::string&& format_string)
{
  _formatted_string        = match.format(format_string);
  _match_successful        = !match.empty();
  _max_possible_submatches = match.max_size();
  _submatch_count          = match.size();

  for (size_t i = 0; i < _submatch_count; ++i)
    _submatches.push_back(Submatch(match[i], match.position(i)));
}
