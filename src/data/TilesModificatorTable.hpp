/*
 * Illarionserver - server for the game Illarion
 * Copyright 2011 Illarion e.V.
 *
 * This file is part of Illarionserver.
 *
 * Illarionserver  is  free  software:  you can redistribute it and/or modify it
 * under the terms of the  GNU Affero General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * Illarionserver is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY;  without  even  the  implied  warranty  of  MERCHANTABILITY  or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Affero General Public License along with
 * Illarionserver. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TILES_MODIFICATOR_TABLE_HPP
#define TILES_MODIFICATOR_TABLE_HPP

#include "TableStructs.hpp"
#include "data/StructTable.hpp"
#include "types.hpp"

class TilesModificatorTable : public StructTable<TYPE_OF_ITEM_ID, TilesModificatorStruct> {
public:
    auto getTableName() const -> std::string override;
    auto getColumnNames() -> std::vector<std::string> override;
    auto assignId(const Database::ResultTuple &row) -> TYPE_OF_ITEM_ID override;
    auto assignTable(const Database::ResultTuple &row) -> TilesModificatorStruct override;
    auto passable(TYPE_OF_ITEM_ID id) -> bool;
};

#endif
