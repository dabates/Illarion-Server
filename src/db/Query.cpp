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

#include "db/Query.hpp"

#include "db/ConnectionManager.hpp"

#include <stdexcept>

using namespace Database;

Query::Query() : dbConnection(ConnectionManager::getInstance().getConnection()) {}

Query::Query(PConnection connection) : dbConnection(std::move(connection)) {}

Query::Query(const std::string &query) : Query(ConnectionManager::getInstance().getConnection(), query) {}

Query::Query(PConnection connection, std::string query)
        : dbConnection(std::move(connection)), dbQuery(std::move(query)) {}

auto Query::execute() -> Result {
    if (!dbConnection || dbQuery.empty()) {
        throw std::domain_error("Connection and query string are required to execute the query.");
    }

    bool ownTransaction = !dbConnection->transactionActive();

    if (ownTransaction) {
        dbConnection->beginTransaction();
    }

    auto result = dbConnection->query(dbQuery);

    if (ownTransaction) {
        dbConnection->commitTransaction();
    }

    return result;
}

void Query::setQuery(const std::string &query) { dbQuery = query; }

auto Query::getConnection() -> PConnection { return dbConnection; }

auto Query::escapeKey(const std::string &key) -> std::string {
    if (!key.empty() && key.at(0) == '"' && key.at(key.length() - 1) == '"') {
        return key;
    }

    return "\"" + key + "\"";
}

auto Query::escapeAndChainKeys(const std::string &key1, const std::string &key2) -> std::string {
    if (!key1.empty() && !key2.empty()) {
        return escapeKey(key1) + "." + escapeKey(key2);
    }
    if (key1.empty()) {
        return escapeKey(key2);
    }
    if (key2.empty()) {
        return escapeKey(key1);
    }

    return "";
}

void Query::appendToStringList(std::string &list, const std::string &newEntry) {
    if (!list.empty()) {
        list += ", ";
    }

    list += newEntry;
}
