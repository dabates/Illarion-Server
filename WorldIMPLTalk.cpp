//  illarionserver - server for the game Illarion
//  Copyright 2011 Illarion e.V.
//
//  This file is part of illarionserver.
//
//  illarionserver is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  illarionserver is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with illarionserver.  If not, see <http://www.gnu.org/licenses/>.


#include "World.hpp"
#include "data/NamesObjectTable.hpp"
#include "data/TilesTable.hpp"
#include "script/LuaItemScript.hpp"
#include "script/LuaLookAtItemScript.hpp"
#include "TableStructs.hpp"
#include "Player.hpp"
#include "netinterface/protocol/ServerCommands.hpp"
#include "Logger.hpp"

extern std::shared_ptr<LuaLookAtItemScript>lookAtItemScript;

bool World::sendTextInFileToPlayer(const std::string &filename, Player *cp) {
    if (filename.length() == 0) {
        return false;
    }

    FILE *fp;
    fp = fopen(filename.c_str(), "r");

    if (fp != NULL) {
        const unsigned char LINE_LENGTH = 255;
        char line[LINE_LENGTH];

        while (fgets(line, LINE_LENGTH, fp) != NULL) {
            ServerCommandPointer cmd(new SayTC(cp->pos.x, cp->pos.y, cp->pos.z, line));
            cp->Connection->addCommand(cmd);
        }

        fclose(fp);
        return true;
    } else {
        return false;
    }
}


void World::sendMessageToAdmin(const std::string &message) {
    for (const auto &player : Players) {
        if (player->hasGMRight(gmr_getgmcalls)) {
            ServerCommandPointer cmd(new SayTC(player->pos.x, player->pos.y, player->pos.z, message));
            player->Connection->addCommand(cmd);
        }
    }
}


std::string World::languagePrefix(int Language) {
    if (Language==0) {
        return "";
    } else if (Language==1) {
        return "[hum] ";
    } else if (Language==2) {
        return "[dwa] ";
    } else if (Language==3) {
        return "[elf] ";
    } else if (Language==4) {
        return "[liz] ";
    } else if (Language==5) {
        return "[orc] ";
    } else if (Language==6) {
        return "[hal] ";
    } else if (Language==7) {
        return "[fai] ";
    } else if (Language==8) {
        return "[gno] ";
    } else if (Language==9) {
        return "[gob] ";
    } else if (Language==10) {
        return "[anc] ";
    } else {
        return "";
    }
}

std::string World::languageNumberToSkillName(int languageNumber) {
    switch (languageNumber) {
    case 0:
        return "common language";

    case 1:
        return "human language";

    case 2:
        return "dwarf language";

    case 3:
        return "elf language";

    case 4:
        return "lizard language";

    case 5:
        return "orc language";

    case 6:
        return "halfling language";

    case 7:
        return "fairy language";

    case 8:
        return "gnome language";

    case 9:
        return "goblin language";

    case 10:
        return "ancient language";

    default:
        return "";

    }
}

void World::sendMessageToAllPlayers(const std::string &message) {
    for (const auto &player : Players) {
        player->inform(message, Player::informBroadcast);
    }
}

void World::sendMessageToAllCharsInRange(const std::string &german, const std::string &english, Character::talk_type tt, Character *cc) {
    uint16_t range = 0;

    // how far can we be heard?
    switch (tt) {
    case Character::tt_say:
        range = 14;
        break;

    case Character::tt_whisper:
        range = 2;
        break;

    case Character::tt_yell:
        range = 30;
        break;
    }

    std::string spokenMessage_german, spokenMessage_english, tempMessage;

    bool is_action = german.substr(0, 3) == "#me";

    if (!is_action) {
        // alter message because of the speakers inability to speak...
        spokenMessage_german = cc->alterSpokenMessage(german, cc->getLanguageSkill(cc->activeLanguage));
        spokenMessage_english = cc->alterSpokenMessage(english, cc->getLanguageSkill(cc->activeLanguage));
    }

    // tell all OTHER players... (but tell them what they understand due to their inability to do so)
    // tell the player himself what he wanted to say
    std::string prefix = languagePrefix(cc->activeLanguage);

    for (const auto &player : Players.findAllCharactersInRangeOf(cc->pos.x, cc->pos.y, cc->pos.z, range)) {
        if (!is_action && player->getId() != cc->getId()) {
            tempMessage = prefix + player->alterSpokenMessage(player->nls(spokenMessage_german, spokenMessage_english), player->getLanguageSkill(cc->activeLanguage));
            player->receiveText(tt, tempMessage, cc);
        } else {
            if (is_action) {
                player->receiveText(tt, player->nls(german, english), cc);
            } else {
                player->receiveText(tt, prefix + player->nls(german, english), cc);
            }
        }
    }

    if (cc->character == Character::player) {
        // tell all npcs
        for (const auto &npc : Npc.findAllCharactersInRangeOf(cc->pos.x, cc->pos.y, cc->pos.z, range)) {
            tempMessage=prefix + npc->alterSpokenMessage(english, npc->getLanguageSkill(cc->activeLanguage));
            npc->receiveText(tt, tempMessage, cc);
        }

        // tell all monsters
        for (const auto &monster : Monsters.findAllCharactersInRangeOf(cc->pos.x, cc->pos.y, cc->pos.z, range)) {
            monster->receiveText(tt, english, cc);
        }
    }
}

