/*
  Copyright 2014 Statoil ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdexcept>
#include <iostream>
#include <boost/filesystem.hpp>

#define BOOST_TEST_MODULE ScheduleTests
#include <boost/test/unit_test.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <opm/parser/eclipse/EclipseState/Util/CompressedVector.hpp>
#include <opm/parser/eclipse/EclipseState/Util/OrderedMap.hpp>
#include <opm/parser/eclipse/EclipseState/Util/IOrderSet.hpp>


BOOST_AUTO_TEST_CASE( check_empty) {
    Opm::OrderedMap<std::string, std::string> map;
    BOOST_CHECK_EQUAL( 0U , map.size() );
    BOOST_CHECK_THROW( map.iget( 0 ) , std::invalid_argument);
    BOOST_CHECK_THROW( map.get( "KEY" ) , std::invalid_argument);
    BOOST_CHECK_THROW( map.at("KEY"), std::invalid_argument);
    BOOST_CHECK_THROW( map.at(0), std::invalid_argument);
    BOOST_CHECK_EQUAL( map.count("NO_SUCH_KEY"), 0);
}

BOOST_AUTO_TEST_CASE( operator_square ) {
    Opm::OrderedMap<std::string, std::string> map;
    map.insert(std::make_pair("CKEY1" , "Value1"));
    map.insert(std::make_pair("BKEY2" , "Value2"));
    map.insert(std::make_pair("AKEY3" , "Value3"));

    const auto& value = map["CKEY1"];
    BOOST_CHECK_EQUAL( value, std::string("Value1"));
    BOOST_CHECK_EQUAL( map.size(), 3);

    auto& new_value = map["NEW_KEY"];
    BOOST_CHECK_EQUAL( map.size(), 4);
    BOOST_CHECK_EQUAL( new_value, "");
}

BOOST_AUTO_TEST_CASE( find ) {
    Opm::OrderedMap<std::string, std::string> map;
    map.insert(std::make_pair("CKEY1" , "Value1"));
    map.insert(std::make_pair("BKEY2" , "Value2"));
    map.insert(std::make_pair("AKEY3" , "Value3"));

    auto iter_end = map.find("NO_SUCH_KEY");
    BOOST_CHECK( iter_end == map.end());


    auto iter_found = map.find("CKEY1");
    BOOST_CHECK_EQUAL( iter_found->first, std::string("CKEY1"));
    BOOST_CHECK_EQUAL( iter_found->second, std::string("Value1"));
}

BOOST_AUTO_TEST_CASE( check_order ) {
    Opm::OrderedMap<std::string, std::string> map;
    map.insert(std::make_pair("CKEY1" , "Value1"));
    map.insert(std::make_pair("BKEY2" , "Value2"));
    map.insert(std::make_pair("AKEY3" , "Value3"));

    BOOST_CHECK_EQUAL( 3U , map.size() );
    BOOST_CHECK( map.count("CKEY1") > 0);
    BOOST_CHECK( map.count("BKEY2") > 0);
    BOOST_CHECK( map.count("AKEY3") > 0);

    BOOST_CHECK_EQUAL( "Value1" , map.get("CKEY1"));
    BOOST_CHECK_EQUAL( "Value1" , map.iget( 0 ));
    BOOST_CHECK_EQUAL( map.count("CKEY"), 0);

    BOOST_CHECK_EQUAL( "Value2" , map.get("BKEY2"));
    BOOST_CHECK_EQUAL( "Value2" , map.iget( 1 ));

    BOOST_CHECK_EQUAL( "Value3" , map.at("AKEY3"));
    BOOST_CHECK_EQUAL( "Value3" , map.at( 2 ));

    map.insert( std::make_pair("CKEY1" , "NewValue1"));
    BOOST_CHECK_EQUAL( "NewValue1" , map.get("CKEY1"));
    BOOST_CHECK_EQUAL( "NewValue1" , map.iget( 0 ));

    {
        std::vector<std::string> values;
        for (auto iter = map.begin(); iter != map.end(); ++iter)
            values.push_back( iter->second );

        BOOST_CHECK_EQUAL( values[0] , "NewValue1");
        BOOST_CHECK_EQUAL( values[1] , "Value2");
        BOOST_CHECK_EQUAL( values[2] , "Value3");
    }

    BOOST_CHECK_EQUAL(map.erase("NO_SUCH_KEY"), 0);
    BOOST_CHECK_EQUAL(map.erase("BKEY2"), 1);
    /*
    BOOST_CHECK_EQUAL( "NewValue1" , map.get("CKEY1"));
    BOOST_CHECK_EQUAL( "NewValue1" , map.iget( 0 ));
    BOOST_CHECK_EQUAL( map.count("CKEY"), 0);

    BOOST_CHECK_EQUAL( "Value3" , map.at("AKEY3"));
    BOOST_CHECK_EQUAL( "Value3" , map.at( 1 ));
    */
}

