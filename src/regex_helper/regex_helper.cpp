#include <regex_helper.h>
#include <json.hpp>

// On construction, call the regex function according to the algorithm type.
Regex_helper::Regex_helper(std::string   && text,
                           std::string   && regex,
                           Regex_options && regex_options)
: _text          (std::move(text)),
  _regex         (std::move(regex)),
  _regex_options (std::move(regex_options))
{
  switch (regex_options.algorithm())
  {
    case Algorithm::match:
      _execute_regex_match();
      break;
    case Algorithm::search:
      _execute_regex_search();
      break;
    case Algorithm::replace:
      _execute_regex_replace();
      break;
    default:
      break;
  }
}

// Call std::regex_match() on the target text sequence, which will produce a std::smatch.
// Create a Match from that std::smatch, giving it the format string for output. Store
// the Search_results created with the match.
void Regex_helper::_execute_regex_match()
{
  auto regex           = _construct_regex();
  auto match_flag_mask = _regex_options.match_flag_mask();
  auto format_string   = _regex_options.format_string();

  std::smatch match_out;
  std::regex_match(_text, match_out, regex, match_flag_mask);

  Match match = Match(std::move(match_out), std::move(format_string));

  _results = std::make_shared<Match_results>(std::move(match));
}

// Call std::regex_search iteratively on the text with std::sregex_iterator.
// A single std::regex_search only finds the first potential match. This function
// searches the entire text, regardless of matches, and places them in a
// std::vector<Match>, which it std::moves into the Search_results constructor.
void Regex_helper::_execute_regex_search()
{
  auto text_begin      = _text.begin();
  auto text_end        = _text.end();
  auto regex           = _construct_regex();
  auto match_flag_mask = _regex_options.match_flag_mask();
  auto format_string   = _regex_options.format_string();

  auto regex_iterator_begin = std::sregex_iterator(text_begin, text_end, 
                                                   regex,
                                                   match_flag_mask);

  auto matches = _get_matches_from_iterator(regex_iterator_begin, format_string);

  _results = std::make_shared<Search_results>(std::move(matches));
}

// Call std::regex_replace on the target text sequence, which produces a
// formatted result std::string. Create a Replace_results object with that
// result.
void Regex_helper::_execute_regex_replace()
{
  auto regex           = _construct_regex();
  auto format_string   = _regex_options.format_string();
  auto match_flag_mask = _regex_options.match_flag_mask();
  
  std::string replace_result = std::regex_replace(_text,
                                                  regex,
                                                  std::move(format_string),
                                                  match_flag_mask);

  _results = std::make_shared<Replace_results>(std::move(replace_result));
}

// Take the regex text and the mask generated from the syntax options, and
// create a std::regex to use in the std::regex matching functions.
std::regex Regex_helper::_construct_regex() const
{
  return std::regex(_regex, _regex_options.syntax_option_mask());  
}

// Given a std::sregex_iterator created from the target text sequence,
// regex, and match flags, iterate through it and construct Matches with
// the desired format string applied.
std::vector<Match>
Regex_helper::_get_matches_from_iterator(std::sregex_iterator begin,
                                         std::string          format_string) const
{  
  auto end = std::sregex_iterator();
  std::vector<Match> matches;
  matches.reserve(std::distance(begin, end));

  for (std::sregex_iterator it = begin; it != end; ++it)
  {
    std::smatch match = *it;
    matches.emplace_back(Match(std::move(match), std::move(format_string)));
  }

  return matches;
}

// Parse the command line arguments into a Regex_helper, and return it.
// The following describes the structure of the parameters of the program:
// 
// $ cppregex text regex [options...]
//   argv[0]  [1]  [2]   [3...argc]
//
// argv[0]      is ignored, since it holds the program name.
// argv[1,3)    must hold the target text sequence and regex strings
// argv[3,argc) may hold additional options
Regex_helper create_helper_from_args(int argc, char* const argv[])
{
  std::vector<std::string> text_and_regex_arguments(argv + 1, argv + 3);
  std::vector<std::string> option_arguments        (argv + 3, argv + argc);

  std::string text  = text_and_regex_arguments.at(0);
  std::string regex = text_and_regex_arguments.at(1);

  Regex_options regex_options = create_regex_options_from_option_arguments(option_arguments);

  return Regex_helper(std::move(text),
                      std::move(regex),
                      std::move(regex_options));
}

void Regex_helper::pretty_print() const
{
  std::stringstream json_output_buffer;
  json_output_buffer << *this;
  nlohmann::json j = nlohmann::json::parse(json_output_buffer.str());
  std::cout << j.dump(2) << std::endl;
}
