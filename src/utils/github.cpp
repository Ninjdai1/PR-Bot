#include "github.h"
#include <string>
#include "regex.h"
#include "misc.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

dpp::message generate_pr_message(dpp::cluster * bot, string GITHUB_TOKEN, string text){
    string parsed_message_content = regex_replace(regex_replace(text, formatted_link_content, " "), back_quote_content, " ");

    vector<int> rhh_matches = get_pr_numbers_from_tags(parsed_message_content, rhh_matches_regex);
    vector<int> pret_matches = get_pr_numbers_from_tags(parsed_message_content, pret_matches_regex);
    vector<int> fetched_rhh_matches;
    vector<int> fetched_pret_matches;

    dpp::message msg;

    for(int pr_number: rhh_matches){
        if (pr_number <= 20 || std::find(fetched_rhh_matches.begin(), fetched_rhh_matches.end(),pr_number)!=fetched_rhh_matches.end() || msg.components.size() == 5) continue;
        add_pr_button(bot, msg, GITHUB_TOKEN, pr_number, RHH);
    }

    for(int pr_number: pret_matches){
        add_pr_button(bot, msg, GITHUB_TOKEN, pr_number, PRET);
    }
    return msg;
}

void add_pr_button(dpp::cluster * bot, dpp::message msg, string GITHUB_TOKEN, int pr_number, Repo repo){
    std::promise<json> promise;
    std::future<json> future = promise.get_future();
    fetch_pr_data(bot, GITHUB_TOKEN, pr_number, repo, [&promise,&bot](const dpp::http_request_completion_t & cc){
        if(cc.status != 200) {
            promise.set_value(json());
            return 1;
        }
        promise.set_value(json::parse(cc.body));
        return 0;
    });

    json data = future.get();
    if(!data.is_null()) msg.add_component(generate_pr_button(repo, data));
}

dpp::component generate_pr_button(Repo repo, json data){
    string label = (repo==PRET ? "pret" : (string)"") + "#" + to_string(data["number"]) + " - " + (string)data["title"];
    return dpp::component().add_component(
        dpp::component()
            .set_type(dpp::component_type::cot_button)
            .set_label(label.length() > 80 ? label.substr(0, 77)+"..." : label)
            .set_style(dpp::component_style::cos_link)
            .set_url((string)data["html_url"])
    );
}


string get_pr_url(int pr_number, Repo repo){
    string repo_string = repo == RHH ? "rh-hideout/pokeemerald-expansion" : "pret/pokeemerald";
    return "https://api.github.com/repos/"+ repo_string + "/issues/" + to_string(pr_number);
}

void fetch_pr_data(dpp::cluster * bot, string GITHUB_TOKEN, int pr_number, Repo repo, dpp::http_completion_event callback){
    bot->request(
        get_pr_url(pr_number, repo), dpp::m_get, callback,
        "",
        "application/json",
        {
            {"Authorization", "Bearer "+GITHUB_TOKEN},
            {"Accept", "application/vnd.github+json"},
            {"User-Agent", "Ninjdai1/PR-Bot"}
        }
    );
}
