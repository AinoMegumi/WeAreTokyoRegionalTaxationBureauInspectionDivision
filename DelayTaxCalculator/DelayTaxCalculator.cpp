// DelayTaxCalculator.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "Calendar.hpp"
#include <utility>
#include <stdexcept>

// 第１引数：本税の追徴金
// 第２引数：本税の法定納付期限
// 第３引数：本税の完結日
int CalcDelayTax(const int SurchargeOfMainTax, const Calendar MainTaxPaymentLimit, const Calendar DayOfMainTaxIsCompleted) {
	const Calendar Delay = DayOfMainTaxIsCompleted - MainTaxPaymentLimit;
	if (Delay.Month < 2) {
		if (MainTaxPaymentLimit.Year == DayOfMainTaxIsCompleted.Year && MainTaxPaymentLimit.Month <= 2)
	}
}
