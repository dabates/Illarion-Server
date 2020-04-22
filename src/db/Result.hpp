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

#ifndef _RESULT_HPP_
#define _RESULT_HPP_

#include <pqxx/result.hxx>
#include <pqxx/result_iterator.hxx>

namespace Database {
/* This file contains just some namespaces that hide the pqxx implementation
 * of the SQL Query result handling.
 */
typedef pqxx::result Result;
typedef pqxx::row ResultTuple;
typedef pqxx::field ResultField;
typedef Result *PResult;
}

#endif // _RESULT_HPP_
