#ifndef FIX8_64625F73657373696F6E2E687070_
#define FIX8_64625F73657373696F6E2E687070_

#include <fix8/f8config.h>

#if defined FIX8_MAGIC_NUM && FIX8_MAGIC_NUM > 16793603L
#error db_session.hpp version 1.4.3 is out of date. Please regenerate with f8c.
#endif

#include "fix8/f8includes.hpp"
#include "db_router.hpp"

//-------------------------------------------------------------------------------------------------
// server session and router classes
//-------------------------------------------------------------------------------------------------
class db_session_server;

class db_router_server : public FIX8::FIX4200::db_Router
{
   db_session_server& _session;

public:
   db_router_server(db_session_server& session) : _session(session) {}
   virtual ~db_router_server() {}

   // Override these methods to receive specific message callbacks.
   // bool operator() (const FIX8::FIX4200::Heartbeat *msg) const;
   // bool operator() (const FIX8::FIX4200::TestRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::ResendRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::Reject *msg) const;
   // bool operator() (const FIX8::FIX4200::SequenceReset *msg) const;
   // bool operator() (const FIX8::FIX4200::Logout *msg) const;
   // bool operator() (const FIX8::FIX4200::IndicationofInterest *msg) const;
   // bool operator() (const FIX8::FIX4200::Advertisement *msg) const;
   // bool operator() (const FIX8::FIX4200::ExecutionReport *msg) const;
   // bool operator() (const FIX8::FIX4200::OrderCancelReject *msg) const;
   // bool operator() (const FIX8::FIX4200::Logon *msg) const;
   // bool operator() (const FIX8::FIX4200::News *msg) const;
   // bool operator() (const FIX8::FIX4200::Email *msg) const;
   // bool operator() (const FIX8::FIX4200::NewOrderSingle *msg) const;
   // bool operator() (const FIX8::FIX4200::NewOrderList *msg) const;
   // bool operator() (const FIX8::FIX4200::OrderCancelRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::OrderCancelReplaceRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::OrderStatusRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::Allocation *msg) const;
   // bool operator() (const FIX8::FIX4200::ListCancelRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::ListExecute *msg) const;
   // bool operator() (const FIX8::FIX4200::ListStatusRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::ListStatus *msg) const;
   // bool operator() (const FIX8::FIX4200::AllocationACK *msg) const;
   // bool operator() (const FIX8::FIX4200::DontKnowTrade *msg) const;
   // bool operator() (const FIX8::FIX4200::QuoteRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::Quote *msg) const;
   // bool operator() (const FIX8::FIX4200::SettlementInstructions *msg) const;
   // bool operator() (const FIX8::FIX4200::MarketDataRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::MarketDataSnapshotFullRefresh *msg) const;
   // bool operator() (const FIX8::FIX4200::MarketDataIncrementalRefresh *msg) const;
   // bool operator() (const FIX8::FIX4200::MarketDataRequestReject *msg) const;
   // bool operator() (const FIX8::FIX4200::QuoteCancel *msg) const;
   // bool operator() (const FIX8::FIX4200::QuoteStatusRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::QuoteAcknowledgement *msg) const;
   // bool operator() (const FIX8::FIX4200::SecurityDefinitionRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::SecurityDefinition *msg) const;
   // bool operator() (const FIX8::FIX4200::SecurityStatusRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::SecurityStatus *msg) const;
   // bool operator() (const FIX8::FIX4200::TradingSessionStatusRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::TradingSessionStatus *msg) const;
   // bool operator() (const FIX8::FIX4200::MassQuote *msg) const;
   // bool operator() (const FIX8::FIX4200::BusinessMessageReject *msg) const;
   // bool operator() (const FIX8::FIX4200::BidRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::BidResponse *msg) const;
   // bool operator() (const FIX8::FIX4200::ListStrikePrice *msg) const;
};

//-------------------------------------------------------------------------------------------------
class db_session_server : public FIX8::Session
{
   db_router_server _router;

public:
   db_session_server(const FIX8::F8MetaCntx &ctx, const FIX8::sender_comp_id &sci, FIX8::Persister *persist = 0,
                     FIX8::Logger *logger = 0, FIX8::Logger *plogger = 0)
       : Session(ctx, sci, persist, logger, plogger), _router(*this) {}

   // Override these methods if required but remember to call the base class method first.

   // bool handle_logon(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_logon(const unsigned heartbeat_interval, const f8String davi=f8String());
   // bool handle_logout(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_logout();
   // bool handle_heartbeat(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_heartbeat(const f8String& testReqID);
   // bool handle_resend_request(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_resend_request(const unsigned begin, const unsigned end=0);
   // bool handle_sequence_reset(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_sequence_reset(const unsigned newseqnum, const bool gapfillflag=false);
   // bool handle_test_request(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_test_request(const f8String& testReqID);
   // bool handle_reject(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_reject(const unsigned seqnum, const char *what);
   // bool handle_admin(const unsigned seqnum, const FIX8::Message *msg);
   // void modify_outbound(FIX8::Message *msg);
   // bool authenticate(SessionID& id, const FIX8::Message *msg);

   // Override these methods to intercept admin and application methods.
   // bool handle_admin(const unsigned seqnum, const FIX8::Message *msg);

