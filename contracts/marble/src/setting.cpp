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

#include <setting.hpp>

namespace eosio
{
    
    Setting::Setting(eosio::name contract) : m_contract(contract) {}

    void Setting::setSetting(const eosio::name &setter, const string &key,
                             const std::vector<FlexValue> &values)
    {
        eosio::require_auth(getContract());
        setting_table s_t(getContract(), getContract().value);
        auto s_itr = s_t.find(toUint64(key));

        if (s_itr != s_t.end())
        {
            s_t.modify(s_itr, getContract(), [&](SettingRecord &s)
                       {
                s.values = values;
                s.updated_date = eosio::current_time_point(); });
        }
        else
        {
            s_t.emplace(getContract(), [&](SettingRecord &s)
                        {
                s.id = toUint64(key);
                s.key = key;
                s.values = values; });
        }
    }

    void Setting::addToSetting(const eosio::name &setter, const string &key, const FlexValue &value)
    {
        eosio::require_auth(getContract());

        setting_table s_t(getContract(), getContract().value);
        auto s_itr = s_t.find(toUint64(key));

        if (s_itr != s_t.end())
        {
            s_t.modify(s_itr, getContract(), [&](SettingRecord &s)
                       {
                s.values.push_back(value);
                s.updated_date = eosio::current_time_point(); });
        }
        else
        {
            s_t.emplace(getContract(), [&](SettingRecord &s)
                        {
                s.id = toUint64(key);
                s.key = key;
                s.values = std::vector<FlexValue>{};
                s.values.push_back(value); });
        }
    }

    void Setting::clipFromSetting(const eosio::name &setter, const string &key, const FlexValue &value)
    {
        eosio::require_auth(getContract());

        setting_table s_t(getContract(), getContract().value);
        auto s_itr = s_t.find(toUint64(key));
        eosio::check(s_itr != s_t.end(), "key does not exist in settings: " + key);

        std::vector<FlexValue> existingValues = s_itr->values;

        for (std::size_t i = 0; i < existingValues.size(); ++i)
        {
            if (existingValues[i] == value)
            {
                existingValues.erase(existingValues.begin() + i);
            }
        }

        s_t.modify(s_itr, getContract(), [&](SettingRecord &s)
                   {
            s.values = existingValues;
            s.updated_date = eosio::current_time_point(); });
    }

    void Setting::eraseSetting(const eosio::name &setter, const string &key)
    {
        eosio::require_auth(getContract());

        setting_table s_t(getContract(), getContract().value);
        auto s_itr = s_t.find(toUint64(key));
        eosio::check(s_itr != s_t.end(), "key does not exist in settings: " + key);
        s_t.erase(s_itr);
    }

    std::vector<FlexValue> Setting::getSetting(const string &key)
    {
        setting_table s_t(getContract(), getContract().value);
        auto s_itr = s_t.find(toUint64(key));
        eosio::check(s_itr != s_t.end(), "key does not exist in settings: " + key);
        return s_itr->values;
    }

    FlexValue Setting::getFirstValue(const string &key)
    {
        setting_table s_t(getContract(), getContract().value);
        auto s_itr = s_t.find(toUint64(key));
        eosio::check(s_itr != s_t.end(), "key does not exist in settings: " + key);
        eosio::check(s_itr->values.size() > 0, "setting for key has length of zero: " + key);
        return s_itr->values.at(0);
    }

    void Setting::reset(){
        eosio::require_auth(getContract());
        setting_table s_t(getContract(), getContract().value);
        auto s_itr = s_t.begin();
        while(s_itr != s_t.end()){
            s_itr = s_t.erase(s_itr);
        }
    }
}