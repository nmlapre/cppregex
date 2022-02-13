#ifndef REGEX_OPTIONS_H
#define REGEX_OPTIONS_H

#include <regex>

// Algorithms correspond to std::regex match algorithms.
enum class Algorithm
{
  match,   // std::regex_match
  search,  // std::regex_search
  replace  // std::regex_replace
};

// Grammars correspond to grammar-related std::regex_constants::syntax_option_type constants.
enum class Grammar
{
  ecmascript, // Use the Modified ECMAScript regular expression grammar.
  basic,      // Use the basic POSIX regular expression grammar.
  extended,   // Use the extended POSIX regular expression grammar.
  awk,        // Use the regular expression grammar used by the awk utility in POSIX.
  grep,       // Use the regular expression grammar used by the grep utility in POSIX.
              //   This is effectively the same as the basic option with the addition of
              //   newline '\n' as an alternation separator.
  egrep       // Use the regular expression grammar used by the grep utility, with the -E option, in POSIX.
              //   This is effectively the same as the extended option with the addition of
              //   newline '\n' as an alternation separator in addtion to '|'.
};

// Match_options contains options corresponding to std::regex_constants::match_flag_type constants.
// It also contains the format string whose syntax is controlled by other Match_options.
// The struct has a member function that returns the bitwise-OR'd std::regex_constants::match_flag_type.
struct Match_options
{
  Match_options() = default;
  Match_options(bool match_not_bol,
                bool match_not_eol,
                bool match_not_bow,
                bool match_not_eow,
                bool match_any,
                bool match_not_null,
                bool match_continuous,
                bool match_prev_avail,
                bool format_default,
                bool format_sed,
                bool format_no_copy,
                bool format_first_only,
                std::string&& format_string);

  std::string const&                    format_string() const;
  std::regex_constants::match_flag_type mask()          const;

  friend std::ostream& operator<<(std::ostream& os, Match_options const& match_options);

private:
  bool _match_not_bol     {false}; // The first character in [first,last) will be treated as if it is not at the beginning of a line (i.e. ^ will not match [first,first)
  bool _match_not_eol     {false}; // The last character in [first,last) will be treated as if it is not at the end of a line (i.e. $ will not match [last,last)
  bool _match_not_bow     {false}; // "\b" will not match [first,first)
  bool _match_not_eow     {false}; // "\b" will not match [last,last)
  bool _match_any         {false}; // If more than one match is possible, then any match is an acceptable result
  bool _match_not_null    {false}; // Do not match empty sequences
  bool _match_continuous  {false}; // Only match a sub-sequence that begins at first
  bool _match_prev_avail  {false}; // --first is a valid iterator position. When set, causes match_not_bol and match_not_bow to be ignored
  bool _format_default    {false}; // Use ECMAScript rules to construct strings in std::regex_replace
  bool _format_sed        {false}; // Use POSIX sed utility rules in std::regex_replace.
  bool _format_no_copy    {false}; // Do not copy un-matched strings to the output in std::regex_replace
  bool _format_first_only {false}; // Only replace the first match in std::regex_replace

  std::string _format_string {""}; // The format string describes how to output match info.
                                   // std::match_results::format uses it similarly to printf, but with match groups.
                                   // The interpretation of the string changes based on other flags in Match_options.
                                   // For std::regex_constants::match_flag_type::format_default (ECMAScript):
                                   // -characters-                                            -replacement-
                                   //      $n         n-th backreference (i.e., a copy of the n-th matched group specified with parentheses in the regex pattern).
                                   //                   n must be an integer value designating a valid backreference, greater than 1, and of two digits at most.
                                   //      $&         A copy of the entire match
                                   //      $`         The prefix (i.e., the part of the target sequence that precedes the match).
                                   //      $´         The suffix (i.e., the part of the target sequence that follows the match).
                                   //      $$         A single $ character.
};

// Match_options contains options corresponding to std::regex_constants::syntax_option_type constants.
// It also contains the grammar selection, which is itself a syntax option. The struct has a member
// function that returns the bitwise-OR'd std::regex_constants::syntax_option_type.
struct Syntax_options
{
  Syntax_options() = default;
  Syntax_options(Grammar grammar,
                 bool    icase,
                 bool    nosubs,
                 bool    optimize,
                 bool    collate,
                 bool    multiline);

  std::regex_constants::syntax_option_type mask() const;

  friend std::ostream& operator<<(std::ostream& os, Syntax_options const& syntax_options);

private:
  Grammar _grammar   {Grammar::ecmascript};
  bool    _icase     {false}; // Character matching should be performed without regard to case.
  bool    _nosubs    {false}; // When performing matches, all marked sub-expressions (expr) are treated as non-marking sub-expressions (?:expr).
  bool    _optimize  {false}; // Instructs the regular expression engine to make matching faster, with the potential cost of making construction slower.
  bool    _collate   {false}; // Character ranges of the form "[a-b]" will be locale sensitive.
  bool    _multiline {false}; // Specifies that ^ shall match the beginning of a line and $ shall match the end of a line, if the ECMAScript engine is selected.
};

// Regex_options is a struct containing Syntax_options, Match_options, and the selected std::regex
// match algorithm to use. It can give the bitmasks of its options structs, and the match format string.
struct Regex_options
{
  Regex_options() = default;
  Regex_options(Regex_options&&) = default;
  Regex_options(Algorithm      algorithm,
                Match_options  match_options,
                Syntax_options syntax_options);

  Algorithm algorithm() const { return _algorithm; };

  std::string const&                       format_string()      const;
  std::regex_constants::match_flag_type    match_flag_mask()    const;
  std::regex_constants::syntax_option_type syntax_option_mask() const;

  friend std::ostream& operator<<(std::ostream& os, Regex_options const& regex_options);

private:
  Algorithm      _algorithm      {Algorithm::search};
  Match_options  _match_options  {};
  Syntax_options _syntax_options {};
};

Regex_options create_regex_options_from_option_arguments(std::vector<std::string> const& option_arguments);

#endif /* REGEX_OPTIONS_H */
