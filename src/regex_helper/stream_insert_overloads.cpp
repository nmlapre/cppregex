#include <stream_insert_overloads.h>

// Only acceptable escape sequences in JSON strings: '\b', '\f', '\n', '\r', '\t', '\"', '\\'
bool is_valid_json_string_escape_sequence(char escaped_character)
{
  if (escaped_character == 'b'  ||
      escaped_character == 'f'  ||
      escaped_character == 'n'  ||
      escaped_character == 'r'  ||
      escaped_character == 't'  ||
      escaped_character == '"'  ||
      escaped_character == '\\')
  {
    return true;
  }
  return false;
}

// Look through the input string and return a new string built by adding escapes (\)
// to the text such that the result forms a valid string value in a JSON object.
// Assume that all characters in the string are escaped as intended for the regex engine.
std::string escape_for_json(std::string const& s)
{
  std::string res;
  std::string::const_iterator it = s.begin();
  while (it != s.end())
  {
    char current = *it;
    if (current == '\\' && it != s.end())
    {
      ++it;
      char next = *it;
      if (is_valid_json_string_escape_sequence(next))
      {
        // Move along as-is since the sequence is valid.
        res += current;
      }
      else
      {
        // Not valid, so escape the backslash so that the JSON is valid.
        res += '\\';
        res += current;
      }
    }
    else if (current == '"' && it != s.end())
    {
      res += '\\';
    }
    res += *it;
    ++it;
  }
  return res;
}

std::ostream& operator<<(std::ostream& os, Algorithm algorithm)
{
  switch (algorithm)
  {
    case Algorithm::match:
      os << "match";
      break;
    case Algorithm::search:
      os << "search";
      break;
    case Algorithm::replace:
      os << "replace";
      break;
    default:
      break;
  }

  return os;
}

std::ostream& operator<<(std::ostream& os, Grammar grammar)
{
  switch (grammar)
  {
    case Grammar::ecmascript:
      os << "ecmascript";
      break;
    case Grammar::basic:
      os << "basic";
      break;
    case Grammar::extended:
      os << "extended";
      break;
    case Grammar::awk:
      os << "awk";
      break;
    case Grammar::grep:
      os << "grep";
      break;
    case Grammar::egrep:
      os << "egrep";
      break;
    default:
      break;
  }

  return os;
}

std::ostream& operator<<(std::ostream& os, Regex_options const& regex_options)
{
  os << "{"                                                                   << "\n";
  os << "\"algorithm\": "      << '"' << regex_options._algorithm      << '"' << ",\n";
  os << "\"syntax_options\": "        << regex_options._syntax_options        << ",\n";
  os << "\"match_options\": "         << regex_options._match_options         << "\n";
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, Syntax_options const& syntax_options)
{
  std::string const t = "true",
                    f = "false";
  os << "{"                                                                    << "\n";
  os << "\"grammar\": "   << '"' << syntax_options._grammar             << '"' << ",\n";
  os << "\"icase\": "            << (syntax_options._icase     ? t : f)        << ",\n";
  os << "\"nosubs\": "           << (syntax_options._nosubs    ? t : f)        << ",\n";
  os << "\"optimize\": "         << (syntax_options._optimize  ? t : f)        << ",\n";
  os << "\"collate\": "          << (syntax_options._collate   ? t : f)        << ",\n";
  os << "\"multiline\": "        << (syntax_options._multiline ? t : f)        << "\n";
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, Match_options const& match_options)
{
  std::string const t = "true",
                    f = "false";
  os << "{"                                                                                      << "\n";
  os << "\"match_not_bol\": "            << (match_options._match_not_bol     ? t : f)           << ",\n";
  os << "\"match_not_eol\": "            << (match_options._match_not_eol     ? t : f)           << ",\n";
  os << "\"match_not_bow\": "            << (match_options._match_not_bow     ? t : f)           << ",\n";
  os << "\"match_not_eow\": "            << (match_options._match_not_eow     ? t : f)           << ",\n";
  os << "\"match_any\": "                << (match_options._match_any         ? t : f)           << ",\n";
  os << "\"match_not_null\": "           << (match_options._match_not_null    ? t : f)           << ",\n";
  os << "\"match_continuous\": "         << (match_options._match_continuous  ? t : f)           << ",\n";
  os << "\"match_prev_avail\": "         << (match_options._match_prev_avail  ? t : f)           << ",\n";
  os << "\"format_default\": "           << (match_options._format_default    ? t : f)           << ",\n";
  os << "\"format_sed\": "               << (match_options._format_sed        ? t : f)           << ",\n";
  os << "\"format_no_copy\": "           << (match_options._format_no_copy    ? t : f)           << ",\n";
  os << "\"format_first_only\": "        << (match_options._format_first_only ? t : f)           << ",\n";
  os << "\"format_string\": "     << '"' << escape_for_json(match_options._format_string) << '"' << "\n"; 
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, Submatch const& submatch)
{
  os << "{"                                                                 << "\n";
  os << "\"length\": "            << submatch._length                       << ",\n";
  os << "\"text\": "       << '"' << escape_for_json(submatch._text) << '"' << ",\n";
  os << "\"position\": "          << submatch._position                     << "\n";
  os << "}";
  return os;
};

