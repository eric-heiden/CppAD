/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
/*
! WARNING: This file is used as an example by omh/FunConstruct.omh and
by CppAD/local/FunOpSeq.h.

$begin FunCheck.cpp$$
$spell
	abs
$$

$section ADFun Check and Re-Tape: Example and Test$$

$index FunCheck, example$$
$index example, FunCheck$$
$index test, FunCheck$$

$index ADFun, default constructor$$
$index operation, store new sequence$$

$code
$verbatim%Example/FunCheck.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <CppAD/CppAD.h>

namespace { // -----------------------------------------------------------
// define the template function object Fun<Type,Vector> in empty namespace
template <class Type, class Vector>
class Fun {
private:
	size_t n;
public:
	// function constructor
	Fun(size_t n_) : n(n_)
	{ }
	// function evaluator
	Vector operator() (const Vector &x)
	{	Vector y(n);
		size_t i;
		for(i = 0; i < n; i++)
		{	// This operaiton sequence depends on x
			if( x[i] >= 0 ) 
				y[i] = exp(x[i]);
			else	y[i] = exp(-x[i]);
		}
		return y;
	}	
};
// template function FunCheckCases<Vector, ADVector> in empty namespace
template <class Vector, class ADVector>
bool FunCheckCases(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::ADFun;

	// default constructor
	ADFun<double> f;

	// domain space vector
	size_t n = 2;
	ADVector X(n);
	X[0] = -1.;
	X[1] = 1.;

	// declare independent variables and starting recording
	CppAD::Independent(X);

	// create function object to use with AD<double>
	Fun< AD<double>, ADVector > G(n);

	// range space vector
	size_t m = n;
	ADVector Y(m);
	Y = G(X);

	// stop tape and store operation sequence in f : X -> Y
	f(X, Y);
	ok &= (f.size_taylor() == 0);

	// create function object to use with double
	Fun<double, Vector> g(n);

	// function values should agree when the independent variable 
	// values are the same as during recording
	Vector x(n);
	size_t j;
	for(j = 0; j < n; j++)
		x[j] = Value(X[j]);
	double r = 1e-10; 
	double a = 1e-10;
	ok      &= FunCheck(f, g, x, a, r);

	// function values should not agree when the independent variable
	// values are the negative of values during recording
	for(j = 0; j < n; j++)
		x[j] = - Value(X[j]);
	ok      &= ! FunCheck(f, g, x, a, r);

	// re-tape to obtain the new AD of double operation sequence
	for(j = 0; j < n; j++)
		X[j] = x[j];
	CppAD::Independent(X);
	Y = G(X);

	// stop tape and store operation sequence in f : X -> Y
	f(X, Y);

	// function values should agree now
	ok      &= FunCheck(f, g, x, a, r);

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool FunCheck(void)
{	bool ok = true;
	typedef CppAD::vector<double>                Vector1;
	typedef CppAD::vector< CppAD::AD<double> > ADVector1;
	typedef   std::vector<double>                Vector2;
	typedef   std::vector< CppAD::AD<double> > ADVector2;
	typedef std::valarray<double>                Vector3;
	typedef std::valarray< CppAD::AD<double> > ADVector3;
	// Run with Vector and ADVector equal to three different cases
	// all of which are Simple Vectors with elements of type 
	// double and AD<double> respectively.
	ok &= FunCheckCases< Vector1, ADVector2 >();
	ok &= FunCheckCases< Vector2, ADVector3 >();
	ok &= FunCheckCases< Vector3, ADVector1 >();
	return ok;
}
// END PROGRAM
