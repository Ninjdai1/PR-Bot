#include "slashcommands_manager.h"
#include <dpp/appcommand.h>
#include <dpp/cluster.h>
#include <dpp/misc-enum.h>
#include "commands/issue.h"
#include <utility>

slashcommands_manager::slashcommands_manager(dpp::cluster * bot_client){
    bot = bot_client;
}

void slashcommands_manager::register_slash_commands(){
    Issue issue = Issue();
    register_slash_command(issue.get_command(bot), issue.execute);
}

void slashcommands_manager::register_slash_command(dpp::slashcommand command, const command_callback& callback){
    commands.emplace(command.name, std::make_pair(command, callback));
    bot->global_command_create(command);
}

void slashcommands_manager::on_interaction_create(const dpp::interaction_create_t &event){
    auto command_iter = commands.find(event.command.get_command_name());
    if(command_iter==commands.end()){
        bot->log(dpp::ll_warning, "Unknown command " + event.command.get_command_name());
    } else {
        command_iter->second.second(event);
    }
}
