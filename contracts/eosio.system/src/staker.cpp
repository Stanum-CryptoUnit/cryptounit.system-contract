#include <eosio.system/eosio.system.hpp>

#include <eosio.token/eosio.token.hpp>

namespace eosiosystem {

   static constexpr uint64_t usecs_in_sec = 1000000;
   static constexpr uint64_t _total_segments    = 1000000000;
      

   uint64_t system_contract::get_emission_rate(uint64_t current_step){
      uint64_t emission_rate = 0;

      if      ( current_step == 0 ){
         emission_rate = 400000;
      }
      else if ( current_step == 1 ){
         emission_rate = 380000;
      }
      else if ( current_step == 2 ){
         emission_rate = 370000;
      }
      else if ( current_step == 3 ){
         emission_rate = 340000;
      }
      else if ( current_step == 4 ){
         emission_rate = 320000;
      }
      else if ( current_step == 5 ){
         emission_rate = 300000;
      }
      else if   (( current_step  > 5 )&&( current_step  <= 11 )) {
         emission_rate = 240000;
      } else if (( current_step  > 11 )&&( current_step  <= 23 )) {
         emission_rate = 180000;
      } else if (( current_step  > 23 )&&( current_step  <= 35 )) {
         emission_rate = 120000;
      } else if (( current_step  > 35 )&&( current_step  <= 47 )) {
         emission_rate = 100000;
      } else if (( current_step  > 47 )&&( current_step  <= 59 )) {
         emission_rate = 90000;
      } else if (( current_step  > 59 )&&( current_step  <= 71 )) {
         emission_rate = 80000;
      } else if (( current_step  > 71 )&&( current_step  <= 83 )) {
         emission_rate = 70000;
      } else if (( current_step  > 83 )&&( current_step  <= 91 )) {
         emission_rate = 60000;
      } else if (( current_step  > 91 )&&( current_step  <= 103 )) {
         emission_rate = 50000;
      } else if ( current_step  > 103 ) {
         emission_rate = 40000; //but not more then max_cap
      }

      return emission_rate;
      
   }

   uint64_t system_contract::get_next_emission_rate(uint64_t current_step) {
      uint64_t emission_rate = 0;

      if      ( current_step == 0 ){
         emission_rate = 380000;
      }
      else if ( current_step == 1 ){
         emission_rate = 370000;
      }
      else if ( current_step == 2 ){
         emission_rate = 340000;
      }
      else if ( current_step == 3 ){
         emission_rate = 320000;
      }
      else if ( current_step == 4 ){
         emission_rate = 300000;
      }
      else if ( current_step == 5 ){
         emission_rate = 240000;
      }
      else if   (( current_step  > 5 )&&( current_step  <= 11 )) {
         emission_rate = 180000;
      } else if (( current_step  > 11 )&&( current_step  <= 23 )) {
         emission_rate = 120000;
      } else if (( current_step  > 23 )&&( current_step  <= 35 )) {
         emission_rate = 100000;
      } else if (( current_step  > 35 )&&( current_step  <= 47 )) {
         emission_rate = 90000;
      } else if (( current_step  > 47 )&&( current_step  <= 59 )) {
         emission_rate = 80000;
      } else if (( current_step  > 59 )&&( current_step  <= 71 )) {
         emission_rate = 70000;
      } else if (( current_step  > 71 )&&( current_step  <= 83 )) {
         emission_rate = 60000;
      } else if (( current_step  > 83 )&&( current_step  <= 91 )) {
         emission_rate = 50000;
      } else if (( current_step  > 91 )&&( current_step  <= 103 )) {
         emission_rate = 40000;
      } else if ( current_step  > 103 ) {
         emission_rate = 0; 
      }

      return emission_rate;
   }

