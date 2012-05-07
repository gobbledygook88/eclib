// tsat2.cc -- test for saturate.h/cc reading from gens files directly
//////////////////////////////////////////////////////////////////////////
//
// Copyright 1990-2012 John Cremona
// 
// This file is part of the mwrank package.
// 
// mwrank is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
// 
// mwrank is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
// 
// You should have received a copy of the GNU General Public License
// along with mwrank; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
// 
//////////////////////////////////////////////////////////////////////////
//

#include <eclib/interface.h>
#include <eclib/matrix.h>
#include <eclib/curve.h>
#include <eclib/points.h>
#include <eclib/cperiods.h>
#include <eclib/polys.h>
#include <eclib/curvemod.h>
#include <eclib/pointsmod.h>
#include <eclib/saturate.h>
#include <eclib/elog.h>
#include <eclib/sieve_search.h>
#include <eclib/mwprocs.h>
#include <eclib/curvesort.h>

#define PMIN 2
#define PMAX 100

int main()
{
  set_precision(100);
  initprimes("PRIMES",0);
  int verbose = 1;
  //  cout<<"verbose (0/1)? ";             cin >>verbose;
  int i, j, npts;

  long N, nclass, ncurve;
  Curve E;

  long curvecount=0;
  long okcount=0;
  long upcount=0;
  vector<vector<long> > keeplist;  // list of curves which were not saturated, for report at end

  while(1) {
    cin >> N; if(N==0) break;
  cin >> nclass >> ncurve;
  cin >> E;
  curvecount++;

  Curvedata C(E);
  saturator sieve(&C,verbose);

  cout<<endl;
  cout<<"==============================================================="<<endl;
  cout<<endl;
  cout << N<<codeletter(nclass-1)<<ncurve<<" = "<< E << endl;
  Point P(C);
  cin >> npts;
  vector<Point> points; points.reserve(npts);
  j=0; 
  while(j<npts)
    { 
      cin >> P;
      if ( P.isvalid() ) {points.push_back(P); j++;}
      else {cout<<"point "<<P<<" not on curve.\n\n"; abort();}
    }
  cout<<npts<<" points entered:"<<points<<endl;

  int pmax=PMAX;

  sieve.set_points(points);
  int index = sieve.do_saturation_upto(pmax);

  cout<<"Finished p-saturation for p up to "<<pmax;
  if(index>1) 
    {
      cout<<", index gain = "<<index<<endl;
      vector<Point> newpoints = sieve.getgens();
      cout<<"New generators:\n"<<newpoints<<endl;
      upcount++;
      vector<long> keep;
      keep.push_back(N);
      keep.push_back(nclass);
      keep.push_back(ncurve);
      keep.push_back(index);
      keeplist.push_back(keep);
    }
  else
    {
      cout<<", points were saturated"<<endl;
      okcount++;
    }
  }

  cout<<endl;
  cout<<"==============================================================="<<endl;
  cout<<endl;
  cout<<"Number of curves entered:    "<<curvecount<<endl;
  cout<<"Number saturated already:    "<<okcount<<endl;
  cout<<"Number which were saturated: "<<upcount<<endl;
  //  keeper.close();
  if(upcount>0)
    {
      cout<<"Curves which needed saturation: "<<endl;
      for(i=0; i<upcount; i++)
	{
	  vector<long> keep = keeplist[i];
	  cout<<keep[0]<<codeletter(keep[1]-1)<<keep[2]<<": index gained = "<<keep[3]<<endl;
	}
    }
}

//end of file tsat2.cc




