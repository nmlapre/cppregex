#include <regex_options.h>

#include <iostream>

Syntax_options::Syntax_options(Grammar grammar,
                               bool    icase,
                               bool    nosubs,
                               bool    optimize,
                               bool    collate,
                               bool    multiline)
: _grammar   (grammar),
  _icase     (icase),
  _nosubs    (nosubs),
  _optimize  (optimize),
  _collate   (collate),
  _multiline (multiline)
{}

// Convert a Grammar to a std::regex_constants flag.
std::regex_constants::syntax_option_type mask_from_grammar(Grammar grammar)
{
  switch (grammar)
  {
    case Grammar::ecmascript:
      return std::regex_constants::ECMAScript;
    case Grammar::basic:
      return std::regex_constants::basic;
    case Grammar::extended:
      return std::regex_constants::extended;
    case Grammar::awk:
      return std::regex_constants::awk;
    case Grammar::grep:
      return std::regex_constants::grep;
    case Grammar::egrep:
      return std::regex_constants::egrep;
    default:
      break;
  }

  return {};
}

// Bitwise OR the Syntax_options constant properties together.
//   TODO: get multiline recognized (header issue?)
std::regex_constants::syntax_option_type Syntax_options::mask() const
{
  auto const no_mask = std::regex_constants::syntax_option_type(0);
  return mask_from_grammar(_grammar)                           |
         (_icase     ? std::regex_constants::icase     : no_mask) |
         (_nosubs    ? std::regex_constants::nosubs    : no_mask) |
         (_optimize  ? std::regex_constants::optimize  : no_mask) |
         (_collate   ? std::regex_constants::collate   : no_mask);
         // (_multiline ? std::regex_constants::multiline : no_mask);
}

Match_options::Match_options(bool match_not_bol,
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
                             std::string&& format_string)
: _match_not_bol     (match_not_bol),
  _match_not_eol     (match_not_eol),
  _match_not_bow     (match_not_bow),
  _match_not_eow     (match_not_eow),
  _match_any         (match_any),
  _match_not_null    (match_not_null),
  _match_continuous  (match_continuous),
  _match_prev_avail  (match_prev_avail),
  _format_default    (format_default),
  _format_sed        (format_sed),
  _format_no_copy    (format_no_copy),
  _format_first_only (format_first_only),
  _format_string     (std::move(format_string))
{}

std::string const& Match_options::format_string() const
{
  return _format_string;
}

// Bitwise OR the Match_options properties together.
std::regex_constants::match_flag_type Match_options::mask() const
{
  auto const no_mask = std::regex_constants::match_flag_type(0);
  return (_match_not_bol     ? std::regex_constants::match_not_bol     : no_mask) |
         (_match_not_eol     ? std::regex_constants::match_not_eol     : no_mask) |
         (_match_not_bow     ? std::regex_constants::match_not_bow     : no_mask) |
         (_match_not_eow     ? std::regex_constants::match_not_eow     : no_mask) |
         (_match_any         ? std::regex_constants::match_any         : no_mask) |
         (_match_not_null    ? std::regex_constants::match_not_null    : no_mask) |
         (_match_continuous  ? std::regex_constants::match_continuous  : no_mask) |
         (_match_prev_avail  ? std::regex_constants::match_prev_avail  : no_mask) |
         (_format_default    ? std::regex_constants::format_default    : no_mask) |
         (_format_sed        ? std::regex_constants::format_sed        : no_mask) |
         (_format_no_copy    ? std::regex_constants::format_no_copy    : no_mask) |
         (_format_first_only ? std::regex_constants::format_first_only : no_mask);
}

Regex_options::Regex_options(Algorithm      algorithm,
                             Match_options  match_options,
                             Syntax_options syntax_options)
: _algorithm      (algorithm),
  _match_options  (match_options),
  _syntax_options (syntax_options)
{}

std::string const& Regex_options::format_string() const
{
  return _match_options.format_string();
}

