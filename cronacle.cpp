#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace eosio;

const std::string version_num = "0.2.1";

class [[eosio::contract("cronacle")]] cronacle : public eosio::contract {
public:

    cronacle(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

    [[eosio::action]]
    void version() {
      // name arnold = name("arnoldlayne");

      std::string version_msg = "Version: " + version_num; // + " arnold = " + std::to_string(arnold.value);
      check(false, version_msg);
    }

    [[eosio::action]]
    void storebtc(uint32_t btcprice) {
      time_point now = current_time_point();

      // open the btcprice table
      btcprice_index btcprice_table(get_self(), get_self().value);
      
      // add the btc price tick
      btcprice_table.emplace(get_self(), [&](auto &btc) { btc.ticktime = now; btc.usdprice = btcprice; });
    }

    using version_action = action_wrapper<"version"_n, &cronacle::version>;

    [[eosio::action]]
    void storeid(name user, std::string principal) {
      time_point now = current_time_point();

      // open the user table
      users_index users_table(get_self(), get_self().value);

      auto user_iterator = users_table.find(user.value);

      if (user_iterator != users_table.end()) {
        // modify
        users_table.modify(user_iterator, get_self(), [&](auto &usr) { usr.time = now; usr.dfinity_principal = principal; });
      } else {
        // emplace
        users_table.emplace(get_self(), [&](auto &usr) { usr.time = now; usr.proton_account = user; usr.dfinity_principal = principal; });
      }

      
    }

private:

  // btc table
  struct[[ eosio::table("btcprice"), eosio::contract("cronacle") ]] btctick {
    time_point  ticktime;
    uint32_t    usdprice;

    uint64_t primary_key() const { return ticktime.sec_since_epoch(); }
  };
  using btcprice_index = eosio::multi_index<"btcprice"_n, btctick>;

  // user table
  struct[[ eosio::table("users"), eosio::contract("cronacle") ]] user {
    time_point  time;
    name        proton_account;
    std::string dfinity_principal;

    uint64_t primary_key() const { return proton_account.value; }
  };
  using users_index = eosio::multi_index<"users"_n, user>;
};