// The MIT License (MIT)

// Copyright (c) 2020, Digital Scarcity

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>
#include <eosio/asset.hpp>
#include <string>
#include <common.hpp>


using common::FlexValue;
using std::string;
using std::vector;
using common::toUint64;

namespace eosio
{
    class Setting
    {
    public:
        Setting(eosio::name contract);

        void setSetting(const eosio::name &setter, const string &key, const std::vector<FlexValue> &values);
        void addToSetting(const eosio::name &setter, const string &key, const FlexValue &value);
        void clipFromSetting(const eosio::name &setter, const string &key, const FlexValue &value);
        void eraseSetting(const eosio::name &setter, const string &key);
        std::vector<FlexValue> getSetting(const string &key);
        FlexValue getFirstValue(const string &key);
        void reset();

        // Can return reference to stored type
        template <class T>
        inline decltype(auto) getValueAs(FlexValue *fv)
        {
            eosio::check(std::holds_alternative<T>(fv), "Content value is not of expected type");
            return std::get<T>(fv);
        }

        // Can return reference to stored type
        template <class T>
        inline decltype(auto) getFirstValueAs(const std::string &key)
        {
            FlexValue fv = getFirstValue(key);
            eosio::check(std::holds_alternative<T>(fv), "Content value is not of expected type for key: " + key);
            return std::get<T>(fv);
        }

        eosio::name m_contract; // should not get persisted (?)
        eosio::name getContract() { return m_contract; }

        struct [[eosio::table, eosio::contract("marble")]] SettingRecord
        {
            uint64_t id = toUint64(key);
            std::string key;
            std::vector<FlexValue> values;
            eosio::time_point created_date = eosio::current_time_point();
            eosio::time_point updated_date = eosio::current_time_point();

            uint64_t primary_key() const { return id; }

            EOSLIB_SERIALIZE(SettingRecord, (id)(key)(values)(created_date)(updated_date))
        };

        typedef eosio::multi_index<eosio::name("settings"), SettingRecord> setting_table;
    };
}