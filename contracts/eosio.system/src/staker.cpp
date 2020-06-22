#include <eosio.system/eosio.system.hpp>

#include <eosio.token/eosio.token.hpp>

namespace eosiosystem {

   static constexpr eosio::symbol _stake_symbol    = eosio::symbol(eosio::symbol_code("CRU"), 0);
   static constexpr eosio::symbol _emit_symbol     = eosio::symbol(eosio::symbol_code("FLO"), 4);
   static constexpr uint64_t usecs_in_sec = 1000000;
   static constexpr uint64_t _total_segments    = 1000000000;
   
   static constexpr uint64_t emission_step_usec_period  = 10 * int64_t(usecs_in_sec);//30 * 24 * 3600 * int64_t(usecs_in_sec);
   static constexpr uint64_t total_cliffs_in_period  =  emission_step_usec_period / usecs_block_period;

   
   void system_contract::activate(const time_point_sec activate_at){
      require_auth(_self);

      auto time = seconds(activate_at.sec_since_epoch());
      _gstate.thresh_activated_stake_time = time_point{ microseconds{time}} ;

      _gstate4.total_stakers_balance = asset(0, _stake_symbol);
      _gstate4.stakers_bucket = asset(0, core_symbol());
      
      _global4.set( _gstate4, _self );

   }


   int64_t system_contract::get_current_emission_step(time_point last_update){
      auto usecs_since_activate = (last_update - _gstate.thresh_activated_stake_time).count();
      int64_t step = usecs_since_activate / emission_step_usec_period;

      return step;
   }


   time_point system_contract::get_left_time_border(time_point last_update){
      int64_t limit = get_current_emission_step(last_update);
      const auto time = static_cast<int64_t>(limit * emission_step_usec_period);
      time_point left_time_border = _gstate.thresh_activated_stake_time + time_point{ microseconds{time}};

      return left_time_border;
   }


   time_point system_contract::get_right_time_border(time_point last_update){
      int64_t limit = get_current_emission_step(last_update) + 1;
      const auto time = static_cast<int64_t>(limit * emission_step_usec_period );

      time_point limited_right_time_border = _gstate.thresh_activated_stake_time + time_point{ microseconds{time}};

      if (current_time_point() >= limited_right_time_border){
         
         return limited_right_time_border;
      }
      else {
         return current_time_point();
      }

   }
      
   

   uint64_t system_contract::get_emission_rate(time_point time_border){
      uint64_t current_step = get_current_emission_step(time_border);
      
      if      ( current_step == 0 ){
         return 400000;
      }
      else if ( current_step == 1 ){
         return 380000;
      }
      else if ( current_step == 2 ){
         return 370000;
      }
      else if ( current_step == 3 ){
         return 340000;
      }
      else if ( current_step == 4 ){
         return 320000;
      }
      else if ( current_step == 5 ){
         return 300000;
      }
      else if ( current_step  > 5 ){
         return 240000;
      };

      return 0;
      
   }

   void system_contract::refresh(const eosio::name username){
      require_auth(username); 
      
      stakers_index stakers_instance(_self, _self.value);

      auto ct = current_time_point();

      check((_gstate.thresh_activated_stake_time != time_point{ microseconds{0}})
         || (_gstate.thresh_activated_stake_time <= ct), "cannot refresh rewards until chain is activated" );

      auto st = stakers_instance.find(username.value);

      if (st != stakers_instance.end()){
         
         check(st->staked_balance.amount > 0, "Nothing is not staked for refresh");

         time_point right_time_border = get_right_time_border(st->last_update_at);
         time_point left_time_border = get_left_time_border(st->last_update_at);
         uint64_t emission_rate = get_emission_rate(left_time_border);
         print("emission_rate:", emission_rate, ";");

         auto to_producers     = emission_rate / 20; //5%
         auto to_stakers       = emission_rate - to_producers; //95%
         

         auto total_emission_in_period = asset(to_stakers * total_cliffs_in_period, _emit_symbol);
         print("total_emission_in_period:", total_emission_in_period, ";");

         auto user_last_position = st->last_update_at > left_time_border ? st->last_update_at : left_time_border;
         
         auto user_cliffs_in_period = ((right_time_border - user_last_position)).count() / usecs_block_period;
         print("user_cliffs_in_period:", user_cliffs_in_period, ";");

         auto user_share_in_segments = st->staked_balance.amount / _gstate4.total_stakers_balance.amount * _total_segments;
         
         double user_emission_in_period_in_segments = (double)user_cliffs_in_period / (double)total_cliffs_in_period * (double)user_share_in_segments * (double)total_emission_in_period.amount;

         asset user_emission_in_period = asset((uint64_t)user_emission_in_period_in_segments / _total_segments, _emit_symbol);
         
         print("stakers_bucket_now:", _gstate4.stakers_bucket, ";");

         print("user_emission_in_period:", user_emission_in_period, ";");


         stakers_instance.modify(st, username, [&](auto &s){
            s.last_update_at = right_time_border;
            s.emitted_segments += user_emission_in_period_in_segments;
            s.emitted_balance += user_emission_in_period;
         });     
         
         check(user_emission_in_period <= _gstate4.stakers_bucket, "System error");

         _gstate4.stakers_bucket -= user_emission_in_period;
         print("stakers_bucket_updated:", _gstate4.stakers_bucket, ";");
      
      }
   }

