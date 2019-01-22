#include <results.h>

Results::Results(Algorithm algorithm)
: _algorithm(algorithm)
{}

Match_results::Match_results(Match&& match)
: Results(Algorithm::match),
  _match(std::move(match))
{}

Search_results::Search_results(std::vector<Match>&& matches)
: Results(Algorithm::search)
{
  for (auto& match : matches)
    _matches.emplace_back(std::move(match));
  _match_count = _matches.size();
}

Replace_results::Replace_results(std::string&& replaced_text)
: Results(Algorithm::replace),
  _replaced_text(std::move(replaced_text))
{}
