//layer name: items
//required: config, groups, behaviors

//======================== item actions ========================

//mint a new item
//auth: manager
ACTION mintitem(name to, name group_name);

//transfer ownership of one or more items
//auth: owner
ACTION transferitem(name from, name to, vector<uint64_t> serials, string memo);

//activate an item
//auth: owner
ACTION activateitem(uint64_t serial);

//reclaim an item from the owner
//auth: manager
ACTION reclaimitem(uint64_t serial);

//consume an item
//post: inline releaseall() if bond(s) exist
//auth: owner
// ACTION consumeitem(uint64_t serial);

//destroy an item
//post: inline releaseall() if bond(s) exist
//auth: manager
// ACTION destroyitem(uint64_t serial, string memo);

//freeze an item to prevent transfer, activate, consume, reclaim, or destroy
//auth: manager
// ACTION freezeitem(uint64_t serial);

//unfreeze an item
//auth: manager
// ACTION unfreezeitem(uint64_t serial);

//======================== item tables ========================

//items table
//scope: self
//ram payer: manager
TABLE item {
    uint64_t serial;
    name group;
    name owner;
    //uint64_t mint; //edition?

    uint64_t primary_key() const { return serial; }
    uint64_t by_group() const { return group.value; }
    uint64_t by_owner() const { return owner.value; }
    uint128_t by_group_serial() const { return (uint128_t(group.value) << 64) + serial; }
    uint128_t by_owner_serial() const { return (uint128_t(owner.value) << 64) + serial; }

    EOSLIB_SERIALIZE(item, (serial)(group)(owner))
};
typedef multi_index<name("items"), item,
    indexed_by<"bygroup"_n, const_mem_fun<item, uint64_t, &item::by_group>>,
    indexed_by<"byowner"_n, const_mem_fun<item, uint64_t, &item::by_owner>>,
    indexed_by<"bygroupsrl"_n, const_mem_fun<item, uint128_t, &item::by_group_serial>>,
    indexed_by<"byownersrl"_n, const_mem_fun<item, uint128_t, &item::by_owner_serial>>
> items_table;
