#include <regex>
using namespace std;

const regex back_quote_content("/`[^``]*`/g");
const regex formatted_link_content("/\\[(.*?)\\]\\(.*?\\)/g");

const regex rhh_matches_regex("(^|\\s)#\\d+");
const regex pret_matches_regex("(^|\\s)pret#\\d+");

vector<string> search_regex(string text, regex expression);
vector<int> get_pr_numbers_from_tags(string text, regex expression);
