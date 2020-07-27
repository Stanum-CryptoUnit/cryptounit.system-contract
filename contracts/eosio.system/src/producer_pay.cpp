#include <eosio.system/eosio.system.hpp>

#include <eosio.token/eosio.token.hpp>

namespace eosiosystem {

   const int64_t  min_pervote_daily_pay = 100'0000;
   const int64_t  min_activated_stake   = 150'000'000'0000;
   const double   continuous_rate       = 0.04879;          // 5% annual rate
   const double   perblock_rate         = 0.0025;           // 0.25%
   const double   standby_rate          = 0.0075;           // 0.75%
   const uint32_t blocks_per_year       = 52*7*24*2*3600;   // half seconds per year
   const uint32_t seconds_per_year      = 52*7*24*3600;
   const uint32_t blocks_per_day        = 2 * 24 * 3600;
   const uint32_t blocks_per_hour       = 2 * 3600;
   const int64_t  useconds_per_day      = 24 * 3600 * int64_t(1000000);
   const int64_t  useconds_per_year     = seconds_per_year*1000000ll;
   const uint64_t usecs_block_period    = 500000; 

   void system_contract::onblock( ignore<block_header> ) {
      using namespace eosio;

      require_auth(_self);

      block_timestamp timestamp;
      name producer;
      _ds >> timestamp >> producer;

      // _gstate2.last_block_num is not used anywhere in the system contract code anymore.
      // Although this field is deprecated, we will continue updating it for now until the last_block_num field
      // is eventually completely removed, at which point this line can be removed.
      _gstate2.last_block_num = timestamp;

      if( (_gstate.thresh_activated_stake_time == time_point({ microseconds{0}}))  
         || (_gstate.thresh_activated_stake_time >= current_time_point() ))
      {
         return;
      } 

      if( _gstate.last_pervote_bucket_fill == time_point() )  /// start the presses
         _gstate.last_pervote_bucket_fill = current_time_point();


      emit_to_buckets();

      /**
       * At startup the initial producer may not be one that is registered / elected
       * and therefore there may be no producer object for them.
       */
      auto prod = _producers.find( producer.value );
      if ( prod != _producers.end() ) {
         _gstate.total_unpaid_blocks++;
         _producers.modify( prod, same_payer, [&](auto& p ) {
            p.unpaid_blocks++;
         });
      }

      /// only update block producers once every minute, block_timestamp is in half seconds
      if( timestamp.slot - _gstate.last_producer_schedule_update.slot > 120 ) {
         update_elected_producers( timestamp );

         if( (timestamp.slot - _gstate.last_name_close.slot) > blocks_per_day ) {
            name_bid_table bids(_self, _self.value);
            auto idx = bids.get_index<"highbid"_n>();
            auto highest = idx.lower_bound( std::numeric_limits<uint64_t>::max()/2 );
            if( highest != idx.end() &&
                highest->high_bid > 0 &&
                (current_time_point() - highest->last_bid_time) > microseconds(useconds_per_day) &&
                _gstate.thresh_activated_stake_time > time_point()
            ) {
               _gstate.last_name_close = timestamp;
               channel_namebid_to_rex( highest->high_bid );
               idx.modify( highest, same_payer, [&]( auto& b ){
                  b.high_bid = -b.high_bid;
               });
            }
         }
      }
   }

   using namespace eosio;


   void system_contract::emit_to_buckets(){
      auto ct = current_time_point();

      const asset token_supply   = eosio::token::get_supply(token_account, core_symbol().code() );
      const asset max_supply = eosio::token::get_max_supply(token_account, core_symbol().code() ); 

      const auto usecs_since_last_fill = (ct - _gstate.last_pervote_bucket_fill).count();
      
      if( usecs_since_last_fill > 0 ) {
         
         time_point ct_minus_block = time_point{microseconds{static_cast<int64_t>((ct - time_point{ microseconds{usecs_block_period}}).count())}};

         uint64_t current_step = get_current_emission_step(ct_minus_block);
         
         auto emission_rate = get_emission_rate(current_step); 
         

         
         if (_gstate4.current_emission_rate.amount != emission_rate){
            _gstate4.current_emission_rate = asset(emission_rate, core_symbol());
            _gstate4.next_emission_step_start_at = get_next_step_date(current_step);
            _gstate4.next_emission_rate = asset(get_next_emission_rate(current_step), core_symbol());
         }


         uint64_t new_tokens;

         if (token_supply.amount + emission_rate <= max_supply.amount){

            new_tokens = emission_rate;
         
         } else {

            new_tokens = max_supply.amount - token_supply.amount;
            _gstate4.current_emission_rate = asset(0, core_symbol());
            
         }

         if (new_tokens > 0){    
            auto to_producers     = new_tokens / 20;
            auto to_savings       = new_tokens - to_producers;
            auto to_per_block_pay = to_producers / 4;
            auto to_per_vote_pay  = to_producers - to_per_block_pay;

            INLINE_ACTION_SENDER(eosio::token, issue)(
               token_account, { {_self, active_permission} },
               { _self, asset(new_tokens, core_symbol()), std::string("issue tokens for producer pay and savings") }
            );

            INLINE_ACTION_SENDER(eosio::token, transfer)(
               token_account, { {_self, active_permission} },
               { _self, saving_account, asset(to_savings, core_symbol()), "unallocated inflation" }
            );

            INLINE_ACTION_SENDER(eosio::token, transfer)(
               token_account, { {_self, active_permission} },
               { _self, bpay_account, asset(to_per_block_pay, core_symbol()), "fund per-block bucket" }
            );

            INLINE_ACTION_SENDER(eosio::token, transfer)(
               token_account, { {_self, active_permission} },
               { _self, vpay_account, asset(to_per_vote_pay, core_symbol()), "fund per-vote bucket" }
            );
            
            _gstate.pervote_bucket          += to_per_vote_pay;
            _gstate.perblock_bucket         += to_per_block_pay;
            _gstate.last_pervote_bucket_fill = ct;

            _gstate4.stakers_bucket += asset(to_savings, core_symbol());
         }
      }
   }


