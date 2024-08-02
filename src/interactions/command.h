#include <dpp/appcommand.h>
#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class command{
    public:
        virtual dpp::slashcommand get_command(dpp::cluster * bot)=0;
        static void execute(const dpp::interaction_create_t &event);
};
