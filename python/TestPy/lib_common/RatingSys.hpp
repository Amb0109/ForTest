#ifndef _RATING_SYSTEM_HPP_
#define _RATING_SYSTEM_HPP_

#include "stdafx.h"

// ��׼ ELO_rating �㷨�� ��� bIsDrawΪ�� ��ǰ������λ�ÿ��Ի���

void StdELORating(double ELO_K, double& WinnerScore, double &LoserScore, bool bIsDraw = false)
{
	double Ea = 1.f / (1.f + pow(10.f, (LoserScore - WinnerScore) / 400.f) );
	double Eb = 1.f / (1.f + pow(10.f, (WinnerScore - LoserScore) / 400.f) );

	double K = ELO_K;

	double Sa = 1.f;
	double Sb = 0.f;
	if (bIsDraw)
	{
		Sa = Sb = 0.5f;
	}

	WinnerScore = WinnerScore + K * (Sa - Ea);
	LoserScore = LoserScore + K * (Sb - Eb);
}

void StdELORating16(double& WinnerScore, double &LoserScore, bool bIsDraw = false)
{
	StdELORating(16, WinnerScore, LoserScore, bIsDraw);
}

void StdELORating32(double& WinnerScore, double &LoserScore, bool bIsDraw = false)
{
	StdELORating(32, WinnerScore, LoserScore, bIsDraw);
}


#endif // _RATING_SYSTEM_HPP_
