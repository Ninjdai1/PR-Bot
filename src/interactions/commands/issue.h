#include <dpp/appcommand.h>
#include <dpp/cluster.h>
#include "../command.h"

class Issue: public command{
    public:
        virtual dpp::slashcommand get_command(dpp::cluster * bot);
        static void execute(const dpp::interaction_create_t &event);
};
