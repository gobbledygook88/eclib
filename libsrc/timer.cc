// timer.cc:  implementations of timer functions
//////////////////////////////////////////////////////////////////////////
//
// Copyright 1990-2012 John Cremona
//                     Marcus Mo     (timer class)
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
 
#include <iostream>
#include <iomanip>
#include <eclib/interface.h>
#include <eclib/timer.h>

#ifndef CLK_TCK
#define CLK_TCK	CLOCKS_PER_SEC
#endif

double starttime,stoptime;
void init_time() { ;}
void start_time() { starttime = GetTime();}
void stop_time()  { stoptime  = GetTime();}
void show_time(ostream& s) 
{
  s<<" ("<<(stoptime-starttime)<<" seconds)" << flush; 
}

// Implementation of timer class 
// (essentially the same as above and more)
/**
 * timer()
 *
 * Default constructor.
 */
timer::timer() : s_( NULL ) {
  // Set up default stream to standard-out
  stream();
}

/**
 * timer()
 *
 * Constructor. Output filename given.
 */
timer::timer( string filename ) : s_( NULL ) {
  // Set up stream 
  stream( filename );
}

/**
 * ~timer()
 *
 * Destructor. Flush the stream before
 * destroying object.
 */
timer::~timer() {
  s_ -> flush();

  // Close file, if ever opened
  // Do not reference s_ _ever_. We do not
  // want to close std::cout nonetheless.
  if( file_ != NULL ) {
    file_.close();
  }
}

/**
 * stream()
 *
 * Initialise stream for current instance.
 * If called again, new stream will replace old stream,
 * which will be flushed and closed safely, if required.
 */
void timer::stream( string filename ) {
  // Flush current stream if defined, just to be safe
  if( s_ != NULL ) {
    s_ -> flush();
  }

  // First check if a filename was given
  if( !filename.size() ) {
    // Default stream to standard-out
    s_ = &cout;
  } else {
    // Open new file
    file_.open(filename.c_str(),ios::out|ios::trunc);
    
    // Check is file successfully opened
    if( file_ == NULL ) {
      cout << "File " << filename << " could not be opened ... exiting" << endl;
      exit( EXIT_FAILURE );
    }

    // Point main reference to newly opened file
    s_ = &file_;
  }
}

/**
 * add()
 *
 * Create additional subtimers, referenced by unique
 * name, which we check for and notify before any alterations.
 * 
 * This function essentially does nothing apart from 
 * check timer names for validity.
 */
void timer::add( string name ) {
  // Check if default timer is being added
  if( name.compare("default") == 0 ) {
    cout << "Timer of name `default' cannot be used. "
         << "Try another name ... exiting" << endl;
    exit( EXIT_FAILURE );
  }

  // Existence based on length of vector
  if( times_[name].size() != 0 ) {
    cout << "Subtimer " << name << " already exists. "
         << "Erasing, and starting again." << endl;

    // Clear timer
    times_[name].empty();
  }
}

/**
 * start()
 *
 * Stores current time into timer storage.
 * Start time positioned at index 0.
 * Purely semanitc function - Can use split() 
 * which will perform the same action on an empty vector.
 */
void timer::start( string name ) {
  split( name );
}

/**
 * split()
 *
 * Logs the current time for specified timer.
 */
void timer::split( string name ) {
  times_[name].push_back( getWallTime() );
}

/**
 * stop()
 *
 * Store current time into timer storage.
 * Purely semantic.
 */
void timer::stop( string name ) {
  split( name );
}

/**
 * stopAll()
 *
 * Stop all timers.
 */
void timer::stopAll() {
  timers::iterator it;

  // TODO Check if timer is active via flags

  for( it = times_.begin(); it != times_.end(); it++ ) {
    stop( it -> first );
  }
}

/**
 * write()
 *
 * Write given message to output stream
 * defined upon construction of instance.
 */
void timer::write( string message ) {
  s_ -> write( message.c_str(), message.size() );
  s_ -> flush();
}

/**
 * show()
 *
 * Display time to specified stream.
 * Time interval length is defined by two indexes,
 * which default to 0 and 1.
 * Allow option to add newline character to stream
 * for accessibility.
 */
void timer::show( int nline, string name, int idx1, int idx2 ) { 
  // Set second index to end of list
  if( idx2 == -1 ) {
    idx2 = times_[name].size() - 1;
  }
  
  // Calculate difference in time
  double diff = times_[name][idx2] - times_[name][idx1];

  // Compose message
  string message = name + " (" + toString( diff ) + " seconds)\t";
  
  // Add newline
  if( nline ) message += "\n";

  s_ -> write( message.c_str(), message.size() );
  s_ -> flush(); 
}

/**
 * showAll()
 *
 * Loop through all timers, and write out.
 */
void timer::showAll( int nline ) {
  timers::iterator it;
 
  for( it = times_.begin(); it != times_.end(); it++ ) {
    show( nline, it -> first );
  }
}

/**
 * clear()
 *
 * Removes all times from a subtimer.
 */
void timer::clear( string name ) {
  times_[name].clear();
}

/**
 * clearAll()
 *
 * Removes all times from all subtimers.
 */
void timer::clearAll() {
  timers::iterator it;

  for( it = times_.begin(); it != times_.end(); it++ ) {
    clear( it -> first );
  }
}

/**
 * list()
 *
 * List names of all available timers
 */
void timer::list() {
  timers::iterator it;

  string message;

  for( it = times_.begin(); it != times_.end(); it++ ) {
    message += it -> first + " ";
  }

  message += "\n";
  s_ -> write( message.c_str(), message.size() );
  s_ -> flush();
}

/**
 * count()
 *
 * Returns number times stores in specified timer,
 * i.e. size of vector
 */
int timer::count( string name ) {
  return times_[name].size();
}

/**
 * total()
 *
 * Return total time of a given timer.
 */
double timer::total( string name ) {
  // Cache size of vector
  vector<double>::size_type size = times_[name].size();
  double total = 0;
  
  for( int i = 0; i < size; i++ ) {
    total += times_[name][i];
  }

  return total;
}

/**
 * average()
 *
 * Return average time of a given timer.
 */
double timer::average( string name ) {
  // Cache size of vector
  vector<double>::size_type size = times_[name].size();

  return total( name ) / size;
}

/**
 * getWallTime()
 *
 * Returns the real time in seconds. Only use this method
 * to find elapsed time. Do not base calculations on this 
 * method. Subject to changes in system time.
 * Accuracy varies over different systems.
 */
double timer::getWallTime() {
#ifdef _WIN32
  // Windows real time clock
  FILETIME  tm;
  ULONGLONG t;

  GetSystemTimeAsFileTime( &tm );
  
  t = ((ULONGLONG)tm.dwHighDateTime << 32) | (ULONGLONG)tm.dwLowDateTime;
  return (double) t / 10000000.0;
#else
  // POSIX real time clock
  struct timeval tm;

  gettimeofday( &tm, NULL );

  return (double) tm.tv_sec + (double) tm.tv_usec / 1000000.0;
#endif
}

/** 
 * toString()
 *
 * Convert anything to string.
 * Templated function, so possible to 
 * pass through any typed variable, but
 * mainly used to convert numbers to string type.
 */
template< typename T >
string timer::toString( T el ) {
  stringstream s;
  s << el;
  return s.str();
}
