#pragma once
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>
#include <eosio/crypto.hpp>

#include <string>

using eosio::name;
using std::string;

namespace common
{

    typedef std::variant<std::monostate, eosio::name, std::string,
                         eosio::asset, eosio::time_point,
                         //  eosio::microseconds,
                         int64_t,
                         uint32_t,
                         //  uint64_t,
                         eosio::checksum256>
        FlexValue;

    static const string SETTING_BENNYFI_CONTRACT = "BENNYFI_CONTRACT";

    const static std::uint64_t toUint64(const std::string &fingerprint)
    {
        uint64_t id = 0;
        eosio::checksum256 h = eosio::sha256(const_cast<char *>(fingerprint.c_str()), fingerprint.size());
        auto hbytes = h.extract_as_byte_array();
        for (int i = 0; i < 4; i++)
        {
            id <<= 8;
            id |= hbytes[i];
        }
        return id;
    }

    static eosio::time_point currentTime()
    {
        return eosio::current_block_time().to_time_point();
    }

}
