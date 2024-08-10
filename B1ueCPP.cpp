/*
B1ue bot in CPP.

(C) 2024 - B1ue-Dev
*/

#include <dpp/dpp.h>
#include <dpp/message.h>
#include <dpp/appcommand.h>
#include <map>
#include <string>
#include <vector>
#include <tuple>

const std::string TOKEN = "";

std::string joinList(std::vector<std::string> arr) {
    if (arr.empty()) return "";
    std::string str;
    for (auto i : arr)
        str += i;
    return str;
}

/*
For rarity command, which include 
*/

/*
The struct of RarityData, which consists of
cards: A vector<int> of card value.
rings: A vector<int> or ring value.
exps: A vector<int> or exp value.
*/
struct RarityData {
    std::vector<int> cards;
    std::vector<int> rings;
    std::vector<int> exps;
    int _total_cards;
};

/*
A map of color hex for rarity color.
*/
std::map<std::string, std::pair<std::string, int>> RarityColor = {
    {"Common", {"<:CommonCard:1064631202310533320>", 0x96B1CA}},
    {"Rare", {"<:RareCard:1064631265753567384>", 0xF9AB00}},
    {"Super Rare", {"<:SuperRareCard:1064631338218573954>", 0x8A32FB}},
    {"Special", {"<:SpecialCard:1064631403007975424>", 0x12AD01}},
    {"Challenger", {"<:ChallengerCard:1064631495341391903>", 0xC92828}}
};

/*
std::string rarity The rarity of the character.
int current_level The current level of the character.
int card The current card number of the character.
*/
std::tuple<int, int, std::string, int, int> get_max(std::string rarity, int current_level, int card) {
    std::map<std::string, RarityData> rarity_dict = {
        {"Common",
            {
                {90, 30, 50, 90, 140, 200, 300, 450, 650, 900, 1300, 1700, 2250, 2950, 3700, 4600},
                {0, 100, 400, 900, 1600, 2500, 3600, 5000, 6600, 8600, 11300, 18600, 22000, 23300, 28000, 33300},
                {0, 10, 20, 30, 40, 50, 70, 90, 110, 130, 150, 170, 200, 240, 280, 320}
            }
        },
        {"Rare",
            {
                {60, 10, 20, 40, 70, 120, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100},
                {0, 200, 900, 1800, 3700, 6000, 9000, 12700, 18700, 26200, 33700, 41200, 48700, 60000, 75000, 90000},
                {0, 20, 40, 60, 80, 100, 140, 180, 220, 260, 300, 340, 400, 480, 560, 640}
            }
        },
        {"Super Rare",
            {
                {30, 6, 8, 12, 20, 40, 60, 80, 100, 130, 160, 200, 240, 280, 330, 400},
                {0, 400, 2500, 5000, 9000, 16000, 24000, 32000, 50000, 70000, 85000, 100000, 130000, 160000, 200000, 240000},
                {0, 40, 80, 120, 160, 200, 280, 360, 440, 520, 600, 680, 800, 960, 1120, 1280}
            }
        },
        {"Special",
            {
                {300, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 350, 400, 450, 500, 600},
                {0, 1000, 5000, 15000, 30000, 60000, 100000, 150000, 200000, 250000, 300000, 400000, 500000, 600000, 800000, 1000000},
                {0, 40, 80, 120, 160, 200, 280, 360, 440, 520, 600, 680, 800, 960, 1120, 1280}
            }
        },
        {"Challenger",
            {
                {500, 20, 50, 100, 170, 250, 350, 500, 700, 1000, 1400, 1900, 2500, 3200, 4000, 5000},
                {0, 500, 2500, 8000, 16000, 32000, 50000, 80000, 120000, 150000, 180000, 240000, 300000, 400000, 550000, 750000},
                {0, 50, 100, 150, 200, 250, 350, 450, 550, 650, 750, 900, 1050, 1200, 1350, 1600}
            }
        }
    };

    std::vector<int> cards = rarity_dict[rarity].cards;
    std::vector<int> rings = rarity_dict[rarity].rings;
    std::vector<int> exps = rarity_dict[rarity].exps;

    std::vector<int> levels = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int i = find(levels.begin(), levels.end(), current_level) - levels.begin();

    int left_card = card;
    int level = current_level;
    int total_rings = 0;
    int total_exps = 0;

    while (left_card >= cards[i]) {
        left_card -= cards[i];
        total_rings += rings[i];
        total_exps += exps[i];
        if (level == 15) {
            level = 16;
            break;
        } else {
            level++;
            i++;
        }
    }

    return std::make_tuple(level, left_card, level != 16 ? " / " + std::to_string(cards[level]) : "", total_rings, total_exps);
}

