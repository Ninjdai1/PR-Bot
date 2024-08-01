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
#include <map>
#include <string>
#include <vector>
#include <string>
#include "utils/github.h"
#include "utils/regex.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

map<string, string> read_config(string config_path);
string string_format( const std::string& format, ...);
string get_env_var( string const & key );

string GITHUB_TOKEN;

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
        if(reply_message.components.size() > 0) event.reply(reply_message);
    });

    bot.on_message_update([&bot](const dpp::message_update_t& event) {
        if(event.msg.author.is_bot()) return;

        dpp::message reply_message = generate_pr_message(&bot, GITHUB_TOKEN, event.msg.content);
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