   time_point system_contract::get_next_step_date(uint64_t current_step) {
      time_point next_step_date;

      if      (( current_step >= 0 )&&( current_step <= 5 )){
         next_step_date = _gstate.thresh_activated_stake_time + microseconds(_gstate4.emission_step_in_usec * (current_step + 1));
      }
      else if   (( current_step  > 5 )&&( current_step  <= 11 )) {
         next_step_date = _gstate.thresh_activated_stake_time + microseconds(_gstate4.emission_step_in_usec * (6 + 12 - current_step));
      } else if (( current_step  > 11 )&&( current_step  <= 23 )) { 
         next_step_date = _gstate.thresh_activated_stake_time + microseconds(_gstate4.emission_step_in_usec * (12 + 24 - current_step));
      } else if (( current_step  > 23 )&&( current_step  <= 35 )) {
         next_step_date = _gstate.thresh_activated_stake_time + microseconds(_gstate4.emission_step_in_usec * (24 + 36 - current_step));
      } else if (( current_step  > 35 )&&( current_step  <= 47 )) {
         next_step_date = _gstate.thresh_activated_stake_time + microseconds(_gstate4.emission_step_in_usec * (36 + 48 - current_step));
      } else if (( current_step  > 47 )&&( current_step  <= 59 )) {
         next_step_date = _gstate.thresh_activated_stake_time + microseconds(_gstate4.emission_step_in_usec * (48 + 60 - current_step));
      } else if (( current_step  > 59 )&&( current_step  <= 71 )) {
         next_step_date = _gstate.thresh_activated_stake_time + microseconds(_gstate4.emission_step_in_usec * (60 + 72 - current_step));
      } else if (( current_step  > 71 )&&( current_step  <= 83 )) {
         next_step_date = _gstate.thresh_activated_stake_time + microseconds(_gstate4.emission_step_in_usec * (72 + 84 - current_step));
      } else if (( current_step  > 83 )&&( current_step  <= 91 )) {
         next_step_date = _gstate.thresh_activated_stake_time + microseconds(_gstate4.emission_step_in_usec * (84 + 92 - current_step));
      } else if (( current_step  > 91 )&&( current_step  <= 103 )) {
         next_step_date = _gstate.thresh_activated_stake_time + microseconds(_gstate4.emission_step_in_usec * (92 + 104 - current_step));
      } else if ( current_step  > 103 ) {
         next_step_date = time_point{microseconds{0}}; 
      }

      return next_step_date;
   }

   void system_contract::activate(const time_point_sec activate_at, const uint64_t emission_step_in_sec){
      require_auth(_self);
      
      auto time = seconds(activate_at.sec_since_epoch());
      _gstate.thresh_activated_stake_time = time_point{ microseconds{time}} ;

      _gstate4.total_stakers_balance = 0;
      _gstate4.total_stakers_cru_balance = asset(0, _cru_symbol);
      _gstate4.total_stakers_frozen_cru_balance = asset(0, _cru_symbol);
      _gstate4.total_stakers_wcru_balance = asset(0, _wcru_symbol);
      _gstate4.total_stakers_frozen_wcru_balance = asset(0, _wcru_symbol);

      _gstate4.stakers_bucket = asset(0, core_symbol());
      _gstate4.emission_step_in_usec = emission_step_in_sec * uint64_t(usecs_in_sec);
      _gstate4.total_cliffs_in_period = _gstate4.emission_step_in_usec / usecs_block_period;
      _gstate4.current_emission_rate = asset(0, core_symbol());
      
      _gstate4.next_emission_rate = asset(get_emission_rate(0), core_symbol());
      _gstate4.next_emission_step_start_at = _gstate.thresh_activated_stake_time;
      
      _global4.set( _gstate4, _self );

   }


   int64_t system_contract::get_current_emission_step(time_point last_update){
      auto usecs_since_activate = (last_update - _gstate.thresh_activated_stake_time).count();
      int64_t step = usecs_since_activate / _gstate4.emission_step_in_usec;

      return step;
   }


   time_point system_contract::get_left_time_border(time_point last_update){
      int64_t limit = get_current_emission_step(last_update);
      const auto time = static_cast<int64_t>(limit * _gstate4.emission_step_in_usec);
      time_point left_time_border = _gstate.thresh_activated_stake_time + time_point{ microseconds{time}};

      return left_time_border;
   }


   time_point system_contract::get_right_time_border(time_point last_update){
      int64_t limit = get_current_emission_step(last_update) + 1;
      const auto time = static_cast<int64_t>(limit * _gstate4.emission_step_in_usec );

      time_point limited_right_time_border = _gstate.thresh_activated_stake_time + time_point{ microseconds{time}};

      if (current_time_point() >= limited_right_time_border){
         
         return limited_right_time_border;
      }
      else {
         return current_time_point();
      }

   }
    