   void system_contract::stake(const eosio::name username, const eosio::asset quantity){
      require_auth(username);
      stakers_index stakers_instance(_self, _self.value);
      
      eosio::check(quantity.symbol == _stake_symbol, "Wrong token symbol for staking");  
      
      const asset token_supply = eosio::token::get_balance(token_account, username, _stake_symbol.code() );

      eosio::check(token_supply >= quantity, "Not enought balance for stake");

      system_contract::refresh(username);

      auto st = stakers_instance.find(username.value);
      
      check(st -> last_update_at == current_time_point(), "Impossible to stake before full refresh balance.");

      

      if (st == stakers_instance.end()){
         stakers_instance.emplace(_self, [&](auto &s){
            s.username = username;
            s.last_update_at = current_time_point();
            s.staked_balance = quantity;
            s.emitted_segments = 0;
            s.emitted_balance = asset(0, _emit_symbol);
         });
      } else {
         stakers_instance.modify(st ,_self, [&](auto &s){
            s.staked_balance += quantity;
         });
      }

      INLINE_ACTION_SENDER(eosio::token, transfer)(
         token_account, { {username, active_permission} },
         { username, stake_account, quantity, std::string("stake it!") }
      );
      
      _gstate4.total_stakers_balance += quantity;
      

   }


   void system_contract::unstake(const eosio::name username, const eosio::asset quantity){
      require_auth(username);

      system_contract::refresh(username);

      stakers_index stakers_instance(_self, _self.value);

      auto st = stakers_instance.find(username.value);

      check(quantity.symbol == _stake_symbol, "Wrong stake token");

      check(st != stakers_instance.end(), "Username is not found");

      check((st -> staked_balance).amount > 0, "Nothing to unstake");

      check(st -> last_update_at == current_time_point(), "Impossible unstake not refreshed balance. Refresh balance first and try again.");

      stakers_instance.modify(st, username, [&](auto &s){
         s.staked_balance -= quantity;
      });
  
      _gstate4.total_stakers_balance -= quantity;

      INLINE_ACTION_SENDER(eosio::token, transfer)(
         token_account, { {stake_account, active_permission} },
         { stake_account, username, quantity, std::string("unstake it!") }
      );
      
   }

   void system_contract::getreward(const eosio::name username){
      require_auth(username);

      stakers_index stakers_instance(_self, _self.value);
      
      auto st = stakers_instance.find(username.value);

      check(st != stakers_instance.end(), "Username is not found");

      check((st -> emitted_balance).amount > 0, "Nothing to withdraw");

      asset on_withdraw = st -> emitted_balance;
      
      double segments = on_withdraw.amount * _total_segments;

      stakers_instance.modify(st, username, [&](auto &s){
         s.emitted_balance = asset(0, _emit_symbol);
         s.emitted_segments -= segments;
      });
  
    
      INLINE_ACTION_SENDER(eosio::token, transfer)(
         token_account, { {saving_account, active_permission} },
         { saving_account, username, on_withdraw, std::string("withdraw it!") }
      );
      
   }


} //namespace eosiosystem