BOOST_AUTO_TEST_CASE(test_IOrderSet) {
    Opm::IOrderSet<std::string> iset;
    BOOST_CHECK(iset.empty());
    BOOST_CHECK_EQUAL(iset.size(), 0);
    BOOST_CHECK_EQUAL(iset.count("HEI"), 0);
    BOOST_CHECK_EQUAL(iset.contains("HEI"), false);

    BOOST_CHECK(iset.insert("HEI"));
    BOOST_CHECK_EQUAL(iset.size(), 1);
    BOOST_CHECK_EQUAL(iset.count("HEI"), 1);
    BOOST_CHECK_EQUAL(iset.contains("HEI"), true);

    BOOST_CHECK(!iset.insert("HEI"));
    BOOST_CHECK_EQUAL(iset.size(), 1);
    BOOST_CHECK_EQUAL(iset.count("HEI"), 1);
    BOOST_CHECK_EQUAL(iset.contains("HEI"), true);

    BOOST_CHECK_THROW(iset[10], std::out_of_range);


    Opm::IOrderSet<int> iset2;


    for (int i=10; i >= 0; i--)
        iset2.insert(i);

    int expected = 10;
    std::size_t index=0;
    const auto &d = iset2.data();
    for (const auto &v : iset2) {
        BOOST_CHECK_EQUAL(v, expected);
        BOOST_CHECK_EQUAL(iset2[index], expected);
        BOOST_CHECK_EQUAL(d[index], expected);
        expected--;
        index++;
    }


    Opm::IOrderSet<std::string> iset3;
    iset3.insert("AAA");
    iset3.insert("BBB");

    BOOST_CHECK_EQUAL(iset3[0], "AAA");
    BOOST_CHECK_EQUAL(iset3[1], "BBB");

    BOOST_CHECK_EQUAL(iset3.erase("AAA"), 1);
    BOOST_CHECK_EQUAL(iset3.size() , 1);
    BOOST_CHECK_EQUAL(iset3[0], "BBB");

    BOOST_CHECK_EQUAL(iset3.erase("AAA"), 0);
    BOOST_CHECK_EQUAL(iset3.size() , 1);
    BOOST_CHECK_EQUAL(iset3[0], "BBB");
}




BOOST_AUTO_TEST_CASE(CompVectorTest) {
    Opm::CompressedVector<int> v(1000);
    BOOST_CHECK_EQUAL(v.size(), 1000);
    std::vector<int> small(10,0);
    std::vector<int> expected(1000,0);
    auto data = v.data();
    BOOST_CHECK(data == expected);
    BOOST_CHECK_THROW(v.assign(small), std::invalid_argument);


    std::vector<int> new_v(1000);
    for (int i=0; i < 10; i++)
        std::fill_n(new_v.begin() + i*100, 100, i + 1);
    v.assign(new_v);
    auto new_data = v.data();
    BOOST_CHECK(new_data == new_v);
}
