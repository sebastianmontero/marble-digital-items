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

ACTION marble::reset(const uint64_t limit) {
    eosio::require_auth(get_self());

    uint64_t count = 0;
    groups_table groups_t(get_self(), get_self().value);
    auto groups_itr = groups_t.begin();
    while(groups_itr != groups_t.end() && count < limit){
        groups_itr = groups_t.erase(groups_itr);
        count++;
    }

    frames_table frames_t(get_self(), get_self().value);
    auto frames_itr = frames_t.begin();
    while(frames_itr != frames_t.end() && count < limit){
        frames_itr = frames_t.erase(frames_itr);
        count++;
    }

    items_table items_t(get_self(), get_self().value);
    auto items_itr = items_t.begin();
    while(items_itr != items_t.end() && count < limit){
        attributes_table attributes_t(get_self(), items_itr->serial);    
        auto attributes_itr = attributes_t.begin();
        while(attributes_itr != attributes_t.end() && count < limit){
            attributes_itr = attributes_t.erase(attributes_itr);
            count++;
        }
        if (count < limit) {
          items_itr = items_t.erase(items_itr);  
        }
    }

}