   bool handle_application(const unsigned seqnum,
                           const FIX8::Message *&msg) override;
};

class db_session_client;

class db_router_client : public FIX8::FIX4200::db_Router
{
   db_session_client& _session;

public:
   db_router_client(db_session_client& session) : _session(session) {}
   virtual ~db_router_client() {}

   // Override these methods to receive specific message callbacks.
   // bool operator() (const FIX8::FIX4200::Heartbeat *msg) const;
   // bool operator() (const FIX8::FIX4200::TestRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::ResendRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::Reject *msg) const;
   // bool operator() (const FIX8::FIX4200::SequenceReset *msg) const;
   // bool operator() (const FIX8::FIX4200::Logout *msg) const;
   // bool operator() (const FIX8::FIX4200::IndicationofInterest *msg) const;
   // bool operator() (const FIX8::FIX4200::Advertisement *msg) const;
   // bool operator() (const FIX8::FIX4200::ExecutionReport *msg) const;
   // bool operator() (const FIX8::FIX4200::OrderCancelReject *msg) const;
   // bool operator() (const FIX8::FIX4200::Logon *msg) const;
   // bool operator() (const FIX8::FIX4200::News *msg) const;
   // bool operator() (const FIX8::FIX4200::Email *msg) const;
   // bool operator() (const FIX8::FIX4200::NewOrderSingle *msg) const;
   // bool operator() (const FIX8::FIX4200::NewOrderList *msg) const;
   // bool operator() (const FIX8::FIX4200::OrderCancelRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::OrderCancelReplaceRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::OrderStatusRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::Allocation *msg) const;
   // bool operator() (const FIX8::FIX4200::ListCancelRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::ListExecute *msg) const;
   // bool operator() (const FIX8::FIX4200::ListStatusRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::ListStatus *msg) const;
   // bool operator() (const FIX8::FIX4200::AllocationACK *msg) const;
   // bool operator() (const FIX8::FIX4200::DontKnowTrade *msg) const;
   // bool operator() (const FIX8::FIX4200::QuoteRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::Quote *msg) const;
   // bool operator() (const FIX8::FIX4200::SettlementInstructions *msg) const;
   // bool operator() (const FIX8::FIX4200::MarketDataRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::MarketDataSnapshotFullRefresh *msg) const;
   // bool operator() (const FIX8::FIX4200::MarketDataIncrementalRefresh *msg) const;
   // bool operator() (const FIX8::FIX4200::MarketDataRequestReject *msg) const;
   // bool operator() (const FIX8::FIX4200::QuoteCancel *msg) const;
   // bool operator() (const FIX8::FIX4200::QuoteStatusRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::QuoteAcknowledgement *msg) const;
   // bool operator() (const FIX8::FIX4200::SecurityDefinitionRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::SecurityDefinition *msg) const;
   // bool operator() (const FIX8::FIX4200::SecurityStatusRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::SecurityStatus *msg) const;
   // bool operator() (const FIX8::FIX4200::TradingSessionStatusRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::TradingSessionStatus *msg) const;
   // bool operator() (const FIX8::FIX4200::MassQuote *msg) const;
   // bool operator() (const FIX8::FIX4200::BusinessMessageReject *msg) const;
   // bool operator() (const FIX8::FIX4200::BidRequest *msg) const;
   // bool operator() (const FIX8::FIX4200::BidResponse *msg) const;
   // bool operator() (const FIX8::FIX4200::ListStrikePrice *msg) const;
};

//-------------------------------------------------------------------------------------------------
class db_session_client : public FIX8::Session
{
   db_router_client _router;

public:
   db_session_client(const FIX8::F8MetaCntx& ctx, const FIX8::SessionID& sid, FIX8::Persister *persist=0,
      FIX8::Logger *logger=0, FIX8::Logger *plogger=0) : Session(ctx, sid, persist, logger, plogger), _router(*this) {}

   // Override these methods if required but remember to call the base class method first.
   // bool handle_logon(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_logon(const unsigned heartbeat_interval, const f8String davi=f8String());
   // bool handle_logout(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_logout();
   // bool handle_heartbeat(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_heartbeat(const f8String& testReqID);
   // bool handle_resend_request(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_resend_request(const unsigned begin, const unsigned end=0);
   // bool handle_sequence_reset(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_sequence_reset(const unsigned newseqnum, const bool gapfillflag=false);
   // bool handle_test_request(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_test_request(const f8String& testReqID);
   // bool handle_reject(const unsigned seqnum, const FIX8::Message *msg);
   // Message *generate_reject(const unsigned seqnum, const char *what);
   // bool handle_admin(const unsigned seqnum, const FIX8::Message *msg);
   // void modify_outbound(FIX8::Message *msg);
   // bool authenticate(SessionID& id, const FIX8::Message *msg);

   // Override these methods to intercept admin and application methods.
   // bool handle_admin(const unsigned seqnum, const FIX8::Message *msg);

   bool handle_application(const unsigned seqnum,
                           const FIX8::Message *&msg) override;
   /* In your compilation unit, this should be implemented with something like the following:
   bool db_session_client::handle_application(const unsigned seqnum, const FIX8::Message *&msg)
   {
      return enforce(seqnum, msg) || msg->process(_router);
   }
   */
};

#endif // FIX8_64625F73657373696F6E2E687070_
