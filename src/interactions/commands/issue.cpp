#include "issue.h"
#include <dpp/appcommand.h>
#include <dpp/cluster.h>
#include <algorithm>
#include <dpp/message.h>
#include <string>

struct issue_templates{
    std::string issue_template;
    std::string labels;
};

std::unordered_map<std::string, issue_templates> templates
{
    {
        {"battle_engine", issue_templates{"01_battle_engine_bugs.yaml", "bug\%2Cstatus\%3A+unconfirmed\%2Ccategory\%3A+battle-mechanic"}},
        {"battle_ai", issue_templates{"02_battle_ai_issues.yaml", "bug\%2Cstatus\%3A+unconfirmed\%2Ccategory\%3A+battle-ai"}},
        {"feature_request", issue_templates{"03_feature_requests.yaml", "feature-request"}},
        {"other", issue_templates{"04_other_errors.yaml", "bug\%2Cstatus\%3A+unconfirmed"}},
    }
};

dpp::slashcommand Issue::get_command(dpp::cluster * bot){
    dpp::slashcommand issue("issue", "Create an issue", bot->me.id);
    issue.add_option(
        dpp::command_option(dpp::co_string, "type", "The type of issue", true)
            .add_choice(dpp::command_option_choice("⚔️ Battle Engine mechanical bugs 🐛", std::string("battle_engine")))
            .add_choice(dpp::command_option_choice("🧠 Battle AI bugs 🐛", std::string("battle_ai")))
            .add_choice(dpp::command_option_choice("🙏 Feature Request 🙏", std::string("feature_request")))
            .add_choice(dpp::command_option_choice("💾 Other errors 🖥️", std::string("other")))
    ).add_option(
        dpp::command_option(dpp::co_string, "title", "Quick description of the issue", true)
    );
    return issue;
}

void Issue::execute(const dpp::interaction_create_t &event){
    issue_templates itmp = templates.at(std::get<std::string>(event.get_parameter("type")));
    std::string title = std::get<std::string>(event.get_parameter("title"));
    std::string url_title = title;
    std::replace(url_title.begin(), url_title.end(), ' ', '+');

    std::string issue_url = std::string("https://github.com/rh-hideout/pokeemerald-expansion/issues/new?assignees=&labels=")
        + itmp.labels
        + "&projects=&template="
        + itmp.issue_template
        + "&contact="
        + event.command.usr.username
        + "&title="
        + url_title;

    dpp::message msg("To report your issue, click on the button below and fill out the issues form !");
    msg.add_component(dpp::component().add_component(
        dpp::component()
            .set_type(dpp::component_type::cot_button)
            .set_style(dpp::component_style::cos_link)
            .set_label((title.length() > 80 ? title.substr(0, 77)+"..." : title))
            .set_url(issue_url)
    ));

    event.reply(msg);
}