std::ostream& operator<<(std::ostream& os, Match const& match)
{
  std::string match_successful_str = match._match_successful ? "true" : "false";

  os << "{"                                                                                       << "\n";
  os << "\"match_successful\": "        << '"' << match_successful_str                     << '"' << ",\n";
  os << "\"max_possible_submatches\": "        << match._max_possible_submatches                  << ",\n";
  os << "\"submatch_count\": "                 << match._submatch_count                           << ",\n";
  os << "\"formatted_string\": "        << '"' << escape_for_json(match._formatted_string) << '"' << ",\n"; 
  os << "\"submatches\": ["                                                                       << "\n";
  for (size_t i = 0; i < match._submatch_count; ++i)
  {
    os << match._submatches.at(i);
    if (i < match._submatch_count - 1)
      os << ",";
    os << "\n";
  }
  os << "]\n";
  os << "}";
  return os;
};

// Output the Result base class info, without any objects, just properties.
std::ostream& operator<<(std::ostream& os, Results const& results)
{
  os << "\"algorithm\": \"" << results._algorithm << "\"";
  return os;
}

std::ostream& operator<<(std::ostream& os, Match_results const& match_results)
{
  os << "{"                                   << "\n";
  os << static_cast<Results>(match_results)   << ",\n";
  os << "\"match\": " << match_results._match << "\n";
  os << "}";
  return os;
};

std::ostream& operator<<(std::ostream& os, Search_results const& search_results)
{
  os << "{"                                                                  << "\n";
  os << static_cast<Results>(search_results)                                 << ",\n";
  os << "\"match_count\": "                  << search_results._match_count  << ",\n";
  os << "\"matches\": ["                                                     << "\n";
  for (size_t i = 0; i < search_results._matches.size(); ++i)
  {
    os << search_results._matches.at(i);
    if (i < search_results._matches.size() - 1)
      os << ",";
    os << "\n";
  }
  os << "]\n";
  os << "}";
  return os;
};

std::ostream& operator<<(std::ostream& os, Replace_results const& replace_results)
{
  os << "{"                                                                                    << "\n";
  os << static_cast<Results>(replace_results)                                                  << ",\n";
  os << "\"replaced_text\": " << '"' << escape_for_json(replace_results._replaced_text) << '"' << "\n";
  os << "}";
  return os;
};

std::ostream& operator<<(std::ostream& os, Regex_helper const& regex_helper)
{
  os << "{"                                                                         << "\n";
  os << "\"text\": "          << '"' << escape_for_json(regex_helper._text)  << '"' << ",\n";
  os << "\"regex\": "         << '"' << escape_for_json(regex_helper._regex) << '"' << ",\n";
  os << "\"regex_options\": "        << regex_helper._regex_options                 << ",\n";
  os << "\"results\": ";
  std::shared_ptr<Results> results = regex_helper._results;
  if (results)
  {
    switch (results->algorithm())
    {
      case Algorithm::match:
      {
        os << *std::dynamic_pointer_cast<Match_results>(results);
        break;
      }
      case Algorithm::search:
      {
        os << *std::dynamic_pointer_cast<Search_results>(results);
        break;
      }
      case Algorithm::replace:
      {
        os << *std::dynamic_pointer_cast<Replace_results>(results);
        break;
      }
      default:
        break;
    }
  }
  
  os << "\n";
  os << "}";
  return os;
}