std::tuple<int, int, int> get_reached(std::string rarity, int current_level, int card, int aimed_level) {
    std::map<std::string, RarityData> rarity_dict = {
        {"Common",
            {
                {0, 30, 50, 90, 140, 200, 300, 450, 650, 900, 1300, 1700, 2250, 2950, 3700, 4600, 0},
                {0, 100, 400, 900, 1600, 2500, 3600, 5000, 6600, 8600, 11300, 18600, 22000, 23300, 28000, 33300, 0},
                {0, 10, 20, 30, 40, 50, 70, 90, 110, 130, 150, 170, 200, 240, 280, 320, 0},
                90
            }
        },
        {"Rare",
            {
                {60, 10, 20, 40, 70, 120, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100},
                {0, 200, 900, 1800, 3700, 6000, 9000, 12700, 18700, 26200, 33700, 41200, 48700, 60000, 75000, 90000},
                {0, 20, 40, 60, 80, 100, 140, 180, 220, 260, 300, 340, 400, 480, 560, 640},
                60
            }
        },
        {"Super Rare",
            {
                {30, 6, 8, 12, 20, 40, 60, 80, 100, 130, 160, 200, 240, 280, 330, 400},
                {0, 400, 2500, 5000, 9000, 16000, 24000, 32000, 50000, 70000, 85000, 100000, 130000, 160000, 200000, 240000},
                {0, 40, 80, 120, 160, 200, 280, 360, 440, 520, 600, 680, 800, 960, 1120, 1280},
                30
            }
        },
        {"Special",
            {
                {300, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 350, 400, 450, 500, 600},
                {0, 1000, 5000, 15000, 30000, 60000, 100000, 150000, 200000, 250000, 300000, 400000, 500000, 600000, 800000, 1000000},
                {0, 40, 80, 120, 160, 200, 280, 360, 440, 520, 600, 680, 800, 960, 1120, 1280},
                300
            }
        },
        {"Challenger",
            {
                {500, 20, 50, 100, 170, 250, 350, 500, 700, 1000, 1400, 1900, 2500, 3200, 4000, 5000},
                {0, 500, 2500, 8000, 16000, 32000, 50000, 80000, 120000, 150000, 180000, 240000, 300000, 400000, 550000, 750000},
                {0, 50, 100, 150, 200, 250, 350, 450, 550, 650, 750, 900, 1050, 1200, 1350, 1600},
                500
            }
        }
    };

    std::vector<int> cards = rarity_dict[rarity].cards;
    std::vector<int> rings = rarity_dict[rarity].rings;
    std::vector<int> exps = rarity_dict[rarity].exps;
    int _total_cards = rarity_dict[rarity]._total_cards;

    std::vector<int> levels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    int level = current_level;
    int i = find(levels.begin(), levels.end(), current_level == 0 ? current_level + 1 : current_level) - levels.begin();
    int aimed_level_index = find(levels.begin(), levels.end(), aimed_level) - levels.begin() + 1;
    int total_cards = level != 0 ? 0 : _total_cards;
    int total_exps = 0;
    int total_rings = 0;

    for (; i < (level == 1 ? aimed_level_index : aimed_level_index + 1); ++i) {
        total_cards += cards[i];
        total_rings += rings[i];
        total_exps += exps[i];
    }

    total_cards -= card;

    return std::make_tuple(total_cards, total_rings, total_exps);

}

std::string natural_rings(int rings) {
    int magnitude = 0;
    double rings_double = static_cast<double>(rings);

    while (std::abs(rings_double) >= 1000) {
        magnitude++;
        rings_double /= 1000.0;
    }

    char magnitude_suffix = '\0';
    if (magnitude == 1) {
        magnitude_suffix = 'K';
    }
    else if (magnitude == 2) {
        magnitude_suffix = 'M';
    }

    std::ostringstream result_stream;
    if (rings_double != 0) {
        result_stream << std::fixed;
        result_stream.precision(1);
        result_stream << rings_double;
        std::string result = result_stream.str();

        // Remove trailing zeros after the decimal point
        while (!result.empty() && (result.back() == '0' || result.back() == '.')) {
            result.pop_back();
        }

        result += magnitude_suffix;
        return result;
    }
    else {
        return "0";
    }
}

