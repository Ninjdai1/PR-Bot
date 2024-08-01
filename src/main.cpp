#include <cstdio>
#include <dpp/cluster.h>
#include <dpp/colors.h>
#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
#include <dpp/httpsclient.h>
#include <dpp/message.h>
#include <dpp/misc-enum.h>
#include <dpp/once.h>
#include <dpp/queues.h>
#include <dpp/snowflake.h>
#include <map>
#include <string>
#include <vector>
#include <string>
#include "utils/github.h"
#include "utils/regex.h"
#include "utils/misc.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

string get_env_var( string const & key );

string GITHUB_TOKEN;
// Snowflake: Received message id, Message: Replied message
map<dpp::snowflake, dpp::message> message_cache;

int main (int argc, char *argv[]) {
    map<string, string> config = read_config("config.cfg");
    GITHUB_TOKEN = config.at("github-token");

    dpp::cluster bot(config.at("discord-token"), dpp::i_default_intents | dpp::i_message_content);
    
    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&bot](const dpp::ready_t& event){
        bot.log(dpp::ll_info, "Ready !");
    });
    

    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        if(event.msg.author.is_bot()) return;

        dpp::message reply_message = generate_pr_message(&bot, GITHUB_TOKEN, event.msg.content);
        if(reply_message.components.size() > 0) {
            event.reply(reply_message, false, [&bot, event](const dpp::confirmation_callback_t& callback) {
                if(callback.is_error()) bot.log(dpp::ll_error, callback.get_error().human_readable);
                message_cache.emplace(event.msg.id, callback.get<dpp::message>());
            });
        }
    });

    bot.on_message_update([&bot](const dpp::message_update_t& event) {
        if(event.msg.author.is_bot()) return;

        auto cached_message_iter = message_cache.find(event.msg.id);
        if(cached_message_iter == message_cache.end()) return;

        dpp::message reply_message = generate_pr_message(&bot, GITHUB_TOKEN, event.msg.content);
        if(reply_message.components.size()==0){
            bot.message_delete(cached_message_iter->second.id, cached_message_iter->second.channel_id);
            message_cache.erase(cached_message_iter->first);
            return;
        }
        cached_message_iter->second.components.swap(reply_message.components);
        bot.message_edit(cached_message_iter->second);
    });

    bot.on_message_delete([](const dpp::message_delete_t& event){
        message_cache.erase(event.id);
    });

    bot.start(dpp::st_wait);
}

string get_env_var( string const & key ) {                                 
    char * val;                                                                        
    val = getenv( key.c_str() );                                                       
    string retval = "";                                                           
    if (val != NULL) {                                                                 
        retval = val;                                                                    
    }                                                                                  
    return retval;                                                                        
}
