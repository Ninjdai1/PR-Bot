#include "github.h"
#include <algorithm>
#include <cstring>
#include <dpp/misc-enum.h>
#include <iterator>
#include <map>
#include <string>
#include "regex.h"
#include "misc.h"
#include <nlohmann/json.hpp>
#include <utility>
using json = nlohmann::json;
using namespace std;

map<int, PRData> rhh_cache;
map<int, PRData> pret_cache;

dpp::message generate_pr_message(dpp::cluster * bot, string GITHUB_TOKEN, string text){
    string parsed_message_content = regex_replace(regex_replace(text, formatted_link_content, " "), back_quote_content, " ");

    vector<int> rhh_matches = get_pr_numbers_from_tags(parsed_message_content, rhh_matches_regex);
    vector<int> pret_matches = get_pr_numbers_from_tags(parsed_message_content, pret_matches_regex);
    vector<int> fetched_rhh_matches;
    vector<int> fetched_pret_matches;

    dpp::message msg;

    for(int pr_number: rhh_matches){
        if (pr_number <= 20 || find(fetched_rhh_matches.begin(), fetched_rhh_matches.end(),pr_number)!=fetched_rhh_matches.end() || msg.components.size() == 5) continue;
        add_pr_button(bot, &msg, GITHUB_TOKEN, pr_number, RHH);
        fetched_rhh_matches.push_back(pr_number);
    }

    for(int pr_number: pret_matches){
        if (find(fetched_pret_matches.begin(), fetched_pret_matches.end(),pr_number)!=fetched_pret_matches.end() || msg.components.size() == 5) continue;
        add_pr_button(bot, &msg, GITHUB_TOKEN, pr_number, PRET);
        fetched_pret_matches.push_back(pr_number);
    }
    return msg;
}

void add_pr_button(dpp::cluster * bot, dpp::message * msg, string GITHUB_TOKEN, int pr_number, Repo repo){
    auto cached_data = repo == RHH ? rhh_cache.find(pr_number) : pret_cache.find(pr_number);
    PRData data;
    if(cached_data == rhh_cache.end() || cached_data == pret_cache.end()){
        promise<json> promise;
        future<json> future = promise.get_future();
        fetch_pr_data(bot, GITHUB_TOKEN, pr_number, repo, [&promise,&bot](const dpp::http_request_completion_t & cc){
            if(cc.status != 200) {
                promise.set_value(json());
                return 1;
            }
            promise.set_value(json::parse(cc.body));
            return 0;
        });
        json raw_data = future.get();
        if(raw_data.is_null()) return;

        string label = (repo==PRET ? "pret" : (string)"") + "#" + to_string(pr_number) + " - " + (string)raw_data["title"];
        label = (label.length() > 80 ? label.substr(0, 77)+"..." : label);

        data = {
            .pr_number = pr_number,
            .html_url = (string)raw_data["html_url"]
        };
        strcpy(data.label, label.c_str());
        switch (repo) {
            case RHH: rhh_cache.emplace(data.pr_number, data);
            case PRET: pret_cache.emplace(data.pr_number, data);
        }
    } else {
        data = cached_data->second;
    }
    msg->add_component(generate_pr_button(repo, data));
}

dpp::component generate_pr_button(Repo repo, PRData data){
    return dpp::component().add_component(
        dpp::component()
            .set_type(dpp::component_type::cot_button)
            .set_label(data.label)
            .set_style(dpp::component_style::cos_link)
            .set_url(data.html_url)
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
