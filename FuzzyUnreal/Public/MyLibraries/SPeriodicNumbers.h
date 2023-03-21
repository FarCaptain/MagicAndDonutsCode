/*
Coding Quickie: Handling Periodic Numbers with Templates
	"Don't need no ceiling" - javidx9
	
	License (OLC-3)
	~~~~~~~~~~~~~~~
	Copyright 2018-2023 OneLoneCoder.com

	Modified by Lucas Qu
*/

#pragma once

class FUZZYUNREAL_API SPeriodicNumbers
{
public:
	SPeriodicNumbers();
	~SPeriodicNumbers();

	template<class TL, class TR>
	static constexpr auto circ_add( const TL& lhs, const TR& rhs, const decltype(lhs + rhs) rmin = 0, const decltype(lhs + rhs) rmax = 360);

	template<class TL, class TR>
	static constexpr auto circ_sub(const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin = 0, const decltype(lhs - rhs) rmax = 360);

	template<class TL, class TR>
	static constexpr auto circ_mul(const TL& lhs, const TR& rhs, const decltype(lhs * rhs) rmin = 0, const decltype(lhs * rhs) rmax = 360);

	template<class TL, class TR>
	static constexpr auto circ_div(const TL& lhs, const TR& rhs, const decltype(lhs / rhs) rmin = 0, const decltype(lhs / rhs) rmax = 360);


	template<class TL, class TR>
	static constexpr auto circ_lerp(const double t, const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin = 0, const decltype(lhs - rhs) rmax = 360);
	template<class TL, class TR>
	static constexpr auto circ_shortdist(const TL& lhs, const TR& rhs, const decltype(lhs-rhs) rmin = 0, const decltype(lhs - rhs) rmax = 360);
	template<class TL, class TR>
	static constexpr auto circ_longdist(const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin = 0, const decltype(lhs - rhs) rmax = 360);

	template<class TL, class TR>
	static constexpr auto circ_shortdiff(const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin = 0, const decltype(lhs - rhs) rmax = 360);

	template<class TL, class TR>
	static constexpr auto circ_shortlerp(const double t, const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin = 0, const decltype(lhs - rhs) rmax = 360);
};


template<class TL, class TR>
constexpr auto SPeriodicNumbers::circ_add(
	const TL& lhs, 
	const TR& rhs, 
	const decltype(lhs + rhs) rmin, 
	const decltype(lhs + rhs) rmax)
{
	auto c = lhs + rhs;
	auto range = rmax - rmin;
	while (c >= rmax)  c -= range;
	while (c <  rmin)  c += range;
	return c;
}

template<class TL, class TR>
constexpr auto SPeriodicNumbers::circ_sub(const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin, const decltype(lhs - rhs) rmax)
{
	auto c = lhs - rhs;
	auto range = rmax - rmin;
	while (c >= rmax)  c -= range;
	while (c < rmin)  c += range;
	return c;
}

template<class TL, class TR>
constexpr auto SPeriodicNumbers::circ_mul(const TL& lhs, const TR& rhs, const decltype(lhs * rhs) rmin, const decltype(lhs * rhs) rmax)
{
	auto c = lhs * rhs;
	auto range = rmax - rmin;
	while (c >= rmax)  c -= range;
	while (c < rmin)  c += range;
	return c;
}

template<class TL, class TR>
constexpr auto SPeriodicNumbers::circ_div(const TL& lhs, const TR& rhs, const decltype(lhs / rhs) rmin, const decltype(lhs / rhs) rmax)
{
	auto c = lhs / rhs;
	auto range = rmax - rmin;
	while (c >= rmax)  c -= range;
	while (c < rmin)  c += range;
	return c;
}


template<class TL, class TR>
constexpr auto SPeriodicNumbers::circ_lerp(const double t, const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin, const decltype(lhs - rhs) rmax)
{
	return circ_add(lhs, circ_sub(rhs, lhs, rmin, rmax) * t, rmin, rmax);
}

template<class TL, class TR>
constexpr auto SPeriodicNumbers::circ_shortdist(const TL& lhs, const TR& rhs, const decltype(lhs-rhs) rmin, const decltype(lhs - rhs) rmax)
{
	auto a = circ_sub(rhs, lhs, rmin, rmax);
	auto b = circ_sub(lhs, rhs, rmin, rmax);
	return std::min(a, b);
}

template<class TL, class TR>
constexpr auto SPeriodicNumbers::circ_longdist(const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin, const decltype(lhs - rhs) rmax)
{
	auto a = circ_sub(rhs, lhs, rmin, rmax);
	auto b = circ_sub(lhs, rhs, rmin, rmax);
	return std::max(a, b);
}

template<class TL, class TR>
constexpr auto SPeriodicNumbers::circ_shortdiff(const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin, const decltype(lhs - rhs) rmax)
{
	auto a = circ_sub(rhs, lhs, rmin, rmax);
	auto b = circ_sub(lhs, rhs, rmin, rmax);
	if (b > a)
		return a;
	else
		return -b;
}

template<class TL, class TR>
constexpr auto SPeriodicNumbers::circ_shortlerp(const double t, const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin, const decltype(lhs - rhs) rmax)
{
	auto a = circ_sub(rhs, lhs, rmin, rmax);
	auto b = circ_sub(lhs, rhs, rmin, rmax);
	if(a < b)
		return circ_add(lhs, a * t, rmin, rmax);
	else
		return circ_sub(lhs, b * t, rmin, rmax);
}