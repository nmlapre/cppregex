#ifndef RESULTS_H
#define RESULTS_H

#include <iostream>

#include <match.h>
#include <regex_options.h>

// The Results struct is an ancestor meant to hold common information among Results.
// The only common information is the algorithm type.
struct Results
{
  Results() = delete;
  Results(Algorithm algorithm);

  Algorithm algorithm() const { return _algorithm; };

  friend std::ostream& operator<<(std::ostream& os, Results const& results);

private:
  Algorithm _algorithm{Algorithm::match};

  virtual void _dummy() { };
};

// Match_results are Results that have a single Match that may or may not be successful.
struct Match_results : Results
{
  Match_results() = delete;
  Match_results(Match&& match);

  friend std::ostream& operator<<(std::ostream& os, Match_results const& match_results);

  Match const& match() const { return _match; }

private:
  Match _match {};

  void _dummy() {};
};

// Search_results are Results with a std::vector of potential Matches.
// It can be any number of Matches because the text may be of any size,
// and the search does not stop upon encountering a match.
struct Search_results : Results
{
  Search_results() = delete;
  Search_results(std::vector<Match>&& matches);

  friend std::ostream& operator<<(std::ostream& os, Search_results const& search_results);

  size_t                    match_count() const { return _match_count; }
  std::vector<Match> const& matches()     const { return _matches; }

private:
  size_t             _match_count {0};
  std::vector<Match> _matches     {};

  void _dummy() {};
};

// Replace_results are Results with a single piece of information: the target
// text sequence, with all regex matches replaced as specified by the format options.
struct Replace_results : Results
{
  Replace_results() = delete;
  Replace_results(std::string&& replaced_text);

  friend std::ostream& operator<<(std::ostream& os, Replace_results const& replace_results);

  std::string const& replaced_text() const { return _replaced_text; }

private:
  std::string _replaced_text {""};

  void _dummy() {};
};

#endif /* RESULTS_H */