   void system_contract::refresh(const eosio::name username){
      // require_auth(username); 
      
      stakers_index stakers_instance(_self, _self.value);
      
      stakers2_index stakers2_instance(_self, _self.value);


      auto ct = current_time_point();

      check((_gstate.thresh_activated_stake_time != time_point{ microseconds{0}})
         || (_gstate.thresh_activated_stake_time <= ct), "cannot refresh rewards until chain is activated" );

      auto st = stakers_instance.find(username.value);
      time_point current_update_at = time_point{ microseconds{0}};

      auto st2 = stakers2_instance.find(username.value);
      
      if (st2 != stakers2_instance.end()) {
         current_update_at = st2 -> current_update_at;
      };

      if (st != stakers_instance.end()){
         if (current_update_at < ct){

            if (st2 == stakers2_instance.end()){
               stakers2_instance.emplace(_self, [&](auto &s2){
                  s2.username = username;
                  s2.current_update_at = ct;
               });
            } else {
               stakers2_instance.modify(st2, _self, [&](auto &s2){
                  s2.current_update_at = ct;
               });
            };

            // check(st->staked_balance.amount > 0, "Nothing is not staked for refresh");

            time_point right_time_border = get_right_time_border(st->last_update_at);
            time_point left_time_border = get_left_time_border(st->last_update_at);
            
            uint64_t current_step = get_current_emission_step(left_time_border);
            uint64_t next_step = current_step + 1;

            uint64_t emission_rate = get_emission_rate(current_step);
            print("emission_rate:", emission_rate, ";");

           
            auto to_producers     = emission_rate / 20; //5%
            auto to_stakers       = emission_rate - to_producers; //95%
            

            eosio::asset total_emission_in_period = asset(to_stakers * _gstate4.total_cliffs_in_period, _emit_symbol);
            print("total_emission_in_period:", total_emission_in_period, ";");

            auto user_last_position = st->last_update_at > left_time_border ? st->last_update_at : left_time_border;
            
            uint64_t user_cliffs_in_period = ((right_time_border - user_last_position)).count() / usecs_block_period;
            print("user_cliffs_in_period:", user_cliffs_in_period, ";");

            double user_share_in_segments = (double)st->staked_balance / (double)_gstate4.total_stakers_balance * (double)_total_segments;
            print("user_share_in_segments:", user_share_in_segments, ";");
            double user_emission_in_period_in_segments = (double)user_cliffs_in_period / (double)_gstate4.total_cliffs_in_period * (double)user_share_in_segments * (double)total_emission_in_period.amount;
            print("user_emission_in_period_in_segments:", user_emission_in_period_in_segments, ";");
            asset user_emission_in_period = asset((uint64_t)user_emission_in_period_in_segments / _total_segments, _emit_symbol);
            
            print("stakers_bucket_now:", _gstate4.stakers_bucket, ";");

            print("user_emission_in_period:", user_emission_in_period, ";");


            stakers_instance.modify(st, _self, [&](auto &s){
               s.last_update_at = right_time_border;
               s.emitted_segments += user_emission_in_period_in_segments;
               s.emitted_balance += user_emission_in_period;
            });     
            

            check(user_emission_in_period <= _gstate4.stakers_bucket, "System error");

            _gstate4.stakers_bucket -= user_emission_in_period;
            print("stakers_bucket_updated:", _gstate4.stakers_bucket, ";");
         }
      }
   }


   void system_contract::frunstake(const eosio::name username, const eosio::asset quantity) {
      require_auth(_tokenlock);

      system_contract::refresh(username);
      system_contract::refresh(username);

      stakers_index stakers_instance(_self, _self.value);

      auto st = stakers_instance.find(username.value);

      if (st != stakers_instance.end()){
         check(st -> staked_frozen_wcru_balance.amount >= quantity.amount, "Not enought frozen tokens for unstake");      
         check(st -> staked_balance >= quantity.amount, "Not enought tokens for unstake");     

         check(st -> last_update_at == current_time_point(), "Impossible unstake not refreshed balance. Refresh balance first and try again.");


         stakers_instance.modify(st, username, [&](auto &s){
            s.staked_balance -= quantity.amount;
            s.staked_wcru_balance -= quantity;
            s.staked_frozen_wcru_balance -= quantity;        
         });

     
         _gstate4.total_stakers_balance -= quantity.amount;
         _gstate4.total_stakers_frozen_wcru_balance -= quantity;
         _gstate4.total_stakers_wcru_balance -= quantity;   
      }
      
   }

