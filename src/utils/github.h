#include <string>
#include <dpp/cluster.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

enum Repo {RHH, PRET};
struct PRData {
    int pr_number;
    char label[81];
    string html_url;
};

dpp::message generate_pr_message(dpp::cluster * bot, string GITHUB_TOKEN, string text);
void add_pr_button(dpp::cluster * bot, dpp::message * msg, string GITHUB_TOKEN, int pr_number, Repo repo);
dpp::component generate_pr_button(Repo repo, PRData data);

string get_pr_url(int pr_number, Repo repo);
void fetch_pr_data(dpp::cluster * bot, string GITHUB_TOKEN, int pr_number, Repo repo, dpp::http_completion_event callback);
