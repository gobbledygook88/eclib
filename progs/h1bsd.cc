// FILE H1BSD.CC: Program to compute L^(r)(f,1) for newforms 
//////////////////////////////////////////////////////////////////////////
//
// Copyright 1990-2012 John Cremona
// 
// This file is part of the eclib package.
// 
// eclib is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
// 
// eclib is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
// 
// You should have received a copy of the GNU General Public License
// along with eclib; if not, write to the Free Software Foundation,
// Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
// 
//////////////////////////////////////////////////////////////////////////
//
#include <fstream>
#include <eclib/moddata.h>
#include <eclib/symb.h>
#include <eclib/oldforms.h>
#include <eclib/homspace.h>
#include <eclib/cperiods.h>     //from qcurves, for computing conductors
#include <eclib/newforms.h>
#include <eclib/periods.h>
#include <eclib/curvesort.h>

#define AUTOLOOP

int main(void)
{
  set_precision("Enter number of decimal places");
 int limit,n=1; 
#ifdef AUTOLOOP
 cout<<"Enter first and last N: ";cin>>n>>limit; 
 n--; cout<<endl;
 while (n<limit) { n++;
#else
 while (n>0) { cout<<"Enter level: "; cin>>n;
#endif
 if (n>0)
{
 newforms nf(n,0);
 int noldap=25;
 nf.createfromdata(1,noldap,0); // do not create from scratch if data absent
 for(int i=0; i<nf.n1ds; i++)
   {
     //     i=booknumber0(n,i);
     newform& nfi = nf.nflist[i];
     ldash1 x(&nf, &nfi);  
     x.compute();
     bigfloat lf1 = abs(x.value());
     long r = x.rank();
     cout << n << "\t" << codeletter(i) 
	  << "\tRank = " << r << "\tL^(r)(f,1)/r! = " << lf1 << endl;
   }
}       // end of if(n)
}       // end of while()
}       // end of main()