   void system_contract::frstake(const eosio::name username, const eosio::asset quantity) {
      require_auth(_tokenlock);
      
      eosio::check(quantity.symbol == _wcru_symbol, "Wrong token symbol for staking");
      
      auto ct = current_time_point();
      
      system_contract::refresh(username);
      system_contract::refresh(username);

      stakers_index stakers_instance(_self, _self.value);
      
      auto st = stakers_instance.find(username.value);
      
      if (st == stakers_instance.end()){

         stakers_instance.emplace(_self, [&](auto &s){
            s.username = username;
            s.last_update_at = ct;
            s.staked_balance = quantity.amount;
            s.staked_frozen_wcru_balance = quantity;
            s.staked_cru_balance = asset(0, _cru_symbol);
            s.staked_frozen_cru_balance = asset(0, _cru_symbol);
            s.staked_wcru_balance = quantity;
            s.emitted_segments = 0;
            s.emitted_balance = asset(0, _emit_symbol);
         });
         
      } else {

         check(st -> last_update_at == ct, "Impossible to stake before full refresh balance.");

         stakers_instance.modify(st ,_self, [&](auto &s){
            s.staked_balance += quantity.amount;
            s.staked_wcru_balance += quantity;
            s.staked_frozen_wcru_balance += quantity;
         });
      }

      _gstate4.total_stakers_balance += quantity.amount;
      _gstate4.total_stakers_frozen_wcru_balance += quantity;
      _gstate4.total_stakers_wcru_balance += quantity;


 }

   void system_contract::stake(const eosio::name username, const eosio::asset quantity) {
      require_auth(username);
      
      asset token_supply;

      if (quantity.symbol == _cru_symbol){
         token_supply = eosio::token::get_balance(token_account, username, _cru_symbol.code() );
      } else if (quantity.symbol == _wcru_symbol) {
         token_supply = eosio::token::get_balance(token_account, username, _wcru_symbol.code() );
      } else {
         eosio::check(false, "Wrong token symbol for staking");
      }
      
      auto ct = current_time_point();
      print("token_supply: ", token_supply);
      print("quantity: ", quantity);
      
      eosio::check(token_supply >= quantity, "Not enought balance for stake");

      system_contract::refresh(username);
      system_contract::refresh(username);

      stakers_index stakers_instance(_self, _self.value);
      
      auto st = stakers_instance.find(username.value);
      
      
      if (st == stakers_instance.end()){
         stakers_instance.emplace(_self, [&](auto &s){
            s.username = username;
            s.last_update_at = ct;
            s.staked_balance = quantity.amount;
            s.staked_frozen_wcru_balance = asset(0, _wcru_symbol);

            if (quantity.symbol == _cru_symbol){
               s.staked_cru_balance = quantity;
               s.staked_wcru_balance = asset(0, _wcru_symbol);
            } else {
               s.staked_cru_balance = asset(0, _cru_symbol);
               s.staked_wcru_balance = quantity;
            }

            s.emitted_segments = 0;
            s.emitted_balance = asset(0, _emit_symbol);
         });
      } else {

         check(st -> last_update_at == ct, "Impossible to stake before full refresh balance.");

         stakers_instance.modify(st ,_self, [&](auto &s){
            s.staked_balance += quantity.amount;

            quantity.symbol == _cru_symbol ? s.staked_cru_balance += quantity : s.staked_wcru_balance += quantity;
               
         });
      }

      INLINE_ACTION_SENDER(eosio::token, transfer)(
         token_account, { {username, active_permission} },
         { username, stake_account, quantity, std::string("stake it!") }
      );
   


      action(
       permission_level{_self,"active"_n},
       _tokenlock,
       name("chlbal"),
       std::make_tuple(username, quantity, uint64_t(0))
     ).send();

      action(
       permission_level{_self,"active"_n},
       _tokenlock,
       name("chlbal"),
       std::make_tuple(username, quantity, uint64_t(2))
     ).send();


      _gstate4.total_stakers_balance += quantity.amount;

      quantity.symbol == _cru_symbol ? _gstate4.total_stakers_cru_balance += quantity : _gstate4.total_stakers_wcru_balance += quantity;


   }

   void system_contract::frwithdraw(const eosio::name username){
      require_auth(username);

      stakers3_index stakers3_instance(_self, _self.value);
      auto staker = stakers3_instance.find(username.value);
      
      //3 * useconds_per_day

      if (staker != stakers3_instance.end() && staker->last_update_at + microseconds(20 * usecs_block_period) < current_time_point()) {
         if (staker -> cru_on_widthdraw.amount > 0 || staker -> wcru_on_widthdraw.amount > 0){
            
            if (staker -> cru_on_widthdraw.amount > 0) {
               INLINE_ACTION_SENDER(eosio::token, transfer)(
                  token_account, { {stake_account, active_permission} },
                  { stake_account, username, staker -> cru_on_widthdraw, std::string("unstake it!") }
               );

              action(
                permission_level{_self,"active"_n},
                _tokenlock,
                name("chlbal"),
                std::make_tuple(username, - staker -> cru_on_widthdraw, uint64_t(0))
              ).send();
            };

            if (staker -> wcru_on_widthdraw.amount > 0) {
               INLINE_ACTION_SENDER(eosio::token, transfer)(
                  token_account, { {stake_account, active_permission} },
                  { stake_account, username, staker -> wcru_on_widthdraw, std::string("unstake it!") }
               );

              action(
                permission_level{_self,"active"_n},
                _tokenlock,
                name("chlbal"),
                std::make_tuple(username, - staker -> wcru_on_widthdraw, uint64_t(0))
              ).send();

            };

            stakers3_instance.modify(staker, username, [&](auto &s){
               s.last_update_at = current_time_point();
               s.cru_on_widthdraw -= staker -> cru_on_widthdraw;
               s.wcru_on_widthdraw -= staker -> wcru_on_widthdraw;
            });
         }
      }

   }
   
