#include <regex_helper.h>
#include <stream_insert_overloads.h>

void print_help_and_exit(int exit_code)
{
  std::string const help_string =
R"(
  Usage: $ cppregex text regex [options...]

  cppregex is a tool for running std::regex in C++. Give the tool the text and regex, and any desired
  options, and get back JSON detailing the query and results. The default behavior, if no options are
  specified, is a std::regex_search of the text, using the Modified ECMAScript regular expression grammar.
  All other options are disabled unless requested. The options to the program reflect the options of the
  std::regex API, and can be broadly categorized into algorithm, grammar, match, and syntax options.

  Algorithm options:
  ==================
    --search             Search the entire text (std::regex_search) and return all matches. [Default]
    --match              Attempt to match the entire text (std::regex_match) and return a match if it exists.
    --replace            Replace occurrences of regex matches in the text with std::regex_replace.

  Grammar options:
  ================

      Only one option can be enabled at a time. The grammar option that appears last in the argument
      list will be the grammar to use.

    --ecmascript         Use the Modified ECMAScript regular expression grammar. [Default]
    --extended           Use the extended POSIX regular expression grammar.
    --basic              Use the basic POSIX regular expression grammar.
    --awk                Use the regular expression grammar used by the awk utility in POSIX.
    --grep               Use the regular expression grammar used by the grep utility in POSIX.
                           This is effectively the same as the basic option with the addition of newline
                           '\n' as an alternation separator.
    --egrep              Use the regular expression grammar used by the grep utility, with the -E option, in POSIX.
                           This is effectively the same as the extended option with the addition of newline
                          '\n' as an alternation separator in addtion to '|'.

  Match options:
  ==============

      All of the following options are disabled by default, except for 'format-default'.
      In the following descriptions, [first, last) refers to the character sequence being matched.

    --match-not-bol      The first character in [first, last) will be treated as if it is not at the beginning of a
                           line (i.e. ^ will not match [first, first)).
    --match-not-eol      The last character in [first, last) will be treated as if it is not at the end of a
                           line (i.e. $ will not match [last, last)).
    --match-not-bow      "\b" will not match [first, first).
    --match-not-eow      "\b" will not match [last, last).
    --match-any          If more than one match is possible, then any match is an acceptable result.
    --match-not-null     Do not match empty sequences.
    --match-continuous   Only match a sub-sequence that begins at first.
    --match-prev-avail   --first is a valid iterator position. When set, causes match-not-bol and match-not-bow
                           to be ignored.
    --format-default     Use ECMAScript rules to construct strings in std::regex_replace.
    --format-sed         Use POSIX sed utility rules in std::regex_replace.
    --format-no-copy     Do not copy un-matched strings to the output in std::regex_replace.
    --format-first-only  Only replace the first match in std::regex_replace.
    --format-string=""   The string to use to format the text. Works similarly to printf, but you can match groups.
                           For regex_constants::match_flag_type::format_default:
                             -Characters-                          -Replacement-
                                  $n         n-th backreference (i.e., a copy of the n-th matched group specified with
                                               parentheses in the regex pattern). n must be an integer value designating
                                               a valid backreference, greater than 1, and of two digits at most.
                                  $&         A copy of the entire match
                                  $`         The prefix (i.e., the part of the target sequence that precedes the match).
                                  $´         The suffix (i.e., the part of the target sequence that follows the match).
                                  $$         A single $ character.

  Syntax options (all disabled by default):
  =========================================
    --icase              Character matching should be performed without regard to case.
    --nosubs             When performing matches, all marked sub-expressions (expr) are treated as non-marking
                           sub-expressions (?:expr).
    --optimize           Instructs the regular expression engine to make matching faster, with the potential
                           cost of making construction slower.
    --collate            Character ranges of the form "[a-b]" will be locale sensitive.
    --multiline          Specifies that ^ shall match the beginning of a line and $ shall match the end of a line,
                           if the ECMAScript engine is selected.
)";
  std::cout << help_string << std::endl;
  exit(exit_code);
}

int main (int argc, char* const argv[])
{
  if (argc < 3)
    print_help_and_exit(-1);

  Regex_helper regex_helper = create_helper_from_args(argc, argv);
  regex_helper.pretty_print();

  return 0;
}
