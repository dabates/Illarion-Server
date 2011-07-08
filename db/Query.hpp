/*
 * Illarionserver - server for the game Illarion
 * Copyright 2011 Illarion e.V.
 *
 * This file is part of Illarionserver.
 *
 * Illarionserver  is  free  software:  you can redistribute it and/or modify it
 * under the terms of the  GNU  General  Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * Illarionserver is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY;  without  even  the  implied  warranty  of  MERCHANTABILITY  or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU  General Public License along with
 * Illarionserver. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _QUERY_HPP_
#define _QUERY_HPP_

#include <string>

#include <boost/shared_ptr.hpp>

#include "db/Connection.hpp"
#include "db/Result.hpp"

namespace Database {
class Query;

typedef Query *PQuery;

class Query {
private:
    const boost::shared_ptr<Connection> dbConnection;
    std::string dbQuery;

public:
    Query(const Query &org);
    Query(const std::string &query);
    Query(const boost::shared_ptr<Connection> connection, const std::string &query);
    virtual ~Query();

    virtual Result execute();

protected:
    Query();
    Query(const boost::shared_ptr<Connection> connection);

    void setQuery(const std::string &query);
    boost::shared_ptr<Connection> getConnection();
};
}

#endif // _QUERY_HPP_