   void system_contract::claimrewards( const name owner ) {
      require_auth( owner );

      const auto& prod = _producers.get( owner.value );
      check( prod.active(), "producer does not have an active key" );

      const auto ct = current_time_point();

      check((_gstate.thresh_activated_stake_time != time_point{ microseconds{0}})
         || (_gstate.thresh_activated_stake_time <= ct), "cannot claim rewards until chain is activated" );

      
      // check( ct - prod.last_claim_time > microseconds(useconds_per_day), "already claimed rewards within past day" );

      auto prod2 = _producers2.find( owner.value );


      /// New metric to be used in pervote pay calculation. Instead of vote weight ratio, we combine vote weight and
      /// time duration the vote weight has been held into one metric.
      const auto last_claim_plus_3days = prod.last_claim_time + microseconds(3 * useconds_per_day);

      bool crossed_threshold       = (last_claim_plus_3days <= ct);
      bool updated_after_threshold = true;
      if ( prod2 != _producers2.end() ) {
         updated_after_threshold = (last_claim_plus_3days <= prod2->last_votepay_share_update);
      } else {
         prod2 = _producers2.emplace( owner, [&]( producer_info2& info  ) {
            info.owner                     = owner;
            info.last_votepay_share_update = ct;
         });
      }

      // Note: updated_after_threshold implies cross_threshold (except if claiming rewards when the producers2 table row did not exist).
      // The exception leads to updated_after_threshold to be treated as true regardless of whether the threshold was crossed.
      // This is okay because in this case the producer will not get paid anything either way.
      // In fact it is desired behavior because the producers votes need to be counted in the global total_producer_votepay_share for the first time.

      int64_t producer_per_block_pay = 0;
      if( _gstate.total_unpaid_blocks > 0 ) {
         producer_per_block_pay = (_gstate.perblock_bucket * prod.unpaid_blocks) / _gstate.total_unpaid_blocks;
      }

      double new_votepay_share = update_producer_votepay_share( prod2,
                                    ct,
                                    updated_after_threshold ? 0.0 : prod.total_votes,
                                    true // reset votepay_share to zero after updating
                                 );

      int64_t producer_per_vote_pay = 0;
      if( _gstate2.revision > 0 ) {
         double total_votepay_share = update_total_votepay_share( ct );
         if( total_votepay_share > 0 && !crossed_threshold ) {
            producer_per_vote_pay = int64_t((new_votepay_share * _gstate.pervote_bucket) / total_votepay_share);
            if( producer_per_vote_pay > _gstate.pervote_bucket )
               producer_per_vote_pay = _gstate.pervote_bucket;
         }
      } else {
         if( _gstate.total_producer_vote_weight > 0 ) {
            producer_per_vote_pay = int64_t((_gstate.pervote_bucket * prod.total_votes) / _gstate.total_producer_vote_weight);
         }
      }

      if( producer_per_vote_pay < min_pervote_daily_pay ) {
         producer_per_vote_pay = 0;
      }

      _gstate.pervote_bucket      -= producer_per_vote_pay;
      _gstate.perblock_bucket     -= producer_per_block_pay;
      _gstate.total_unpaid_blocks -= prod.unpaid_blocks;

      update_total_votepay_share( ct, -new_votepay_share, (updated_after_threshold ? prod.total_votes : 0.0) );

      _producers.modify( prod, same_payer, [&](auto& p) {
         p.last_claim_time = ct;
         p.unpaid_blocks   = 0;
      });

      if( producer_per_block_pay > 0 ) {
         INLINE_ACTION_SENDER(eosio::token, transfer)(
            token_account, { {bpay_account, active_permission}, {owner, active_permission} },
            { bpay_account, owner, asset(producer_per_block_pay, core_symbol()), std::string("producer block pay") }
         );
      }
      if( producer_per_vote_pay > 0 ) {
         INLINE_ACTION_SENDER(eosio::token, transfer)(
            token_account, { {vpay_account, active_permission}, {owner, active_permission} },
            { vpay_account, owner, asset(producer_per_vote_pay, core_symbol()), std::string("producer vote pay") }
         );
      }
   }

} //namespace eosiosystem
