/*
 *  illarionserver - server for the game Illarion
 *  Copyright 2011 Illarion e.V.
 *
 *  This file is part of illarionserver.
 *
 *  illarionserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  illarionserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with illarionserver.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "World.hpp"

#include "script/binding/binding.hpp"
#include "script/forwarder.hpp"

#include <luabind/luabind.hpp>
#include <luabind/out_value_policy.hpp>

namespace binding {

auto world() -> Binding<World> {
    return luabind::class_<World>("World")
            .def("LoS", &world_LuaLoS)
            .def("deleteNPC", &World::deleteNPC)
            .def("createDynamicNPC", &World::createDynamicNPC)
            .def("getPlayersOnline", &world_getPlayersOnline)
            .def("getPlayerIdByName", &World::getPlayerIdByName, luabind::pure_out_value(_3))
            .def("getNPCS", &world_getNPCS)
            .def("getCharactersInRangeOf", &world_getCharactersInRangeOf)
            .def("getPlayersInRangeOf", &world_getPlayersInRangeOf)
            .def("getMonstersInRangeOf", &world_getMonstersInRangeOf)
            .def("getNPCSInRangeOf", &world_getNPCSInRangeOf)
            .def("getArmorStruct", &World::getArmorStruct, luabind::pure_out_value(_3))
            .def("getWeaponStruct", &World::getWeaponStruct, luabind::pure_out_value(_3))
            .def("getNaturalArmor", &World::getNaturalArmor, luabind::pure_out_value(_3))
            .def("getMonsterAttack", &World::getMonsterAttack, luabind::pure_out_value(_3))
            .def("changeQuality", &World::changeQuality)
            .def("changeItem", &World::changeItem)
            .def("isCharacterOnField", &World::isCharacterOnField)
            .def("getCharacterOnField", &World::getCharacterOnField)
            .def("getField", &world_fieldAt)
            .def("makePersistentAt", &World::makePersistentAt)
            .def("removePersistenceAt", &World::removePersistenceAt)
            .def("isPersistentAt", &World::isPersistentAt)
            .def("getTime", &World::getTime)
            .def("erase", &World::erase)
            .def("increase", &World::increase)
            .def("swap", &World::swap)
            .def("createItemFromId", world_createFromId)
            .def("createItemFromItem", &World::createFromItem)
            .def("createMonster", &World::createMonster)
            .def("gfx", &World::gfx)
            .def("makeSound", &World::makeSound)
            .def("getItemStats", &World::getItemStats)
            .def("getItemStatsFromId", &World::getItemStatsFromId)
            .def("setWeather", &World::setWeather)
            .def("isItemOnField", &World::isItemOnField)
            .def("getItemOnField", &World::getItemOnField)
            .def("changeTile", &World::changeTile)
            .def("getItemName", &World::getItemName)
            .def("createSavedArea", &World::createSavedArea)
            .def("broadcast", &World::broadcast)
            .def("sendMonitoringMessage", &World::sendMonitoringMessage)
            .def_readwrite("weather", &World::weather);
}

} // namespace binding