void World::sendLanguageMessageToAllCharsInRange(const std::string &message, Character::talk_type tt, Language lang, Character *cc) {
    uint16_t range = 0;

    // how far can we be heard?
    switch (tt) {
    case Character::tt_say:
        range = 14;
        break;

    case Character::tt_whisper:
        range = 2;
        break;

    case Character::tt_yell:
        range = 30;
        break;
    }

    //determine spoken language skill

    // get all Players
    std::vector<Player *> players = Players.findAllCharactersInRangeOf(cc->pos.x, cc->pos.y, cc->pos.z, range);
    // get all NPCs
    std::vector<NPC *> npcs = Npc.findAllCharactersInRangeOf(cc->pos.x, cc->pos.y, cc->pos.z, range);
    // get all Monsters
    std::vector<Monster *> monsters = Monsters.findAllCharactersInRangeOf(cc->pos.x, cc->pos.y, cc->pos.z, range);

    // alter message because of the speakers inability to speak...
    std::string spokenMessage,tempMessage;
    spokenMessage=cc->alterSpokenMessage(message,cc->getLanguageSkill(cc->activeLanguage));

    // tell all OTHER players... (but tell them what they understand due to their inability to do so)
    // tell the player himself what he wanted to say
    //std::cout << "message in WorldIMPLTalk:" << message;
    if (message.substr(0, 3) == "#me") {
        for (const auto &player : players) {
            if (player->getPlayerLanguage() == lang) {
                player->receiveText(tt, message, cc);
            }
        }
    } else {
        for (const auto &player : players) {
            if (player->getPlayerLanguage() == lang) {
                if (player->getId() != cc->getId()) {
                    tempMessage=languagePrefix(cc->activeLanguage) + player->alterSpokenMessage(spokenMessage, player->getLanguageSkill(cc->activeLanguage));
                    player->receiveText(tt, tempMessage, cc);
                } else {
                    player->receiveText(tt, languagePrefix(cc->activeLanguage)+message, cc);
                }
            }
        }
    }

    if (cc->character == Character::player) {
        // tell all npcs
        for (const auto &npc : npcs) {
            tempMessage=languagePrefix(cc->activeLanguage) + npc->alterSpokenMessage(spokenMessage, npc->getLanguageSkill(cc->activeLanguage));
            npc->receiveText(tt, tempMessage, cc);
        }

        // tell all monsters
        for (const auto &monster : monsters) {
            monster->receiveText(tt, message, cc);
        }
    }
}


void World::sendMessageToAllCharsInRange(const std::string &message, Character::talk_type tt, Character *cc) {
    sendMessageToAllCharsInRange(message, message, tt, cc);
}

void World::makeGFXForAllPlayersInRange(short int xc, short int yc, short int zc, int distancemetric ,unsigned short int gfx) {
    for (const auto &player : Players.findAllCharactersInRangeOf(xc, yc, zc, distancemetric)) {
        ServerCommandPointer cmd(new GraphicEffectTC(xc, yc, zc, gfx));
        player->Connection->addCommand(cmd);
    }
}


