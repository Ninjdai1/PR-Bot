# RHH Bot (C++ edition)

## Some context
Rewrite of PR Bot in c++ as a fun, educative project

Uses [D++](https://github.com/brainboxdotcc/DPP) and [nlohmann/json](https://github.com/nlohmann/json)

## Purpose
PR Bot is a simple utility discord bot for the RHH Discord server. It automatically links pull requests and issues when someone tags one (#1234). Supports multiple links in a single message and escapes formatted links and code blocks

## Compilation instructions
You'll first need to [install D++](https://dpp.dev/installing.html)

Then, you can build the bot with the following sequence:
```bash
mkdir build/
cd build/
cmake ..
cd ..
cmake --build build/ -j$(nproc) # If you don't have nproc installed, you can replace this by -jyour-amount-of-cores
```
Alternatively, run `build.sh`, that does it for you
> [!CAUTION]
> While scripts are often very handy, never run blindly a script someone on the internet gave you. If you don't understand everything the script does, DO NOT run it

## Usage instructions
After the compilation is complete, you can fill your `config.cfg` following the `config.example.cfg` example config.
You can get a Github Personal Access Token at https://github.com/settings/tokens, that is needed to avoid rate-limits on the Github API (going from 60 to 15000 allowed requests per hour). This token is for the `github-token` config option
Create a Discord Bot at https://discord.com/developers/applications and use the token you get as the `discord-token`

Start the bot with `./build/prbot`
