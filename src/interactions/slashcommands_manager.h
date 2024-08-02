#include <dpp/appcommand.h>
#include <dpp/cluster.h>
#include <dpp/dispatcher.h>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

using command_callback = void (*)(const dpp::interaction_create_t &event);

class slashcommands_manager{
private:
    dpp::cluster * bot;

public:
    slashcommands_manager(dpp::cluster * bot_client);
    std::map<std::string, std::pair<dpp::slashcommand, command_callback>> commands;
    void register_slash_commands();
    void register_slash_command(dpp::slashcommand command, const command_callback& callback);
    void on_interaction_create(const dpp::interaction_create_t &event);
};
