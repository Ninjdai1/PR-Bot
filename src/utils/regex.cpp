#include "regex.h"
#include <regex>
using namespace std;

vector<string> search_regex(string text, regex expression){
    vector<string> matches;
    sregex_iterator m(text.begin(), text.end(), expression);
    sregex_iterator m_end;
 
    while (m != m_end) {
        matches.push_back(m->str());
        m++;
    }
    return matches;
};

regex ignored_content("\\[(.*?)\\]\\(.*?\\)|`[^``]*`");
regex string_cleaner("#|pret#");
vector<int> get_pr_numbers_from_tags(string text, regex expression){
    vector<int> prs;
    string parsed_text = regex_replace(text, ignored_content, "");
    for(string pr_string : search_regex(parsed_text, expression)){
        prs.push_back(stoi(regex_replace(pr_string, string_cleaner, "")));
    }
    return prs;
}