   void system_contract::unstake(const eosio::name username, const eosio::asset quantity){
      require_auth(username);

      system_contract::refresh(username);

      stakers_index stakers_instance(_self, _self.value);

      auto st = stakers_instance.find(username.value);

      check(st != stakers_instance.end(), "Username is not found");

      if (quantity.symbol == _cru_symbol){
         check((st -> staked_cru_balance).amount >= quantity.amount, "Not enought tokens for unstake");
      } else if (quantity.symbol == _wcru_symbol) {
         check((st -> staked_wcru_balance).amount >= quantity.amount, "Not enought tokens for unstake");
      } else {
         check(false, "Wrong stake token symbol");
      }
      
      check(st -> staked_balance >= quantity.amount, "Not enought tokens for unstake");     
      

      check(st -> last_update_at == current_time_point(), "Impossible unstake not refreshed balance. Refresh balance first and try again.");

      stakers_instance.modify(st, username, [&](auto &s){

         s.staked_balance -= quantity.amount;
         
         quantity.symbol == _cru_symbol ? s.staked_cru_balance -= quantity : s.staked_wcru_balance -= quantity;
        
      });
  
      _gstate4.total_stakers_balance -= quantity.amount;
      quantity.symbol == _cru_symbol ? _gstate4.total_stakers_cru_balance -= quantity : _gstate4.total_stakers_wcru_balance -= quantity;
      

      
     //FREEZE tokens on withdraw
      stakers3_index stakers3_instance(_self, _self.value);
      auto staker = stakers3_instance.find(username.value);
      if (staker == stakers3_instance.end()){
         stakers3_instance.emplace(username, [&](auto &s){
            s.username = username;
            s.last_update_at = current_time_point();
            if (quantity.symbol == _cru_symbol){
               s.cru_on_widthdraw = quantity;
               s.wcru_on_widthdraw = asset(0, _wcru_symbol);
            } else if (quantity.symbol == _wcru_symbol){
               s.cru_on_widthdraw = asset(0, _cru_symbol);
               s.wcru_on_widthdraw = quantity;
            } else {
               eosio::check(false, "Only CRU or WCRU symbols can be unstaked");
            };
         });
      } else {
         stakers3_instance.modify(staker, username, [&](auto &s){
            s.last_update_at = current_time_point();
            if (quantity.symbol == _cru_symbol){
               s.cru_on_widthdraw += quantity;
            } else if (quantity.symbol == _wcru_symbol){
               s.wcru_on_widthdraw += quantity;
            } else {
               eosio::check(false, "Only CRU or WCRU symbols can be unstaked");
            };
         });
      }

     action(
       permission_level{_self,"active"_n},
       _tokenlock,
       name("chlbal"),
       std::make_tuple(username, - quantity, uint64_t(2))
     ).send();

   }

   void system_contract::getreward(const eosio::name username, const eosio::asset to_withdraw){
      require_auth(username);

      stakers_index stakers_instance(_self, _self.value);
      
      auto st = stakers_instance.find(username.value);

      check(st != stakers_instance.end(), "Username is not found");

      check((st -> emitted_balance).amount > 0, "Nothing to withdraw");
      check(to_withdraw.symbol == _emit_symbol, "Wrong symbol for get reward");
      asset available_withdraw = st -> emitted_balance;
      
      check(available_withdraw >= to_withdraw, "Not enought emitted balance for withdraw");

      
      double segments = to_withdraw.amount * _total_segments;

      stakers_instance.modify(st, username, [&](auto &s){
         s.emitted_balance = available_withdraw - to_withdraw;
         s.emitted_segments -= segments;
      });
  

      INLINE_ACTION_SENDER(eosio::token, transfer)(
         token_account, { {saving_account, active_permission} },
         { saving_account, username, to_withdraw, std::string("withdraw it!") }
      );
      
   }


} //namespace eosiosystem
