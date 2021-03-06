#include "trade.hpp"

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

// Order::Order(string _symbol, int _quantity, double _price, bool _buy,
// 		     string _expiration_date, string _transaction_dt, string _trader) :
// 	buy {_buy},
// 	price {_price},
// 	quantity {_quantity},
// 	expiration_date {_expiration_date},
// 	symbol {_symbol},
// 	trader_id {_trader},
// 	transaction_datetime {_transaction_dt} {
// }

Order::Order(string s, int q, bool b, string exp, string trader) :
buy {b},
quantity {q},
expiration_date {exp},
symbol {s},
trader_id {trader} {
}


Order::~Order() {}

string Order::toSQL() {
	ostringstream sql;
	sql << "VALUES ("
		<< "'" << symbol << "', "
		<< quantity << ", "
		// << price << ", "
		<< (buy ? "'true', " : "'false', ")
		<< "'" << trader_id << "', "
		<< "'" << expiration_date << "', "
		<< "'" << transaction_datetime << "', "
		<< order_type
		<< ");";

	return sql.str();
}

string Order::toCSV() {
	ostringstream csv;
	csv << symbol << ", "
		<< quantity << ", "
		// << price << ", "
		<< (buy ? "BUY, " : "SELL, ")
		<< expiration_date << ", "
		<< transaction_datetime << ", "
		<< trader_id
		<< "\n";

	return csv.str();
}

Aggregate::Aggregate(string _symbol, string _trader, int _quantity, bool _buy) :
	symbol {_symbol}, trader_id {_trader}, positions {} {
		positions.push_back(Position {_quantity, _buy});
}

Aggregate::~Aggregate() {};

string Aggregate::toCSV() {

	// Calculate aggregate quantity for this (symbol, trader) trade
	int aggregate_quantity = 0;
	for (auto &p : positions) {
		aggregate_quantity += (p.buy ? p.quantity : -1*p.quantity);
	}

	ostringstream csv;
	csv << symbol << ", "
		<< aggregate_quantity << ", "
		<< (aggregate_quantity > 0 ? "BUY, " : "SELL, ")
		<< trader_id
		<< "\n";

	return csv.str();
}

void Aggregate::add_position(int _qty, bool _buy) {
	positions.push_back({_qty, _buy});
}

void Aggregate::account_trade(vector<Aggregate>& v, string _symbol, string _trader, int _qty, bool _buy) {
	// check if (symbol, trader) is alread in container
	for (auto& p : v) {
		// (symbol, trader) found
		if (_symbol.compare(p.symbol) == 0 && _trader.compare(p.trader_id) == 0) {
			p.add_position(_qty, _buy);
			return;
		}
	}

	v.push_back(Aggregate(_symbol, _trader, _qty, _buy));
}

LimitOrder::LimitOrder(string s, int q, double p, bool b, string exp, string trader) :
Order(s, q, b, exp, trader), limitPrice {p} {
}

PeggedOrder::PeggedOrder(string s, int q, bool b, string exp, string trader) :
Order(s, q, b, exp, trader) {
}
