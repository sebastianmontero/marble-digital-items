#include "../include/marble.hpp"

//marble core
#include "./core/config.cpp"
#include "./core/groups.cpp"
#include "./core/behaviors.cpp"
#include "./core/items.cpp"

//marble layers
#include "./layers/tags.cpp"
#include "./layers/attributes.cpp"
#include "./layers/events.cpp"
#include "./layers/frames.cpp"
// #include "./layers/bonds.cpp"
// #include "./layers/wallets.cpp"


void marble::setsetting(const eosio::name &setter, const string &key, const FlexValue &value)
  {
    setSetting(setter, key, value);
  }

  void marble::appndsetting(const eosio::name &setter, const string &key, const FlexValue &value)
  {
    Setting s(get_self());
    s.addToSetting(setter, key, value);
  }

  void marble::clipsetting(const eosio::name &setter, const string &key, const FlexValue &value)
  {
    Setting s(get_self());
    s.clipFromSetting(setter, key, value);
  }

  void marble::erasesetting(const eosio::name &setter, const string &key)
  {
    Setting s(get_self());
    s.eraseSetting(setter, key);
  }

  void marble::setSetting(const eosio::name &setter, const string &key, const FlexValue &value)
    {
      Setting s(get_self());
      std::vector<FlexValue> settings{};
      settings.push_back(value);
      s.setSetting(setter, key, settings);
    }