void World::makeSoundForAllPlayersInRange(short int xc, short int yc, short int zc, int distancemetric, unsigned short int sound) {
    for (const auto &player : Players.findAllCharactersInRangeOf(xc, yc, zc, distancemetric)) {
        ServerCommandPointer cmd(new SoundTC(xc, yc, zc, sound));
        player->Connection->addCommand(cmd);
    }
}



void World::lookAtMapItem(Player *cp, short int x, short int y, short int z) {

    Field *field;
    Item titem;

    if (GetPToCFieldAt(field, x, y, z)) {
        // Feld vorhanden
        if (field->ViewTopItem(titem)) {
            std::shared_ptr<LuaItemScript> script = Data::CommonItems.script(titem.getId());
            ScriptItem n_item = titem;
            n_item.type = ScriptItem::it_field;
            n_item.pos = position(x, y, z);
            n_item.owner = cp;

            if (script && script->existsEntrypoint("LookAtItem")) {
                script->LookAtItem(cp, n_item);
                return;
            }

            if (!lookAtItemScript->lookAtItem(cp, n_item)) {
                lookAtTile(cp, field->getTileId(), x, y, z);
            }
        } else {
            lookAtTile(cp, field->getTileId(), x, y, z);
        }
    }
}


void World::lookAtTile(Player *cp, unsigned short int tile, short int x, short int y, short int z) {
    const TilesStruct &tileStruct = Data::Tiles[tile];
    ServerCommandPointer cmd(new LookAtTileTC(x, y, z, cp->nls(tileStruct.German, tileStruct.English)));
    cp->Connection->addCommand(cmd);
}


void World::lookAtShowcaseItem(Player *cp, uint8_t showcase, unsigned char position) {

    ScriptItem titem;

    if (cp->isShowcaseOpen(showcase)) {
        Container *ps = cp->getShowcaseContainer(showcase);

        if (ps != NULL) {
            Container *tc;

            if (ps->viewItemNr(position, titem, tc)) {
                std::shared_ptr<LuaItemScript> script = Data::CommonItems.script(titem.getId());
                ScriptItem n_item = titem;

                n_item.type = ScriptItem::it_container;
                n_item.pos = cp->pos;
                n_item.owner = cp;
                n_item.itempos = position;
                n_item.inside = ps;

                if (script && script->existsEntrypoint("LookAtItem")) {
                    script->LookAtItem(cp, n_item);
                    return;
                }

                lookAtItemScript->lookAtItem(cp, n_item);
            }
        }
    }
}



void World::lookAtInventoryItem(Player *cp, unsigned char position) {
    if (cp->characterItems[ position ].getId() != 0) {

        Item titem = cp->characterItems[ position ];

        std::shared_ptr<LuaItemScript> script = Data::CommonItems.script(cp->characterItems[ position ].getId());
        ScriptItem n_item = cp->characterItems[ position ];

        if (position < MAX_BODY_ITEMS) {
            n_item.type = ScriptItem::it_inventory;
        } else {
            n_item.type = ScriptItem::it_belt;
        }

        n_item.itempos = position;
        n_item.pos = cp->pos;
        n_item.owner = cp;

        if (script && script->existsEntrypoint("LookAtItem")) {
            script->LookAtItem(cp, n_item);
            return;
        }

        lookAtItemScript->lookAtItem(cp, n_item);
    }
}

void World::forceIntroducePlayer(Player *cp, Player *Admin) {
    Admin->introducePlayer(cp);
}

void World::introduceMyself(Player *cp) {
    for (const auto &player : Players.findAllCharactersInRangeOf(cp->pos.x, cp->pos.y, cp->pos.z, 2)) {
        player->introducePlayer(cp);
    }
}

void World::sendWeather(Player *cp) {
    cp->sendWeather(weather);
}

void World::sendIGTime(Player *cp) {
    ServerCommandPointer cmd(new UpdateTimeTC(static_cast<unsigned char>(getTime("hour")),static_cast<unsigned char>(getTime("minute")),static_cast<unsigned char>(getTime("day")),static_cast<unsigned char>(getTime("month")), static_cast<short int>(getTime("year"))));
    cp->Connection->addCommand(cmd);
}

void World::sendIGTimeToAllPlayers() {
    for (const auto &player : Players) {
        sendIGTime(player);
    }
}

void World::sendWeatherToAllPlayers() {
    for (const auto &player : Players) {
        player->sendWeather(weather);
    }
}