int main() {
    // Instantiating bot
    dpp::cluster bot(TOKEN);

    // Libraries output
    bot.on_log(dpp::utility::cout_logger());

    // On slashcommand event
    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        // /ping command.
        if (event.command.get_command_name() == "ping") {
            std::ostringstream ping;
            ping << std::fixed << std::setprecision(0) << bot.rest_ping * 1000;
            event.reply(":ping_pong: Pong! " + ping.str() + "ms");
        }

        // /about command.
        if (event.command.get_command_name() == "about") {
            std::string s = "Maintained by @b1uedev (IGN: ARK Blue)";
            dpp::embed embed = dpp::embed()
                .set_title("About B1ue")
                .set_description("The All-in-one Sonic Forces: Speed Battle Discord bot. Check your current character level, how many rings are need, look up for images, sprites, B1ue has you covered.")
                .set_color(0x4192C7)
                .set_footer(
                    dpp::embed_footer()
                    .set_text(s)
                );

            dpp::message msg(event.command.channel_id, embed);
            msg.add_component(
                dpp::component().add_component(
                    dpp::component()
                    .set_label("Invite Me")
                    .set_type(dpp::cot_button)
                    .set_style(dpp::cos_link)
                    .set_url("https://discord.com/api/oauth2/authorize?client_id=947339220823994388&permissions=8&scope=bot%20applications.commands")
                )
                .add_component(
                    dpp::component()
                    .set_label("Top.gg")
                    .set_type(dpp::cot_button)
                    .set_style(dpp::cos_link)
                    .set_url("https://top.gg/bot/947339220823994388")
                )
                .add_component(
                    dpp::component()
                    .set_label("Support Server")
                    .set_type(dpp::cot_button)
                    .set_style(dpp::cos_link)
                    .set_url("https://discord.gg/mE967ub6Ct")
                )
            );
            event.reply(msg);
        }

        // /special command.
        if (event.command.get_command_name() == "special") {
            std::int64_t current_level = std::get<std::int64_t>(event.get_parameter("current_level"));
            std::int64_t aimed_level = std::get<std::int64_t>(event.get_parameter("aimed_level"));
            std::int64_t card = std::get<std::int64_t>(event.get_parameter("card"));
            if (current_level == 16) {
                event.reply(dpp::message("Your character has already reached the maximum level.").set_flags(dpp::m_ephemeral));
                return;
            }
            else if (current_level > aimed_level || current_level == aimed_level) {
                event.reply(dpp::message("Your aimed level cannot be lower/equal to your current level.").set_flags(dpp::m_ephemeral));
                return;
            }

            std::string rarityName = "Special"; // Replace with the desired rarity
            auto it = RarityColor.find(rarityName);

            std::string cardEmoji = it->second.first;
            int hexColor = it->second.second;
            std::string goldRing = "<:GoldRing:1064628961931440198> : ";
            std::string Exp = "<:Exp:1064630336610381855> : ";

            std::tuple a = get_max("Special", current_level, card);
            std::tuple b = get_reached("Special", current_level, card, aimed_level);

            std::string val1 = joinList(
                {
                    "<:Upgrade:1064630801469276170> : ", std::to_string(current_level), " -> ", std::to_string(std::get<0>(a)), "\n" +
                    cardEmoji, " : ", std::get<2>(a) != "" ? std::to_string(std::get<1>(a)) + std::get<2>(a) : "Maximum Level Reached", "\n" +
                    goldRing, natural_rings(std::get<3>(a)) , "\n" +
                    Exp, std::to_string(std::get<4>(a))
                }
            );
            dpp::embed embed = dpp::embed()
                .set_title("Rarity: Special")
                .set_color(hexColor)
                .add_field(
                    "",
                    val1,
                    true
                );
            if (std::get<0>(b)) {
                std::string val2 = joinList(
                    {
                        "<:Upgrade:1064630801469276170> : ", std::to_string(current_level), " -> ", std::to_string(aimed_level), "\n" +
                        cardEmoji, " : ", std::to_string(std::get<0>(b)), "\n" +
                        goldRing, natural_rings(std::get<1>(b)) , "\n" +
                        Exp, std::to_string(std::get<2>(b))
                    }
                );
                embed.add_field(
                    "",
                    val2,
                    true
                );
            }
            dpp::message msg(event.command.channel_id, embed);
            event.reply(msg);
        }
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        // /ping command
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Pong!", bot.me.id));
        }
        // /about command
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("about", "About B1ue bot", bot.me.id));
        }
        // /special command
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("special", "Calculate the level your Special character can get.", bot.me.id)
                .add_option(
                    dpp::command_option(dpp::co_integer, "current_level", "The current level of your character.", true)
                    .set_min_value(0)
                    .set_max_value(16)
                )
                .add_option(
                    dpp::command_option(dpp::co_integer, "card", "The current amount of card for that character.", true)
                )
                .add_option(
                    dpp::command_option(dpp::co_integer, "aimed_level", "The level you are aimed for.", false)
                    .set_min_value(0)
                    .set_max_value(16)
                )
                .add_option(
                    dpp::command_option(dpp::co_string, "character", "The character", false)
                    .set_auto_complete(true)
                )
            );
        }
    });

	bot.start(dpp::st_wait);

	return 0;
}