std::regex_constants::match_flag_type Regex_options::match_flag_mask() const
{
  return _match_options.mask();
}

std::regex_constants::syntax_option_type Regex_options::syntax_option_mask() const
{
  return _syntax_options.mask();
}

// Loop over all of the arguments and create a Regex_options with all properties set up.
Regex_options create_regex_options_from_option_arguments(std::vector<std::string> const& option_arguments)
{
  // Algorithm options
  Algorithm algorithm {Algorithm::search};

  // Grammar options
  Grammar grammar {Grammar::ecmascript};

  // Match options
  bool match_not_bol     {false};
  bool match_not_eol     {false};
  bool match_not_bow     {false};
  bool match_not_eow     {false};
  bool match_any         {false};
  bool match_not_null    {false};
  bool match_continuous  {false};
  bool match_prev_avail  {false};
  bool format_default    {false};
  bool format_sed        {false};
  bool format_no_copy    {false};
  bool format_first_only {false};
  std::string format_string {""};

  // Syntax_options
  bool icase     {false};
  bool nosubs    {false};
  bool optimize  {false};
  bool collate   {false};
  bool multiline {false};

  for (auto const& arg : option_arguments)
  {
    // Algorithm options:
    if (arg == "--search")
      algorithm = Algorithm::search;
    else if (arg == "--match")
      algorithm = Algorithm::match;
    else if (arg == "--replace")
      algorithm = Algorithm::replace;

    // Match options:
    else if (arg == "--match-not-bol")
      match_not_bol     = true;
    else if (arg == "--match-not-eol")
      match_not_eol     = true;
    else if (arg == "--match-not-bow")
      match_not_bow     = true;
    else if (arg == "--match-not-eow")
      match_not_eow     = true;
    else if (arg == "--match-any")
      match_any         = true;
    else if (arg == "--match-not-null")
      match_not_null    = true;
    else if (arg == "--match-continuous")
      match_continuous  = true;
    else if (arg == "--match-prev-avail")
      match_prev_avail  = true;
    else if (arg == "--format-default")
      format_default    = true;
    else if (arg == "--format-sed")
      format_sed        = true;
    else if (arg == "--format-no-copy")
      format_no_copy    = true;
    else if (arg == "--format-first-only")
      format_first_only = true;
    else if (arg.find("--format-string=") == 0)
    {
      const std::string format_string_arg_id_text = "--format-string=";
      const size_t      argument_start_position   = format_string_arg_id_text.length();

      // Get argument contained in [argument_start_position, arg.length()).
      format_string = arg.substr(argument_start_position);
    }

    // Grammar options:
    else if (arg == "--ecmascript")
      grammar = Grammar::ecmascript;
    else if (arg == "--basic")
      grammar = Grammar::basic;
    else if (arg == "--extended")
      grammar = Grammar::extended;
    else if (arg == "--awk")
      grammar = Grammar::awk;
    else if (arg == "--grep")
      grammar = Grammar::grep;
    else if (arg == "--egrep")
      grammar = Grammar::egrep;

    // Syntax options:
    else if (arg == "--icase")
      icase     = true;
    else if (arg == "--nosubs")
      nosubs    = true;
    else if (arg == "--optimize")
      optimize  = true;
    else if (arg == "--collate")
      collate   = true;
    else if (arg == "--multiline")
      multiline = true;
    else
      std::cout << "Ignoring invalid argument " << arg << std::endl;
  }
  auto match_options = Match_options(match_not_bol,
                                     match_not_eol,
                                     match_not_bow,
                                     match_not_eow,
                                     match_any,
                                     match_not_null,
                                     match_continuous,
                                     match_prev_avail,
                                     format_default,
                                     format_sed,
                                     format_no_copy,
                                     format_first_only,
                                     std::move(format_string));

  auto syntax_options = Syntax_options(grammar,
                                       icase,
                                       nosubs,
                                       optimize,
                                       collate,
                                       multiline);

  return Regex_options(algorithm, match_options, syntax_options);
}
